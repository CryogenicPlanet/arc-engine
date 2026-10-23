// Copyright 2026 ArcMod. Arc's autocomplete boundary on Chromium's omnibox.
#import "arc/api/arc_internal.h"
#import <objc/runtime.h>

#include <memory>
#include <map>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "chrome/browser/autocomplete/chrome_autocomplete_provider_client.h"
#include "chrome/browser/autocomplete/chrome_autocomplete_scheme_classifier.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_observer.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "components/omnibox/browser/autocomplete_classifier.h"
#include "components/omnibox/browser/autocomplete_controller.h"
#include "components/omnibox/browser/autocomplete_controller_config.h"
#include "components/omnibox/browser/autocomplete_input.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_match_type.h"
#include "components/omnibox/browser/autocomplete_provider.h"
#include "components/omnibox/browser/autocomplete_result.h"
#include "components/omnibox/browser/keyword_provider.h"
#include "components/search_engines/template_url_service.h"
#include "ui/base/page_transition_types.h"

// These native inputs belong to this implementation; Arc's C++ accessors are
// internal ABI, not app-facing selectors (ARCHITECTURE.md).
using ArcOmniboxStringPair = std::pair<std::string, std::string>;
namespace { class Controller; }

ARC_EXPORT @interface ArcAutocompleteMatchAdditionalData : NSObject
- (instancetype)init:(NSString*)key value:(NSString*)value;
- (instancetype)initWithAutocompleteMatchAdditionalData:(const void*)data;
@property(nonatomic, readonly, copy) NSString* key;
@property(nonatomic, readonly, copy) NSString* value;
@end

@implementation ArcAutocompleteMatchAdditionalData {
  ArcOmniboxStringPair _data;
}
- (instancetype)init:(NSString*)key value:(NSString*)value {
  if ((self = [super init])) {
    _data = {ArcUTF8(key), ArcUTF8(value)};
  }
  return self;
}
- (instancetype)initWithAutocompleteMatchAdditionalData:(const void*)data {
  if ((self = [super init]) && data) {
    _data = *static_cast<const ArcOmniboxStringPair*>(data);
  }
  return self;
}
- (void*)mutableAutocompleteMatchAdditionalData { return &_data; }
- (NSString*)key { return ArcNSString(_data.first); }
- (NSString*)value { return ArcNSString(_data.second); }
@end

ARC_EXPORT @interface ArcPasteAndGoClassification : NSObject
- (instancetype)initWithPasteAndGoClassification:(const void*)data;
@property(nonatomic, readonly, copy) NSString* menuTitle;
@property(nonatomic, readonly) NSURL* destinationURL;
@end

@implementation ArcPasteAndGoClassification {
  ArcOmniboxStringPair _data;
}
- (instancetype)initWithPasteAndGoClassification:(const void*)data {
  if ((self = [super init]) && data) {
    _data = *static_cast<const ArcOmniboxStringPair*>(data);
  }
  return self;
}
- (NSString*)menuTitle { return ArcNSString(_data.first); }
- (NSURL*)destinationURL { return ArcNSURL(GURL(_data.second)); }
@end

ARC_EXPORT @interface ArcAutocompleteResult : NSObject
- (instancetype)init:(NSArray*)matches perProviderMatches:(NSDictionary*)perProvider;
@property(nonatomic, readonly) NSArray* combinedMatches;
@property(nonatomic, readonly) NSDictionary* perProviderMatches;
@end

@implementation ArcAutocompleteResult
@synthesize combinedMatches = _combinedMatches;
@synthesize perProviderMatches = _perProviderMatches;
- (instancetype)init:(NSArray*)matches perProviderMatches:(NSDictionary*)perProvider {
  if ((self = [super init])) {
    _combinedMatches = matches;
    _perProviderMatches = perProvider;
  }
  return self;
}
@end

ARC_EXPORT @interface ArcAutocompleteMatch : NSObject
- (instancetype)initWithAutocompleteMatch:(const void*)match;
- (void)setSource:(base::WeakPtr<Controller>)source token:(uint64_t)token;
- (base::WeakPtr<Controller>)source;
- (uint64_t)token __attribute__((objc_direct));
- (NSArray*)snapshotDuplicates __attribute__((objc_direct));
@property(nonatomic, readonly) NSURL* destinationUrl;
@property(nonatomic, readonly, copy) NSString* destinationUrlSpec;
@property(nonatomic, readonly) NSURL* imageUrl;
@property(nonatomic, readonly, copy) NSString* imageUrlSpec;
@property(nonatomic, readonly) int relevance;
@property(nonatomic, readonly, copy) NSString* contents;
@property(nonatomic, readonly, copy) NSString* matchType;
@property(nonatomic, readonly, copy) NSString* providerType;
@property(nonatomic, readonly, copy) NSString* inlineAutocompleteText;
@property(nonatomic, readonly, copy) NSString* description;
@property(nonatomic, readonly) NSArray* additionalData;
@property(nonatomic, readonly) BOOL hasTabMatch;
@property(nonatomic, readonly) BOOL allowedToBeDefaultMatch;
@property(nonatomic, readonly) BOOL deletable;
@property(nonatomic, readonly) BOOL swapContentsAndDescription;
@property(nonatomic, readonly) BOOL isFromPrevious;
@property(nonatomic, readonly) BOOL isFromKeyword;
@property(nonatomic, readonly, copy) NSString* keyword;
@property(nonatomic, readonly, copy) NSString* fillIntoEdit;
@property(nonatomic, readonly, copy) NSString* fillIntoEditAdditionalText;
@property(nonatomic, readonly, copy) NSString* transition;
@property(nonatomic, readonly) NSArray* duplicates;
@property(nonatomic, readonly) ArcAutocompleteMatch* associatedKeyword;
@end

@implementation ArcAutocompleteMatch {
  AutocompleteMatch _match;
  NSString* _providerType;
  NSArray* _duplicates;
  ArcAutocompleteMatch* _associatedKeyword;
  base::WeakPtr<Controller> _source;
  uint64_t _token;
}
@synthesize duplicates = _duplicates;
@synthesize associatedKeyword = _associatedKeyword;
- (instancetype)initWithAutocompleteMatch:(const void*)match {
  if ((self = [super init])) {
    _providerType = @"";
    if (match) {
      const auto& native = *static_cast<const AutocompleteMatch*>(match);
      _providerType = native.provider ? ArcNSString(AutocompleteProvider::TypeToString(native.provider->type())) : @"";
      _match = native;
      // Providers reference the controller's client and profile. Copy values,
      // never retain providers or leave their pointers in app-held snapshots.
      _match.provider = nullptr;
      _match.actions.clear();
      _match.takeover_action.reset();
      _match.duplicate_matches.clear();
      NSMutableArray* duplicates = [NSMutableArray arrayWithCapacity:native.duplicate_matches.size()];
      for (const auto& duplicate : native.duplicate_matches) {
        [duplicates addObject:[[ArcAutocompleteMatch alloc] initWithAutocompleteMatch:&duplicate]];
      }
      _duplicates = [duplicates copy];
    }
  }
  return self;
}
- (void)setSource:(base::WeakPtr<Controller>)source token:(uint64_t)token {
  _source = source;
  _token = token;
}
- (base::WeakPtr<Controller>)source { return _source; }
- (uint64_t)token { return _token; }
- (NSArray*)snapshotDuplicates { return _duplicates; }
- (NSURL*)destinationUrl { return ArcNSURL(_match.destination_url); }
- (NSString*)destinationUrlSpec { return ArcNSString(_match.destination_url.spec()); }
- (NSURL*)imageUrl { return ArcNSURL(_match.image_url); }
- (NSString*)imageUrlSpec { return ArcNSString(_match.image_url.spec()); }
- (int)relevance { return _match.relevance; }
- (NSString*)contents { return ArcNSString(_match.contents); }
- (NSString*)matchType { return ArcNSString(omnibox::AutocompleteMatchTypeToString(_match.type)); }
- (NSString*)providerType {
  return _providerType;
}
- (NSString*)inlineAutocompleteText { return ArcNSString(_match.inline_autocompletion); }
- (NSString*)description { return ArcNSString(_match.description); }
- (NSArray*)additionalData {
  NSMutableArray* values = [NSMutableArray arrayWithCapacity:_match.additional_info.size()];
  for (const auto& [key, value] : _match.additional_info) {
    [values addObject:[[ArcAutocompleteMatchAdditionalData alloc] init:ArcNSString(key) value:ArcNSString(value)]];
  }
  return values;
}
- (BOOL)hasTabMatch { return _match.has_tab_match.value_or(false); }
- (BOOL)allowedToBeDefaultMatch { return _match.allowed_to_be_default_match; }
- (BOOL)deletable { return _match.deletable; }
- (BOOL)swapContentsAndDescription { return _match.swap_contents_and_description; }
- (BOOL)isFromPrevious { return _match.from_previous; }
- (BOOL)isFromKeyword { return _match.from_keyword; }
- (NSString*)keyword { return ArcNSString(_match.keyword); }
- (NSString*)fillIntoEdit { return ArcNSString(_match.fill_into_edit); }
- (NSString*)fillIntoEditAdditionalText { return ArcNSString(_match.additional_text); }
- (NSString*)transition { return ArcNSString(ui::PageTransitionGetCoreTransitionString(_match.transition)); }
- (NSArray*)duplicates { return _duplicates ?: @[]; }
- (ArcAutocompleteMatch*)associatedKeyword {
  if (!_associatedKeyword && !_match.associated_keyword.empty()) {
    AutocompleteMatch match;
    match.keyword = _match.associated_keyword;
    _associatedKeyword = [[ArcAutocompleteMatch alloc] initWithAutocompleteMatch:&match];
  }
  return _associatedKeyword;
}
@end

namespace {
using ContextCompletion = void (^)(NSArray*, BOOL);
using BrowserCompletion = void (^)(ArcAutocompleteResult*, BOOL);

// Both wrapper lifetimes and profile shutdown cancel the native controller.
// A temporary shared owner protects against reentrant release in app blocks.
class Controller : public AutocompleteController::Observer,
                   public ProfileObserver,
                   public std::enable_shared_from_this<Controller> {
 public:
  Controller(Profile* profile, int providers) : profile_(profile->GetWeakPtr()) {
    profile->AddObserver(this);
    native_ = std::make_unique<AutocompleteController>(
        std::make_unique<ChromeAutocompleteProviderClient>(profile),
        AutocompleteControllerConfig{.provider_types = providers});
    native_->AddObserver(this);
  }
  ~Controller() override {
    if (profile_) {
      profile_->RemoveObserver(this);
    }
    Destroy();
  }
  void OnProfileWillBeDestroyed(Profile* profile) override {
    profile->RemoveObserver(this);
    profile_.reset();
    Destroy();
  }
  void Destroy() {
    context_completion_ = nil;
    browser_completion_ = nil;
    if (native_) {
      native_->RemoveObserver(this);
      matches_.clear();
      native_.reset();
    }
  }
  void Stop() {
    const uint64_t generation = generation_;
    if (native_) {
      native_->Stop(AutocompleteStopReason::kClobbered);
    }
    if (generation_ == generation) {
      context_completion_ = nil;
      browser_completion_ = nil;
    }
  }
  void Start(NSString* text, NSInteger cursor, BOOL prevent_inline,
             bool keyword, bool is_browser, ContextCompletion context, BrowserCompletion browser) {
    ++generation_;
    std::erase_if(matches_, [](const auto& entry) { return !entry.second.wrapper; });
    context_completion_ = [context copy];
    browser_completion_ = [browser copy];
    if (!profile_ || !native_) {
      return;
    }
    AutocompleteInput input(ArcUTF16(text), static_cast<size_t>(cursor),
                            metrics::OmniboxEventProto::OTHER,
                            ChromeAutocompleteSchemeClassifier(profile_.get()));
    input.set_prevent_inline_autocomplete(prevent_inline);
    if (is_browser) {
      input.set_allow_exact_keyword_match(keyword);
    }
    input.set_in_keyword_mode(keyword);
    native_->Start(input);
  }
  void Delete(ArcAutocompleteMatch* match) {
    if (!native_ || !match) {
      return;
    }
    if ([match source].get() != this) {
      return;
    }
    auto found = matches_.find([match token]);
    if (found != matches_.end() && found->second.native.SupportsDeletion()) {
      AutocompleteMatch value = found->second.native;
      native_->Stop(AutocompleteStopReason::kInteraction);
      native_->DeleteMatch(value);
    }
  }
  void RegisterMatch(ArcAutocompleteMatch* wrapper, const AutocompleteMatch& match) {
    if (match.SupportsDeletion()) {
      const uint64_t token = ++next_match_;
      matches_.emplace(token, MatchRecord{wrapper, match});
      [wrapper setSource:weak_factory_.GetWeakPtr() token:token];
    }
    NSArray* duplicates = [wrapper snapshotDuplicates];
    for (size_t i = 0; i < match.duplicate_matches.size(); ++i) {
      RegisterMatch(duplicates[i], match.duplicate_matches[i]);
    }
  }
  NSArray* WrapMatches(const ACMatches& matches) {
    NSMutableArray* values = [NSMutableArray arrayWithCapacity:matches.size()];
    for (const auto& match : matches) {
      ArcAutocompleteMatch* wrapper = [[ArcAutocompleteMatch alloc] initWithAutocompleteMatch:&match];
      RegisterMatch(wrapper, match);
      [values addObject:wrapper];
    }
    return [values copy];
  }
  void OnResultChanged(AutocompleteController* controller, bool) override {
    // NotifyChanged continues using the native controller after observers
    // return. Keep this owner until the next UI task if the app releases its
    // last wrapper synchronously from the completion block.
    auto keep_alive = shared_from_this();
    auto context = context_completion_;
    auto browser = browser_completion_;
    const BOOL done = controller->done();
    ACMatches matches(controller->result().begin(), controller->result().end());
    NSArray* combined = WrapMatches(matches);
    if (context) {
      context(combined, done);
    } else if (browser) {
      NSMutableDictionary* per_provider = [NSMutableDictionary dictionary];
      for (const auto& provider : controller->providers()) {
        if (provider->matches().empty()) {
          continue;
        }
        per_provider[ArcNSString(AutocompleteProvider::TypeToString(provider->type()))] =
            WrapMatches(provider->matches());
      }
      browser([[ArcAutocompleteResult alloc] init:combined perProviderMatches:[per_provider copy]], done);
    }
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce([](std::shared_ptr<Controller>) {}, std::move(keep_alive)));
  }
 private:
  base::WeakPtr<Profile> profile_;
  std::unique_ptr<AutocompleteController> native_;
  struct MatchRecord {
    ArcAutocompleteMatch* __weak wrapper;
    AutocompleteMatch native;
  };
  std::map<uint64_t, MatchRecord> matches_;
  uint64_t next_match_ = 0;
  uint64_t generation_ = 0;
  ContextCompletion __strong context_completion_;
  BrowserCompletion __strong browser_completion_;
  base::WeakPtrFactory<Controller> weak_factory_{this};
};
}

ARC_EXPORT @interface ArcBrowserContextAutocomplete : NSObject
- (instancetype)initWithProfile:(Profile*)profile providers:(NSUInteger)providers;
- (void)autocomplete:(NSString*)text cursorPosition:(NSInteger)cursor
    preventInlineAutocomplete:(BOOL)preventInline completion:(ContextCompletion)completion;
- (void)stop;
@end

@implementation ArcBrowserContextAutocomplete {
  std::shared_ptr<Controller> _controller;
}
- (instancetype)initWithProfile:(Profile*)profile providers:(NSUInteger)providers {
  if ((self = [super init]) && profile) {
    // Arc's two bits mean history (quick, URL, fuzzy) and bookmarks.
    const int types = (providers & 1 ? 0x1000c : 0) | (providers & 2 ? 1 : 0);
    _controller = std::make_shared<Controller>(profile, types);
  }
  return self;
}
- (void)autocomplete:(NSString*)text cursorPosition:(NSInteger)cursor
    preventInlineAutocomplete:(BOOL)preventInline completion:(ContextCompletion)completion {
  auto controller = _controller;
  if (controller) {
    controller->Start(text, static_cast<int>(cursor), preventInline, false, false, completion, nil);
  }
}
- (void)stop {
  auto controller = _controller;
  if (controller) {
    controller->Stop();
  }
}
@end

@implementation ArcBrowserContext (Omnibox)
- (ArcBrowserContextAutocomplete*)createAutocompleteControllerWithProviders:(NSUInteger)providers {
  return [[ArcBrowserContextAutocomplete alloc] initWithProfile:[self profile] providers:providers & 3];
}
@end

@interface OmniboxBrowserState : NSObject {
 @public
  std::shared_ptr<Controller> controller;
}
@end
@implementation OmniboxBrowserState
@end

namespace {
const char kBrowserAutocompleteKey = 0;
void EmptyBrowserCompletion(BrowserCompletion completion) {
  if (completion) {
    completion([[ArcAutocompleteResult alloc] init:@[] perProviderMatches:@{}], YES);
  }
}
}

@implementation ArcBrowser (Omnibox)
- (BOOL)autocomplete:(NSString*)text cursorPosition:(int)cursor
    preventInlineAutocomplete:(BOOL)preventInline startFresh:(BOOL)startFresh
    keyword:(NSString*)keyword error:(NSError**)error completion:(BrowserCompletion)completion {
  BrowserWindowInterface* browser = [self browser];
  if (!browser) {
    if (error) {
      *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:6
                              userInfo:@{NSLocalizedDescriptionKey: @"browser deinitialized"}];
    }
    return NO;
  }
  Profile* profile = browser->GetProfile();
  if (keyword.length) {
    auto* service = TemplateURLServiceFactory::GetForProfile(profile);
    if (!service || !service->GetTemplateURLForKeyword(ArcUTF16(keyword))) {
      EmptyBrowserCompletion(completion);
      if (error) {
        *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:2
                                userInfo:@{NSLocalizedDescriptionKey: @"unknown keyword"}];
      }
      return NO;
    }
  }
  const std::string input = ArcUTF8(text);
  if (input.find_first_not_of(' ') == std::string::npos) {
    EmptyBrowserCompletion(completion);
    return YES;
  }
  OmniboxBrowserState* state = objc_getAssociatedObject(self, &kBrowserAutocompleteKey);
  if (!state) {
    state = [[OmniboxBrowserState alloc] init];
    objc_setAssociatedObject(self, &kBrowserAutocompleteKey, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  if (startFresh || !state->controller) {
    state->controller = std::make_shared<Controller>(profile, AutocompleteClassifier::DefaultOmniboxProviders());
  }
  auto controller = state->controller;
  NSString* query = keyword.length ? [NSString stringWithFormat:@"%@ %@", keyword, text ?: @""] : text;
  controller->Start(query, cursor, preventInline, keyword.length > 0, true, nil, completion);
  return YES;
}
- (void)deleteAutocompleteMatch:(ArcAutocompleteMatch*)match {
  OmniboxBrowserState* state = objc_getAssociatedObject(self, &kBrowserAutocompleteKey);
  if (state && state->controller) {
    auto controller = state->controller;
    controller->Delete(match);
  }
}
@end
