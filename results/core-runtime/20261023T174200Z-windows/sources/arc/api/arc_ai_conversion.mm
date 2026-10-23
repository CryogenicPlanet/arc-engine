// Copyright 2026 ArcMod. HTML conversion delegates to Arc's external C ABI.
#import "arc/api/arc_internal.h"
#import "arc/common/renderer_values.h"
#include "arc/browser/ai_conversion_c_abi.h"
#include "arc/browser/ai_transport.h"
#include "base/memory/raw_ptr_exclusion.h"
#include <deque>
#include <unistd.h>

ARC_EXPORT @interface ArcReference : NSObject
- (instancetype)initWithReferenceText:(NSString*)text minimumCharacterCount:(NSInteger)count;
@end

namespace {
namespace abi = mojoc;
using ErrorBlock = void (^)(NSInteger);
using ConversionBlock = void (^)(ArcHtmlToMarkdownResult*, NSInteger);
struct ConversionInput {
  std::deque<std::string> strings;
  std::vector<abi::MetaTagSelector> selectors;
  abi::ContentConversionConfiguration config{};
  abi::HtmlToMarkdownRequest request{};
  abi::String String(NSString* text) {
    strings.push_back(ArcUTF8(text));
    const auto& value = strings.back();
    return {value.data(), static_cast<unsigned int>(value.size())};
  }
};
struct ConversionState {
  std::shared_ptr<arc_ai::Lifetime> lifetime = std::make_shared<arc_ai::Lifetime>();
  dispatch_queue_t queue = dispatch_get_main_queue();
  dispatch_queue_t conversion_queue = dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0);
  std::atomic<bool> available{false};
  std::atomic<bool> disconnected{false};
  RAW_PTR_EXCLUSION void* module = nullptr;
  abi::ContentConversionServiceHandle handle = nullptr;
#define CONVERSION_SYMBOL(name) decltype(&abi::ContentConversionService_##name) name = nullptr;
  CONVERSION_SYMBOL(Create)
  CONVERSION_SYMBOL(Initialize)
  CONVERSION_SYMBOL(SetUser)
  CONVERSION_SYMBOL(Crash)
  CONVERSION_SYMBOL(ConvertHtmlToMarkdown)
#undef CONVERSION_SYMBOL
  NSInteger Load(const std::string& path) {
    if (path.empty()) return abi::ContentConversionErrorInvalidLibraryPath;
    if (access(path.c_str(), F_OK)) return abi::ContentConversionErrorLibraryNotFound;
    module = arc_ai::OpenLibrary(path);
    if (!module) return abi::ContentConversionErrorLibraryMissingRequiredSymbols;
    for (const char* name : abi::Symbols) {
      if (!dlsym(module, name)) return abi::ContentConversionErrorLibraryMissingRequiredSymbols;
    }
#define CONVERSION_SYMBOL(name) name = arc_ai::Symbol<decltype(name)>(module, abi::kSymbolContentConversionService_##name);
    CONVERSION_SYMBOL(Create)
    CONVERSION_SYMBOL(Initialize)
    CONVERSION_SYMBOL(SetUser)
    CONVERSION_SYMBOL(Crash)
    CONVERSION_SYMBOL(ConvertHtmlToMarkdown)
#undef CONVERSION_SYMBOL
    handle = Create();
    return handle ? abi::ContentConversionErrorNone : abi::ContentConversionErrorIllegalState;
  }
};
NSString* Text(abi::String value) {
  return ArcNSString(value.length ? std::string_view(value.string, value.length) : std::string_view());
}
void ErrorReply(abi::ContentConversionError error, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (pending) arc_ai::Deliver(pending, ^{ ((ErrorBlock)pending->completion)(error); });
  }
}
void ConversionReply(abi::HtmlToMarkdownResultOrError* result, abi::UserData data) {
  @autoreleasepool {
  auto pending = arc_ai::Find(data);
  if (!pending) return;
  if (result->type == abi::HtmlToMarkdownResultOrError::HtmlToMarkdownResultOrErrorEnumError) {
    NSInteger error = result->value.error;
    arc_ai::Deliver(pending, ^{ ((ConversionBlock)pending->completion)(nil, error); });
    return;
  }
  // Copy every library-owned buffer before returning from its C callback.
  // Construct Arc wrappers only on the UI reply sequence.
  const auto& value = result->value.result;
  NSString* body = Text(value.body);
  NSString* title = value.title.length ? Text(value.title) : nil;
  NSMutableArray* references = [NSMutableArray arrayWithCapacity:value.referencesLength];
  for (const auto& reference : arc_ai::Buffer(value.references, value.referencesLength)) {
    [references addObject:@[Text(reference.referenceText), @(reference.minimumCharacterCount)]];
  }
  // Original C->Mojo marshaller never reads selectorsApplied (0x42857b8).
  NSArray* selectors = @[];
  NSMutableArray* entries = [NSMutableArray arrayWithCapacity:value.metaTagsLength];
  for (const auto& entry : arc_ai::Buffer(value.metaTags, value.metaTagsLength)) [entries addObject:@[Text(entry.key), Text(entry.value)]];
  const double percentage = value.selectorsContentPct;
  arc_ai::Deliver(pending, ^{
    NSMutableArray* refs = [NSMutableArray arrayWithCapacity:references.count];
    for (NSArray* reference in references) {
      [refs addObject:[[ArcReference alloc] initWithReferenceText:reference[0] minimumCharacterCount:[reference[1] integerValue]]];
    }
    NSMutableArray* tags = [NSMutableArray arrayWithCapacity:entries.count];
    for (NSArray* entry in entries) [tags addObject:[[ArcMetaTagEntry alloc] initWithKey:entry[0] value:entry[1]]];
    ArcHtmlToMarkdownResult* converted = [[ArcHtmlToMarkdownResult alloc]
        initWithBody:body title:title references:refs selectorsApplied:selectors
        selectorsContentPct:percentage metaTags:tags];
    ((ConversionBlock)pending->completion)(converted, -1);
  });
  }
}
template <typename Invoke>
void Send(std::shared_ptr<ConversionState> state, std::shared_ptr<ConversionInput> input,
          id completion, Invoke invoke, bool convert = false) {
  auto pending = arc_ai::Request(completion, nil, state->lifetime);
  auto data = arc_ai::Register<abi::UserData>(pending);
  dispatch_async(state->queue, ^{
    // Utility control messages arrive serially before conversion is posted to
    // workers. Publish initialization/handle writes on main in that order.
    if (convert) {
      dispatch_async(state->conversion_queue, ^{ invoke(state, input, data); (void)pending; });
    } else {
      invoke(state, input, data);
      (void)pending;
    }
  });
}
}  // namespace

ARC_EXPORT @interface ArcContentConversionService : NSObject
@end
@implementation ArcContentConversionService {
  std::shared_ptr<ConversionState> _state;
}
- (instancetype)init {
  if ((self = [super init])) _state = std::make_shared<ConversionState>();
  return self;
}
- (void)dealloc {
  if (_state) arc_ai::Invalidate(_state->lifetime);
}
- (BOOL)isAvailable { return _state && _state->available.load(); }
- (void)initializeWithLibraryPath:(NSURL*)path displayName:(NSString*)name libraryVersion:(NSString*)version
                  appEnvironment:(NSString*)environment fullAppVersion:(NSString*)appVersion callback:(ErrorBlock)callback {
  if (!_state) _state = std::make_shared<ConversionState>();
  if (_state->available.exchange(true)) { callback(4); return; }
  auto input = std::make_shared<ConversionInput>();
  input->config = {input->String(name), input->String(path.path), input->String(version), input->String(environment), input->String(appVersion)};
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    const auto& path = input->config.libraryPath;
    NSInteger error = state->Load(std::string(path.string, path.length));
    if (error != -1) { ErrorReply(static_cast<abi::ContentConversionError>(error), data); return; }
    state->Initialize(state->handle, &input->config, ErrorReply, data);
  });
}
- (void)setUser:(NSString*)user email:(NSString*)email callback:(ErrorBlock)callback {
  if (!_state || !_state->available.load()) {
    callback(_state && _state->disconnected.load() ? -1 : 3);
    return;
  }
  auto input = std::make_shared<ConversionInput>();
  input->config.displayName = input->String(user);
  input->config.libraryPath = input->String(email);
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { ErrorReply(abi::ContentConversionErrorNotInitialized, data); return; }
    state->SetUser(state->handle, input->config.displayName, input->config.libraryPath, ErrorReply, data);
  });
}
- (void)convertHtmlToMarkdown:(NSString*)html url:(NSString*)url linkStyle:(NSInteger)linkStyle
                    strategy:(NSInteger)strategy urlTransform:(NSInteger)urlTransform skipNoscript:(BOOL)skipNoscript
    passThroughInputElements:(BOOL)passThrough metaTagsToCapture:(NSArray*)metaTags
   deduplicateMetaTagContent:(BOOL)deduplicate completion:(ConversionBlock)callback {
  if (!_state || !_state->available.load()) { callback(nil, 3); return; }
  auto input = std::make_shared<ConversionInput>();
  for (ArcMetaTagSelector* selector in metaTags) {
    input->selectors.push_back({static_cast<abi::MetaTagSelectorKind>(selector.kind), input->String(selector.value)});
  }
  input->request = {input->String(html), input->String(url), static_cast<abi::LinkStyle>(linkStyle),
                    static_cast<abi::ConversionStrategy>(strategy), static_cast<abi::UrlTransformKind>(urlTransform),
                    static_cast<bool>(skipNoscript), static_cast<bool>(passThrough), input->selectors.data(),
                    static_cast<unsigned int>(input->selectors.size()), static_cast<bool>(deduplicate)};
  Send(_state, input, callback, [](auto state, auto input, auto data) {
    if (!state->handle) { abi::HtmlToMarkdownResultOrError result{};
      result.type = abi::HtmlToMarkdownResultOrError::HtmlToMarkdownResultOrErrorEnumError;
      result.value.error = abi::ContentConversionErrorNotInitialized; ConversionReply(&result, data); return; }
    state->ConvertHtmlToMarkdown(state->handle, &input->request, ConversionReply, data);
    // Arc's conversion shim expects its C result synchronously (0x4284d8c).
    // A deferred callback leaves the Mojo response null, disconnects the
    // service, and completes with ConversionFailed. Our token lookup drops
    // any subsequent callback without accessing retired output storage.
    if (auto unfinished = arc_ai::Find(data)) {
      state->disconnected.store(true);
      state->available.store(false);
      arc_ai::Deliver(unfinished, ^{
        ((ConversionBlock)unfinished->completion)(nil, abi::ContentConversionErrorConversionFailed);
      });
    }
  }, true);
}
@end
