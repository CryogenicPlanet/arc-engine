// Copyright 2026 ArcMod. Extension groups are owned by Arc's app.
#import "arc/browser/extensions_bridge.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "base/functional/bind.h"
#include "base/memory/scoped_refptr.h"
#include "base/strings/pattern.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/api/tabs/windows_util.h"
#include "chrome/browser/extensions/browser_extension_window_controller.h"
#include "chrome/browser/extensions/chrome_extension_function_details.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/extensions/window_controller.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/browser_window/public/global_browser_collection.h"
#include "chrome/browser/ui/web_applications/app_browser_controller.h"
#include "chrome/common/extensions/api/tab_groups.h"
#include "chrome/common/extensions/api/tabs.h"
#include "chrome/common/extensions/extension_constants.h"
#include "components/sessions/content/session_tab_helper.h"
#include "components/tab_groups/tab_group_color.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_function.h"
#include "extensions/browser/extension_function_registry.h"

// These values' primary implementations belong to the core package.
@interface ArcTabGroupInfo : NSObject
@property(nonatomic, copy) NSString* groupId;
@property(nonatomic) NSInteger extensionGroupId;
@property(nonatomic, copy) NSString* title;
@property(nonatomic) NSInteger color;
@property(nonatomic) BOOL collapsed;
@end
@interface ArcTabGroupProperties : NSObject
@property(nonatomic, copy) NSString* title;
@property(nonatomic, strong) NSNumber* color;
@property(nonatomic, strong) NSNumber* collapsed;
@end
@interface ArcTabGroupResult : NSObject
@property(nonatomic) BOOL success;
@property(nonatomic, copy) NSString* error;
@property(nonatomic, copy) NSString* groupId;
@property(nonatomic) NSInteger extensionGroupId;
@end

@protocol ArcHostedGroupDelegate <NSObject>
- (void)browser:(ArcBrowser*)browser
    createTabGroupWithSessionIds:(NSArray*)ids
                      properties:(ArcTabGroupProperties*)properties
                      completion:(void (^)(ArcTabGroupResult*))completion;
- (void)browser:(ArcBrowser*)browser
    addTabsToGroupWithId:(NSString*)groupId
              sessionIds:(NSArray*)ids
              completion:(void (^)(ArcTabGroupResult*))completion;
- (void)browser:(ArcBrowser*)browser
    removeTabsFromGroupWithSessionIds:(NSArray*)ids
                           completion:(void (^)(ArcTabGroupResult*))completion;
- (void)browser:(ArcBrowser*)browser
    updateTabGroupWithId:(NSString*)groupId
              properties:(ArcTabGroupProperties*)properties
              completion:(void (^)(ArcTabGroupResult*))completion;
- (void)browser:(ArcBrowser*)browser
    moveTabGroupWithId:(NSString*)groupId
            toNewIndex:(int64_t)index
            completion:(void (^)(ArcTabGroupResult*))completion;
- (void)browser:(ArcBrowser*)browser
    getTabGroupWithId:(NSString*)groupId
           completion:(void (^)(ArcTabGroupInfo*))completion;
- (void)browser:(ArcBrowser*)browser
    getAllTabGroupsWithCompletion:(void (^)(NSArray*))completion;
@end

namespace arc {
namespace {
namespace api = extensions::api;
using extensions::ChromeExtensionFunctionDetails;
using extensions::ExtensionTabUtil;

constexpr const char* kUnavailable =
    ExtensionTabUtil::kTabStripDoesNotSupportTabGroupsError;
constexpr char kClosed[] = "The browser was closed.";

// Always defer even a synchronous app completion: Run() must first return
// RespondLater(). This also confines duplicate-completion guards to the UI
// thread.
void OnUI(void (^block)()) {
  content::GetUIThreadTaskRunner({})->PostTask(
      FROM_HERE, base::BindOnce([](void (^work)()) { work(); }, block));
}

struct GroupInfo {
  __strong NSString* group_id;
  int extension_group_id;
  std::string title;
  NSInteger color;
  bool collapsed;
};
using GroupInfos = std::vector<std::shared_ptr<const GroupInfo>>;

std::shared_ptr<const GroupInfos> SnapshotGroups(NSArray* values) {
  auto snapshots = std::make_shared<GroupInfos>();
  for (id value in values) {
    if (![value isKindOfClass:NSClassFromString(@"ArcTabGroupInfo")])
      continue;
    ArcTabGroupInfo* info = value;
    snapshots->push_back(std::make_shared<const GroupInfo>(GroupInfo{
        [info.groupId copy], static_cast<int>(info.extensionGroupId),
        base::SysNSStringToUTF8(info.title ?: @""), info.color,
        static_cast<bool>(info.collapsed)}));
  }
  return snapshots;
}

struct GroupRecord {
  __strong ArcBrowser* browser;
  std::shared_ptr<const GroupInfo> info;
  int window_id;
};
using Groups = std::vector<GroupRecord>;
using GroupsCompletion = void (^)(std::shared_ptr<Groups>, NSString*);
using GroupCompletion =
    void (^)(ArcBrowser*, std::shared_ptr<const GroupInfo>, int window_id,
             NSString*);
enum class ReadbackKind { kUpdate, kMove };

class HostedGroupsFunction : public ExtensionFunction {
 public:
  void Begin() {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    profile_ = Profile::FromBrowserContext(browser_context())->GetWeakPtr();
    // The app may discard a group completion without invoking it. RespondLater
    // does not retain the function: keep the real request pending until its
    // response or context shutdown, rather than destroying an unanswered Mojo
    // request (or manufacturing a successful group result).
    pending_ = this;
  }

  bool Allowed(BrowserWindowInterface* browser) const {
    if (!profile_ || !browser || !BrowserForHostedExtensions(browser) ||
        !profile_->IsSameOrParent(browser->GetProfile()) ||
        (!include_incognito_information() &&
         profile_.get() != browser->GetProfile())) {
      return false;
    }
    auto* controller =
        extensions::BrowserExtensionWindowController::From(browser);
    return controller && controller->IsVisibleToTabsAPIForExtension(
                             extension(), /*allow_dev_tools_windows=*/false);
  }

  BrowserWindowInterface* Live(ArcBrowser* browser) const {
    BrowserWindowInterface* native = [browser browser];
    return Allowed(native) ? native : nullptr;
  }

  ArcBrowser* Window(int window_id, std::string* error) {
    auto* controller = ExtensionTabUtil::GetControllerFromWindowID(
        ChromeExtensionFunctionDetails(this), window_id, error);
    auto* browser =
        controller ? controller->GetBrowserWindowInterface() : nullptr;
    if (!Allowed(browser) || !ExtensionTabUtil::SupportsTabGroups(browser)) {
      if (error->empty())
        *error = kUnavailable;
      return nil;
    }
    return BrowserForHostedExtensions(browser);
  }

  void Fail(NSString* error) {
    if (!did_respond()) {
      Respond(Error(
          base::SysNSStringToUTF8(error ?: @"Tab group operation failed.")));
    }
  }
  void FailString(const std::string& error) {
    Fail(base::SysUTF8ToNSString(error));
  }
  void Success(base::ListValue result) {
    if (did_respond())
      return;
    if (!profile_) {
      Fail(@"The browser context was closed.");
      return;
    }
    Respond(ArgumentList(std::move(result)));
  }

  void Request(ArcBrowser* browser, void (^request)(id delegate),
               void (^missing)() = nil) {
    if (!DispatchBrowserExtensionRequest(browser, request)) {
      // Native preflight failure is synchronous. A later nil clear is handled
      // inside the queued weak task and never reaches this fallback.
      if (missing)
        missing();
      else
        Fail(@"No browser delegate");
    }
  }

  void Gather(GroupsCompletion completion,
              std::optional<int> window_filter = std::nullopt,
              bool keep_inflight_results = false) {
    struct Pending {
      size_t remaining = 0;
      bool completed = false;
      std::shared_ptr<Groups> groups = std::make_shared<Groups>();
      std::vector<Groups> by_browser;
    };
    auto state = std::make_shared<Pending>();
    std::vector<base::WeakPtr<BrowserWindowInterface>> browsers;
    GlobalBrowserCollection::GetInstance()->ForEach(
        [&](BrowserWindowInterface* browser) {
          if (Allowed(browser) &&
              ExtensionTabUtil::SupportsTabGroups(browser) &&
              (!window_filter ||
               *window_filter == ExtensionTabUtil::GetWindowId(browser)))
            browsers.push_back(browser->GetWeakPtr());
          return true;
        },
        BrowserCollection::Order::kActivation);
    state->remaining = browsers.size();
    state->by_browser.resize(browsers.size());
    scoped_refptr<HostedGroupsFunction> self(this);
    if (browsers.empty()) {
      OnUI(^{
        completion(state->groups, nil);
      });
      return;
    }
    for (size_t browser_index = 0; browser_index < browsers.size();
         ++browser_index) {
      const auto& weak = browsers[browser_index];
      ArcBrowser* wrapper =
          weak ? BrowserForHostedExtensions(weak.get()) : nil;
      const int window_id = weak ? ExtensionTabUtil::GetWindowId(weak.get()) : 0;
      auto called = std::make_shared<bool>(false);
      void (^complete)(std::shared_ptr<const GroupInfos>, bool) =
          ^(std::shared_ptr<const GroupInfos> snapshots, bool app_reply) {
        if (*called || state->completed)
          return;
        *called = true;
        if (app_reply && !keep_inflight_results && !self->Live(wrapper)) {
          state->completed = true;
          completion(nullptr, base::SysUTF8ToNSString(kClosed));
          return;
        }
        for (const auto& info : *snapshots)
          state->by_browser[browser_index].push_back({wrapper, info, window_id});
        if (--state->remaining == 0) {
          state->completed = true;
          for (const auto& batch : state->by_browser)
            state->groups->insert(state->groups->end(), batch.begin(), batch.end());
          completion(state->groups, nil);
        }
      };
      // GetAll supplies an empty vector when the target is absent before
      // enqueue; actual app completions keep the native separate reply task.
      Request(wrapper, ^(id target) {
        id<ArcHostedGroupDelegate> delegate = target;
        if (!self->Live(wrapper) ||
            ![delegate respondsToSelector:
                @selector(browser:getAllTabGroupsWithCompletion:)]) {
          if (!state->completed) {
            state->completed = true;
            completion(nullptr, base::SysUTF8ToNSString(kUnavailable));
          }
          return;
        }
        [delegate browser:wrapper
          getAllTabGroupsWithCompletion:^(NSArray* infos) {
            auto snapshots = SnapshotGroups(infos);
            OnUI(^{ complete(snapshots, /*app_reply=*/true); });
          }];
      }, ^{
        complete(std::make_shared<const GroupInfos>(), /*app_reply=*/false);
      });
    }
  }

  void Find(int group_id, GroupCompletion completion) {
    Gather(^(std::shared_ptr<Groups> groups, NSString* error) {
      if (error) {
        completion(nil, nullptr, 0, error);
        return;
      }
      for (const auto& group : *groups) {
        if (group.info->extension_group_id == group_id &&
            group.info->group_id.length) {
          completion(group.browser, group.info, group.window_id, nil);
          return;
        }
      }
      completion(nil, nullptr, 0,
                 base::SysUTF8ToNSString("No group with id: " +
                                         base::NumberToString(group_id) + "."));
    });
  }

  base::DictValue Serialize(int window_id, const GroupInfo& info) {
    base::DictValue value;
    value.Set("id", info.extension_group_id);
    value.Set("windowId", window_id);
    value.Set("title", info.title);
    const NSInteger color_value = info.color;
    auto color = color_value >= 0 && color_value < 9
                     ? static_cast<tab_groups::TabGroupColorId>(color_value)
                     : tab_groups::TabGroupColorId::kGrey;
    value.Set("color", api::tab_groups::ToString(
                           ExtensionTabUtil::ColorIdToColor(color)));
    value.Set("collapsed", info.collapsed);
    value.Set("shared", false);
    return value;
  }

  void RespondGroupInfo(int window_id, const GroupInfo& info) {
    base::ListValue result;
    result.Append(Serialize(window_id, info));
    Success(std::move(result));
  }

  void FinishReadback(ReadbackKind kind, int group_id, int window_id,
                      std::shared_ptr<const GroupInfo> info) {
    if (info) {
      RespondGroupInfo(window_id, *info);
    } else if (kind == ReadbackKind::kMove) {
      Success(base::ListValue());
    } else {
      // Native update emits this generated default object on a missing group
      // or an expired source browser. kNone serializes to the empty string.
      api::tab_groups::TabGroup fallback;
      fallback.id = group_id;
      fallback.window_id = window_id;
      Success(base::ListValue().Append(fallback.ToValue()));
    }
  }

  void ReadBack(ArcBrowser* source, int group_id, int window_id,
                ReadbackKind kind) {
    if (!Live(source)) {
      FinishReadback(kind, group_id, window_id, nullptr);
      return;
    }
    scoped_refptr<HostedGroupsFunction> self(this);
    auto called = std::make_shared<bool>(false);
    if (!DispatchBrowserExtensionRequest(source, ^(id target) {
          id<ArcHostedGroupDelegate> delegate = target;
          if (![delegate respondsToSelector:
                  @selector(browser:getAllTabGroupsWithCompletion:)]) {
            self->FailString(kUnavailable);
            return;
          }
          [delegate browser:source
              getAllTabGroupsWithCompletion:^(NSArray* infos) {
                auto snapshots = SnapshotGroups(infos);
                OnUI(^{
                  if (*called || self->did_respond())
                    return;
                  *called = true;
                  // Native reply uses the captured window ID. It does not
                  // dereference the source browser again after this lookup.
                  for (const auto& info : *snapshots) {
                    if (info->extension_group_id == group_id) {
                      self->FinishReadback(kind, group_id, window_id, info);
                      return;
                    }
                  }
                  self->FinishReadback(kind, group_id, window_id, nullptr);
                });
              }];
        })) {
      FinishReadback(kind, group_id, window_id, nullptr);
    }
  }

  void (^ReadbackCompletion(ArcBrowser* source, int group_id, int window_id,
                            ReadbackKind kind))(ArcTabGroupResult*) {
    scoped_refptr<HostedGroupsFunction> self(this);
    auto called = std::make_shared<bool>(false);
    return ^(ArcTabGroupResult* result) {
      const bool succeeded = result && result.success;
      NSString* error = [result.error copy];
      OnUI(^{
        if (*called || self->did_respond())
          return;
        *called = true;
        if (!succeeded) {
          self->Fail(error.length ? error : kind == ReadbackKind::kUpdate
              ? @"Failed to update tab group" : @"Failed to move tab group");
          return;
        }
        // Both native paths read the original numeric ID from the original
        // source browser, including requests without a JavaScript callback.
        self->ReadBack(source, group_id, window_id, kind);
      });
    };
  }

  void (^GroupMutationCompletion(ArcBrowser* browser))(ArcTabGroupResult*) {
    scoped_refptr<HostedGroupsFunction> self(this);
    auto called = std::make_shared<bool>(false);
    return ^(ArcTabGroupResult* result) {
      const bool succeeded = result && result.success;
      NSString* error = [result.error copy];
      const int extension_group_id = static_cast<int>(result.extensionGroupId);
      OnUI(^{
        if (*called)
          return;
        *called = true;
        if (!self->Live(browser)) {
          self->FailString(kClosed);
          return;
        }
        if (!succeeded) {
          self->Fail(error);
          return;
        }
        self->Success(api::tabs::Group::Results::Create(
            extension_group_id));
      });
    };
  }

  struct TabBatch {
    __strong ArcBrowser* browser;
    __strong NSMutableArray<NSNumber*>* ids;
  };
  bool Tabs(const std::vector<int>& ids,
            std::vector<TabBatch>* batches,
            std::string* error,
            NSMutableArray<NSNumber*>* ordered = nil,
            bool require_group_support = false,
            bool preserve_ordered_duplicates = false) {
    for (int tab_id : ids) {
      extensions::WindowController* window = nullptr;
      content::WebContents* contents = nullptr;
      if (!ExtensionTabUtil::GetTabById(tab_id, browser_context(),
                                        include_incognito_information(),
                                        &window, &contents, nullptr) ||
          !window || !contents) {
        *error = "No tab with id: " + base::NumberToString(tab_id) + ".";
        return false;
      }
      auto* browser = window->GetBrowserWindowInterface();
      if (!Allowed(browser) ||
          (require_group_support &&
           !ExtensionTabUtil::SupportsTabGroups(browser))) {
        *error = kUnavailable;
        return false;
      }
      if (!require_group_support) {
        auto* app = web_app::AppBrowserController::From(browser);
        if (app && app->IsIsolatedWebApp()) {
          *error = "The tab of an Isolated Web App cannot be moved.";
          return false;
        }
      }
      if (!ExtensionTabUtil::IsTabStripEditable(*browser->GetProfile())) {
        *error = ExtensionTabUtil::kTabStripNotEditableError;
        return false;
      }
      if (DevToolsWindow::IsDevToolsWindow(contents)) {
        *error = extensions::tabs_constants::kNotAllowedForDevToolsError;
        return false;
      }
      ArcBrowser* wrapper = BrowserForHostedExtensions(browser);
      NSNumber* session_id =
          @(sessions::SessionTabHelper::IdForTab(contents).id());
      if (ordered && (preserve_ordered_duplicates ||
                      ![ordered containsObject:session_id]))
        [ordered addObject:session_id];
      bool found = false;
      for (auto& batch : *batches) {
        if (batch.browser == wrapper) {
          if (![batch.ids containsObject:session_id])
            [batch.ids addObject:session_id];
          found = true;
          break;
        }
      }
      if (!found)
        batches->push_back(
            {wrapper, [NSMutableArray arrayWithObject:session_id]});
    }
    return true;
  }

 protected:
  void OnResponded() override { pending_.reset(); }
  void OnBrowserContextShutdown() override {
    // ExtensionFunction::Shutdown keeps this alive until it has cleared its
    // browser_context_, so releasing the pending request here is safe.
    profile_.reset();
    pending_.reset();
  }
  ~HostedGroupsFunction() override = default;
  base::WeakPtr<Profile> profile_;
  scoped_refptr<HostedGroupsFunction> pending_;
};

class HostedTabGroupsGetFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabGroups.get", TAB_GROUPS_GET)
  ResponseAction Run() override {
    auto params = api::tab_groups::Get::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    Begin();
    scoped_refptr<HostedTabGroupsGetFunction> self(this);
    bool has_browser = false;
    GlobalBrowserCollection::GetInstance()->ForEach(
        [&](BrowserWindowInterface* browser) {
          has_browser |= Allowed(browser) &&
                         ExtensionTabUtil::SupportsTabGroups(browser);
          return true;
        });
    if (!has_browser)
      return RespondNow(Error("No group with id: " +
                              base::NumberToString(params->group_id) + "."));
    const int group_id = params->group_id;
    Gather(^(std::shared_ptr<Groups> groups, NSString* error) {
      if (error) {
        self->Fail(error);
        return;
      }
      for (const auto& group : *groups) {
        if (group.info->extension_group_id == group_id) {
          self->RespondGroupInfo(group.window_id, *group.info);
          return;
        }
      }
      self->Fail(@"No group with specified id");
    }, std::nullopt, /*keep_inflight_results=*/true);
    return did_respond() ? AlreadyResponded() : RespondLater();
  }

 protected:
  ~HostedTabGroupsGetFunction() override = default;
};

class HostedTabGroupsQueryFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabGroups.query", TAB_GROUPS_QUERY)
  ResponseAction Run() override {
    auto params = api::tab_groups::Query::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    Begin();
    auto* current =
        ChromeExtensionFunctionDetails(this).GetCurrentWindowController();
    if (!current)
      return RespondNow(Error(ExtensionTabUtil::kNoCurrentWindowError));
    auto* browser = current->GetBrowserWindowInterface();
    if (!Allowed(browser))
      return RespondNow(Error(kUnavailable));
    const int current_id = ExtensionTabUtil::GetWindowId(browser);
    auto query = std::make_shared<api::tab_groups::Query::Params::QueryInfo>(
        std::move(params->query_info));
    std::optional<int> window_filter;
    if (query->window_id) {
      if (*query->window_id == extension_misc::kCurrentWindowId)
        window_filter = current_id;
      else if (*query->window_id >= 0)
        window_filter = *query->window_id;
    }
    scoped_refptr<HostedTabGroupsQueryFunction> self(this);
    Gather(
        ^(std::shared_ptr<Groups> groups, NSString* error) {
          if (error) {
            self->Fail(error);
            return;
          }
          base::ListValue result;
          for (const auto& group : *groups) {
            auto* native = self->Live(group.browser);
            if (!native)
              continue;
            const int window_id = ExtensionTabUtil::GetWindowId(native);
            if (query->window_id &&
                ((*query->window_id >= 0 && *query->window_id != window_id) ||
                 (*query->window_id == extension_misc::kCurrentWindowId &&
                  current_id != window_id)))
              continue;
            if (query->collapsed &&
                *query->collapsed != static_cast<bool>(group.info->collapsed))
              continue;
            if (query->title &&
                !base::MatchPattern(
                    base::UTF8ToUTF16(group.info->title),
                    base::UTF8ToUTF16(*query->title)))
              continue;
            if (query->color != api::tab_groups::Color::kNone &&
                static_cast<NSInteger>(ExtensionTabUtil::ColorToColorId(
                    query->color)) != group.info->color)
              continue;
            if (query->shared.value_or(false))
              continue;
            result.Append(self->Serialize(window_id, *group.info));
          }
          self->Success(base::ListValue().Append(std::move(result)));
        },
        window_filter);
    return did_respond() ? AlreadyResponded() : RespondLater();
  }

 protected:
  ~HostedTabGroupsQueryFunction() override = default;
};

class HostedTabGroupsUpdateFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabGroups.update", TAB_GROUPS_UPDATE)
  ResponseAction Run() override {
    auto params = api::tab_groups::Update::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    Begin();
    ArcTabGroupProperties* properties =
        [[NSClassFromString(@"ArcTabGroupProperties") alloc] init];
    if (params->update_properties.title)
      properties.title =
          base::SysUTF8ToNSString(*params->update_properties.title);
    if (params->update_properties.color != api::tab_groups::Color::kNone)
      properties.color = @(static_cast<int>(
          ExtensionTabUtil::ColorToColorId(params->update_properties.color)));
    if (params->update_properties.collapsed)
      properties.collapsed = @(*params->update_properties.collapsed);
    const int original_group_id = params->group_id;
    scoped_refptr<HostedTabGroupsUpdateFunction> self(this);
    Find(original_group_id, ^(ArcBrowser* browser, std::shared_ptr<const GroupInfo> info,
                              int source_window_id, NSString* error) {
      if (error) {
        self->Fail(error);
        return;
      }
      NSString* group_id = [info->group_id copy];
      self->Request(browser, ^(id target) {
        auto* native = self->Live(browser);
        if (!native) {
          self->FailString(kClosed);
          return;
        }
        if (!ExtensionTabUtil::IsTabStripEditable(*native->GetProfile())) {
          self->FailString(ExtensionTabUtil::kTabStripNotEditableError);
          return;
        }
        id<ArcHostedGroupDelegate> delegate = target;
        if (!properties ||
            ![delegate respondsToSelector:@selector(browser:updateTabGroupWithId:
                                                    properties:completion:)]) {
          self->FailString(kUnavailable);
          return;
        }
        [delegate browser:browser
            updateTabGroupWithId:group_id
                      properties:properties
                      completion:self->ReadbackCompletion(browser, original_group_id,
                          source_window_id, ReadbackKind::kUpdate)];
      });
    });
    return did_respond() ? AlreadyResponded() : RespondLater();
  }

 protected:
  ~HostedTabGroupsUpdateFunction() override = default;
};

class HostedTabGroupsMoveFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabGroups.move", TAB_GROUPS_MOVE)
  ResponseAction Run() override {
    auto params = api::tab_groups::Move::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    Begin();
    const auto window_id = params->move_properties.window_id;
    const int index = params->move_properties.index;
    const int original_group_id = params->group_id;
    scoped_refptr<HostedTabGroupsMoveFunction> self(this);
    Find(original_group_id, ^(ArcBrowser* browser, std::shared_ptr<const GroupInfo> info,
                              int source_window_id, NSString* error) {
      if (error) {
        self->Fail(error);
        return;
      }
      auto* native = self->Live(browser);
      if (!native) {
        self->FailString(kClosed);
        return;
      }
      if (!ExtensionTabUtil::IsTabStripEditable(*native->GetProfile())) {
        self->FailString(ExtensionTabUtil::kTabStripNotEditableError);
        return;
      }
      ArcBrowser* destination = browser;
      if (window_id) {
        std::string window_error;
        extensions::WindowController* target_controller = nullptr;
        // Group lookup is asynchronous; the dispatcher can have shut down
        // before Chromium's synchronous window resolver is called.
        if (!self->browser_context() || !self->dispatcher()) {
          self->FailString(kClosed);
          return;
        }
        if (!::windows_util::GetControllerFromWindowID(
                self.get(), *window_id,
                extensions::WindowController::GetAllWindowFilter(),
                &target_controller, &window_error)) {
          self->FailString(window_error);
          return;
        }
        auto* target_native = target_controller->GetBrowserWindowInterface();
        if (!self->Allowed(target_native)) {
          self->FailString(kUnavailable);
          return;
        }
        if (target_native->GetType() != BrowserWindowInterface::TYPE_NORMAL) {
          self->FailString(
              ExtensionTabUtil::kCanOnlyMoveTabsWithinNormalWindowsError);
          return;
        }
        if (!ExtensionTabUtil::SupportsTabGroups(target_native)) {
          self->FailString(kUnavailable);
          return;
        }
        if (target_native->GetProfile() != native->GetProfile()) {
          self->FailString(
              ExtensionTabUtil::kCanOnlyMoveTabsWithinSameProfileError);
          return;
        }
        if (!ExtensionTabUtil::IsTabStripEditable(
                *target_native->GetProfile())) {
          self->FailString(ExtensionTabUtil::kTabStripNotEditableError);
          return;
        }
        destination = BrowserForHostedExtensions(target_native);
      }
      // The retained app wrapper ignores the ArcBrowser argument and passes its
      // receiver to a Swift helper. Selecting the destination's delegate is an
      // inference from that shape; cross-window routing needs runtime evidence.
      NSString* group_id = [info->group_id copy];
      self->Request(destination, ^(id target) {
        if (!self->Live(browser) || !self->Live(destination)) {
          self->FailString(kClosed);
          return;
        }
        id<ArcHostedGroupDelegate> delegate = target;
        if (![delegate respondsToSelector:@selector(browser:moveTabGroupWithId:
                                                    toNewIndex:completion:)]) {
          self->FailString(kUnavailable);
          return;
        }
        [delegate browser:destination
            moveTabGroupWithId:group_id
                    toNewIndex:index
                    completion:self->ReadbackCompletion(browser, original_group_id,
                        source_window_id, ReadbackKind::kMove)];
      });
    });
    return did_respond() ? AlreadyResponded() : RespondLater();
  }

 protected:
  ~HostedTabGroupsMoveFunction() override = default;
};

class HostedTabsGroupFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabs.group", TABS_GROUP)
  ResponseAction Run() override {
    auto params = api::tabs::Group::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    EXTENSION_FUNCTION_VALIDATE(
        params->options.tab_ids.as_integer ||
        (params->options.tab_ids.as_integers &&
         !params->options.tab_ids.as_integers->empty()));
    if (params->options.group_id && params->options.create_properties)
      return RespondNow(Error(extensions::tabs_constants::kGroupParamsError));
    Begin();
    std::vector<int> ids =
        params->options.tab_ids.as_integers.value_or(std::vector<int>());
    if (params->options.tab_ids.as_integer)
      ids.push_back(*params->options.tab_ids.as_integer);
    std::vector<TabBatch> batches;
    std::string error;
    NSMutableArray<NSNumber*>* session_ids = [NSMutableArray array];
    if (!Tabs(ids, &batches, &error, session_ids))
      return RespondNow(Error(std::move(error)));
    scoped_refptr<HostedTabsGroupFunction> self(this);
    if (params->options.group_id) {
      Find(*params->options.group_id,
           ^(ArcBrowser* browser, std::shared_ptr<const GroupInfo> info, int,
             NSString* find_error) {
             if (find_error) {
               self->Fail(find_error);
               return;
             }
             self->Group(browser, info->group_id, session_ids);
           });
    } else {
      int window_id = extension_misc::kCurrentWindowId;
      if (params->options.create_properties &&
          params->options.create_properties->window_id)
        window_id = *params->options.create_properties->window_id;
      ArcBrowser* target = Window(window_id, &error);
      if (!target)
        return RespondNow(Error(std::move(error)));
      self->Group(target, nil, session_ids);
    }
    return did_respond() ? AlreadyResponded() : RespondLater();
  }
  void Group(ArcBrowser* browser, NSString* group_id, NSArray* session_ids) {
    NSString* identifier = [group_id copy];
    scoped_refptr<HostedTabsGroupFunction> self(this);
    Request(browser, ^(id delegate) {
      self->GroupAtDelivery(browser, identifier, session_ids, delegate);
    });
  }
  void GroupAtDelivery(ArcBrowser* browser, NSString* group_id,
                       NSArray* session_ids, id<ArcHostedGroupDelegate> delegate) {
    auto* native = Live(browser);
    if (!native) {
      FailString(kClosed);
      return;
    }
    if (!ExtensionTabUtil::IsTabStripEditable(*native->GetProfile())) {
      FailString(ExtensionTabUtil::kTabStripNotEditableError);
      return;
    }
    // A spanning extension can see both profiles, but Chrome does not allow
    // moving a regular tab into an off-the-record browser (or vice versa).
    // Recheck sources after the asynchronous group lookup before app mutation.
    std::vector<int> ids;
    for (NSNumber* session_id in session_ids)
      ids.push_back(session_id.intValue);
    std::vector<TabBatch> sources;
    std::string error;
    if (!Tabs(ids, &sources, &error)) {
      FailString(error);
      return;
    }
    for (const auto& source : sources) {
      auto* source_browser = Live(source.browser);
      if (!source_browser ||
          source_browser->GetProfile() != native->GetProfile()) {
        FailString(ExtensionTabUtil::kCanOnlyMoveTabsWithinSameProfileError);
        return;
      }
    }
    if (group_id) {
      if (![delegate respondsToSelector:@selector(browser:addTabsToGroupWithId:
                                                  sessionIds:completion:)]) {
        FailString(kUnavailable);
        return;
      }
      [delegate browser:browser
          addTabsToGroupWithId:group_id
                    sessionIds:session_ids
                    completion:GroupMutationCompletion(browser)];
    } else {
      ArcTabGroupProperties* properties =
          [[NSClassFromString(@"ArcTabGroupProperties") alloc] init];
      if (!properties ||
          ![delegate
              respondsToSelector:@selector(browser:createTabGroupWithSessionIds:
                                           properties:completion:)]) {
        FailString(kUnavailable);
        return;
      }
      [delegate browser:browser
          createTabGroupWithSessionIds:session_ids
                            properties:properties
                            completion:GroupMutationCompletion(browser)];
    }
  }

 protected:
  ~HostedTabsGroupFunction() override = default;
};

class HostedTabsUngroupFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabs.ungroup", TABS_UNGROUP)
  ResponseAction Run() override {
    auto params = api::tabs::Ungroup::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    EXTENSION_FUNCTION_VALIDATE(
        params->tab_ids.as_integer ||
        (params->tab_ids.as_integers && !params->tab_ids.as_integers->empty()));
    Begin();
    std::vector<int> ids =
        params->tab_ids.as_integers.value_or(std::vector<int>());
    if (params->tab_ids.as_integer)
      ids.push_back(*params->tab_ids.as_integer);
    std::vector<TabBatch> batches;
    std::string error;
    NSMutableArray<NSNumber*>* ordered = [NSMutableArray array];
    if (!Tabs(ids, &batches, &error, ordered, /*require_group_support=*/true,
              /*preserve_ordered_duplicates=*/true))
      return RespondNow(Error(std::move(error)));
    ArcBrowser* browser = batches.front().browser;
    NSArray<NSNumber*>* session_ids = [ordered copy];
    auto called = std::make_shared<bool>(false);
    scoped_refptr<HostedTabsUngroupFunction> self(this);
    Request(browser, ^(id target) {
      id<ArcHostedGroupDelegate> delegate = target;
      if (!self->Live(browser) ||
          ![delegate respondsToSelector:
              @selector(browser:removeTabsFromGroupWithSessionIds:completion:)]) {
        self->FailString(kClosed);
        return;
      }
      [delegate browser:browser
          removeTabsFromGroupWithSessionIds:session_ids
                                 completion:^(ArcTabGroupResult* result) {
                                   const bool succeeded = result && result.success;
                                   NSString* reply_error = [result.error copy];
                                   OnUI(^{
                                     if (*called || self->did_respond())
                                       return;
                                     *called = true;
                                     if (!succeeded) {
                                       self->Fail(reply_error.length ? reply_error
                                           : @"Failed to ungroup tabs");
                                       return;
                                     }
                                     self->Success(api::tabs::Ungroup::Results::Create());
                                   });
                                 }];
    });
    return did_respond() ? AlreadyResponded() : RespondLater();
  }

 protected:
  ~HostedTabsUngroupFunction() override = default;
};
}  // namespace

void RegisterHostedExtensionAPIs() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto& registry = ExtensionFunctionRegistry::GetInstance();
  registry.RegisterFunction<HostedTabsGroupFunction>();
  registry.RegisterFunction<HostedTabsUngroupFunction>();
  registry.RegisterFunction<HostedTabGroupsGetFunction>();
  registry.RegisterFunction<HostedTabGroupsQueryFunction>();
  registry.RegisterFunction<HostedTabGroupsUpdateFunction>();
  registry.RegisterFunction<HostedTabGroupsMoveFunction>();
}
}  // namespace arc
