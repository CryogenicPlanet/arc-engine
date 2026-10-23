// Copyright 2026 ArcMod. Application URL loads use Chromium's system network context.
#include "arc/api/arc_internal.h"
#include <array>
#include <cmath>
#include <optional>
#include "base/functional/bind.h"
#include "base/files/file_path.h"
#include "base/no_destructor.h"
#include "base/uuid.h"
#include "base/task/sequenced_task_runner.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/storage_partition.h"
#include "net/base/net_errors.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "net/base/load_flags.h"
#include "net/http/http_response_headers.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/cpp/simple_url_loader_stream_consumer.h"
#include "services/network/public/mojom/url_loader_factory.mojom.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "services/network/public/mojom/url_response_head.mojom.h"

@protocol ArcURLLoadDelegate <NSObject>
- (void)didProgress:(NSString*)token withProgress:(int)progress;
- (void)didReceiveHeaders:(NSString*)token withFinalURL:(NSString*)url withCode:(int)code withHeaders:(NSDictionary*)headers;
- (void)didReceiveData:(NSString*)token withData:(NSData*)data;
- (void)didFinish:(NSString*)token;
- (void)didFail:(NSString*)token withError:(int)error;
@end

ARC_EXPORT @interface ArcURLLoadConfiguration : NSObject
@property(nonatomic, copy) NSURL* url;
@property(nonatomic) NSUInteger method;
@property(nonatomic, copy) NSString* payload;
@property(nonatomic, copy) NSData* payloadData;
@property(nonatomic, copy) NSURL* payloadFileURL;
@property(nonatomic, copy) NSString* payloadContentType;
@property(nonatomic, copy) NSDictionary* headers;
@property(nonatomic) BOOL followRedirects;
@property(nonatomic) BOOL streamResponseData;
@property(nonatomic) double timeout;
@property(nonatomic) NSUInteger cachePolicy;
@property(nonatomic) BOOL blockLocalRequest;
@end
@implementation ArcURLLoadConfiguration
@synthesize url = _url, method = _method, payloadData = _payloadData;
@synthesize payloadFileURL = _payloadFileURL, payloadContentType = _payloadContentType;
@synthesize headers = _headers, followRedirects = _followRedirects;
@synthesize streamResponseData = _streamResponseData, timeout = _timeout;
@synthesize cachePolicy = _cachePolicy, blockLocalRequest = _blockLocalRequest;
- (instancetype)init {
  if ((self = [super init])) {
    _url = [NSURL URLWithString:@""];
    _payloadFileURL = [NSURL URLWithString:@""];
    _payloadData = [NSData data];
    _payloadContentType = @"";
    _headers = @{};
    _followRedirects = YES;
  }
  return self;
}
- (NSString*)payload { return [[NSString alloc] initWithData:_payloadData encoding:NSUTF8StringEncoding]; }
- (void)setPayload:(NSString*)payload { _payloadData = [payload dataUsingEncoding:NSUTF8StringEncoding] ?: [NSData data]; }
- (void)setPayloadData:(NSData*)data { _payloadData = [data copy] ?: [NSData data]; }
- (void)setHeaders:(NSDictionary*)headers { _headers = [headers copy] ?: @{}; }
- (void)setPayloadContentType:(NSString*)type { _payloadContentType = [type copy] ?: @""; }
- (void)setMethod:(NSUInteger)method { _method = method <= 3 ? method : 0; }
- (void)setCachePolicy:(NSUInteger)policy { _cachePolicy = policy == 1 ? 1 : 0; }
@end

// Chromium invokes this consumer on the same sequence as the load.
class ArcURLStreamConsumer final : public network::SimpleURLLoaderStreamConsumer {
 public:
  ArcURLStreamConsumer(base::RepeatingCallback<bool(std::string_view)> data,
                       network::SimpleURLLoader::BodyAsStringCallback complete)
      : data_(std::move(data)), complete_(std::move(complete)) {}
  void OnDataReceived(std::string_view data, base::OnceClosure resume) override {
    if (data_.Run(data)) std::move(resume).Run();
  }
  void OnComplete(bool success) override { std::move(complete_).Run(std::nullopt); }
  void OnRetry(base::OnceClosure retry) override { std::move(retry).Run(); }
 private:
  base::RepeatingCallback<bool(std::string_view)> data_;
  network::SimpleURLLoader::BodyAsStringCallback complete_;
};

ARC_EXPORT @interface ArcURLLoadCancellationToken : NSObject {
 @public
  // Destroy the loader before the consumer it references.
  std::unique_ptr<ArcURLStreamConsumer> _consumer;
  std::unique_ptr<network::SimpleURLLoader> _loader;
  // Native URL-load adapters retain their delegate until the request ends.
  id<ArcURLLoadDelegate> _delegate;
  NSString* _identifier;
  BOOL _terminal;
}
- (instancetype)initWithToken:(const void*)token;
@end
@implementation ArcURLLoadCancellationToken
- (instancetype)initWithToken:(const void*)token {
  if ((self = [super init])) {
    _identifier = token ? ArcNSString(*static_cast<const std::string*>(token)) : @"";
  }
  return self;
}
@end

// Requests outlive the caller's reference to the cancellation token, until completion or cancellation.
static NSMutableSet<ArcURLLoadCancellationToken*>* PendingLoads() {
  static base::NoDestructor<NSMutableSet<ArcURLLoadCancellationToken*>*> pending(
      [NSMutableSet set]);
  return *pending;
}

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
static ArcURLLoadCancellationToken* LoadURL(ArcURLLoadConfiguration* config, id<ArcURLLoadDelegate> delegate, network::SharedURLLoaderFactory* factory) {
  std::string identifier = base::Uuid::GenerateRandomV4().AsLowercaseString();
  ArcURLLoadCancellationToken* token = [[ArcURLLoadCancellationToken alloc] initWithToken:&identifier];
  token->_delegate = delegate;
  auto request = std::make_unique<network::ResourceRequest>();
  request->url = ArcGURL(config.url);
  constexpr std::array<const char*, 4> methods = {"GET", "HEAD", "POST", "PUT"};
  request->method = methods[config.method <= 3 ? config.method : 0];
  for (NSString* key in config.headers) {
    request->headers.SetHeader(ArcUTF8(key), ArcUTF8(config.headers[key]));
  }
  if (config.cachePolicy == 1) request->load_flags = net::LOAD_BYPASS_CACHE;
  if (!config.followRedirects) request->redirect_mode = network::mojom::RedirectMode::kManual;
  token->_loader = network::SimpleURLLoader::Create(std::move(request), net::DefineNetworkTrafficAnnotation("arc_application_url_loader", R"(
    semantics {
      sender: "Arc application URL loader"
      description: "Fetches resources requested by the host application."
      trigger: "The host application calls its engine URL loader."
      data: "URL, request headers, and optional request body supplied by the application."
      destination: OTHER
      destination_other: "The URL selected by the application."
    }
    policy {
      cookies_allowed: YES
      cookies_store: "System network context"
      setting: "Controlled by the host application."
      policy_exception_justification: "The host application owns request policy."
    })"));
  bool has_upload = false;
  if ((config.method == 2 || config.method == 3) && config.payloadData.length) {
    has_upload = true;
    token->_loader->AttachStringForUpload(std::string(static_cast<const char*>(config.payloadData.bytes), config.payloadData.length), ArcUTF8(config.payloadContentType));
  } else if ((config.method == 2 || config.method == 3) && config.payloadFileURL.isFileURL) {
    has_upload = true;
    token->_loader->AttachFileForUpload(base::FilePath(ArcUTF8(config.payloadFileURL.path)), ArcUTF8(config.payloadContentType));
  }
  if (std::isfinite(config.timeout) && config.timeout > 0) {
    token->_loader->SetTimeoutDuration(base::Seconds(config.timeout));
  }
  token->_loader->SetAllowHttpErrorResults(true);
  if (config.blockLocalRequest) token->_loader->SetURLLoaderFactoryOptions(network::mojom::kURLLoadOptionBlockLocalRequest);
  // Holding only a weak token in the callbacks avoids a loader -> block -> token cycle.
  __weak ArcURLLoadCancellationToken* weakToken = token;
  if (has_upload) {
    token->_loader->SetOnUploadProgressCallback(base::BindRepeating(^(uint64_t position, uint64_t total) {
      ArcURLLoadCancellationToken* current = weakToken;
      if (!current || current->_terminal) return;
      id<ArcURLLoadDelegate> target = current->_delegate;
      int progress = total ? static_cast<int>((position * 100) / total) : 0;
      if ([target respondsToSelector:@selector(didProgress:withProgress:)])
        [target didProgress:current->_identifier withProgress:progress];
    }));
  }
  token->_loader->SetOnResponseStartedCallback(base::BindOnce(^(const GURL& url, const network::mojom::URLResponseHead& head) {
    ArcURLLoadCancellationToken* current = weakToken;
    if (!current || current->_terminal) return;
    NSMutableDictionary* headers = [NSMutableDictionary dictionary];
    int code = 0;
    if (head.headers) {
      code = head.headers->response_code();
      size_t iterator = 0;
      std::string name, value;
      while (head.headers->EnumerateHeaderLines(&iterator, &name, &value)) headers[ArcNSString(name)] = ArcNSString(value);
    }
    id<ArcURLLoadDelegate> target = current->_delegate;
    if ([target respondsToSelector:@selector(didReceiveHeaders:withFinalURL:withCode:withHeaders:)])
      [target didReceiveHeaders:current->_identifier withFinalURL:ArcNSString(url.spec()) withCode:code withHeaders:headers];
  }));
  [PendingLoads() addObject:token];
  auto complete = base::BindOnce(^(std::optional<std::string> body) {
    ArcURLLoadCancellationToken* current = weakToken;
    if (!current || current->_terminal || !current->_loader) return;
    int error = current->_loader->NetError();
    id<ArcURLLoadDelegate> target = current->_delegate;
    if (error) {
      current->_terminal = YES;
      if ([target respondsToSelector:@selector(didFail:withError:)]) [target didFail:current->_identifier withError:error];
    } else {
      if (body && [target respondsToSelector:@selector(didReceiveData:withData:)])
        [target didReceiveData:current->_identifier withData:[NSData dataWithBytes:body->data() length:body->size()]];
      // A delegate may cancel inside didReceiveData:.
      if (!current->_terminal && current->_loader) {
        current->_terminal = YES;
        if ([target respondsToSelector:@selector(didFinish:)]) [target didFinish:current->_identifier];
      }
    }
    // OnComplete may be running on the consumer itself. Keep it alive until
    // the callback stack unwinds, even if the caller discarded its token.
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
      current->_loader.reset();
      current->_consumer.reset();
      current->_delegate = nil;
    }));
    [PendingLoads() removeObject:current];
  });
  if (config.streamResponseData) {
    token->_consumer = std::make_unique<ArcURLStreamConsumer>(base::BindRepeating(^(std::string_view data) {
      ArcURLLoadCancellationToken* current = weakToken;
      if (!current || current->_terminal || !current->_loader) return false;
      id<ArcURLLoadDelegate> target = current->_delegate;
      if ([target respondsToSelector:@selector(didReceiveData:withData:)])
        [target didReceiveData:current->_identifier withData:[NSData dataWithBytes:data.data() length:data.size()]];
      return !current->_terminal && current->_loader != nullptr;
    }), std::move(complete));
    token->_loader->DownloadAsStream(factory, token->_consumer.get());
  } else {
    // The native Arc loader uses an effectively unbounded response buffer.
    token->_loader->DownloadToStringOfUnboundedSizeUntilCrashAndDie(factory, std::move(complete));
  }
  return token;
}
ArcURLLoadCancellationToken* ArcStartURLLoad(Profile* profile, ArcURLLoadConfiguration* configuration, id delegate) {
  scoped_refptr<network::SharedURLLoaderFactory> factory = profile
      ? profile->GetDefaultStoragePartition()->GetURLLoaderFactoryForBrowserProcess()
      : SystemNetworkContextManager::GetInstance()->GetSharedURLLoaderFactory();
  return LoadURL(configuration, delegate, factory.get());
}

void ArcCancelURLLoad(ArcURLLoadCancellationToken* token) {
  if (!token || token->_terminal || ![PendingLoads() containsObject:token] || !token->_loader) return;
  token->_terminal = YES;
  token->_loader.reset();
  [PendingLoads() removeObject:token];
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(FROM_HERE, base::BindOnce(^{
    // A delegate can cancel inside the consumer's OnDataReceived callback.
    token->_consumer.reset();
    id<ArcURLLoadDelegate> delegate = token->_delegate;
    if ([delegate respondsToSelector:@selector(didFail:withError:)])
      [delegate didFail:token->_identifier withError:net::ERR_ABORTED];
    token->_delegate = nil;
  }));
}

@implementation ArcBrowserApplication (AppURLLoading)
- (ArcURLLoadCancellationToken*)loadUrl:(ArcURLLoadConfiguration*)config withDelegate:(id<ArcURLLoadDelegate>)delegate {
  return ArcStartURLLoad(nullptr, config, delegate);
}
- (void)cancelUrlLoad:(ArcURLLoadCancellationToken*)token { ArcCancelURLLoad(token); }
@end
