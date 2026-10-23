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

struct GroupRecord {
  __strong ArcBrowser* browser;
  __strong ArcTabGroupInfo* info;
};
using Groups = std::vector<GroupRecord>;
using GroupsCompletion = void (^)(std::shared_ptr<Groups>, NSString*);
using GroupCompletion = void (^)(ArcBrowser*, ArcTabGroupInfo*, NSString*);

class HostedGroupsFunction : public ExtensionFunction {
 public:
  void Begin() {
    DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
    profile_ = Profile::FromBrowserContext(browser_context())->GetWeakPtr();
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

  void Gather(GroupsCompletion completion,
              std::optional<int> window_filter = std::nullopt) {
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
      id<ArcHostedGroupDelegate> delegate = BrowserExtensionDelegate(wrapper);
      if (!wrapper ||
          ![delegate
              respondsToSelector:@selector(
                                     browser:getAllTabGroupsWithCompletion:)]) {
        OnUI(^{
          if (!state->completed) {
            state->completed = true;
            completion(nullptr, base::SysUTF8ToNSString(kUnavailable));
          }
        });
        continue;
      }
      auto called = std::make_shared<bool>(false);
      [delegate browser:wrapper
          getAllTabGroupsWithCompletion:^(NSArray* infos) {
            OnUI(^{
              if (*called || state->completed)
                return;
              *called = true;
              if (!self->Live(wrapper) || !infos) {
                state->completed = true;
                completion(nullptr, base::SysUTF8ToNSString(kClosed));
                return;
              }
              for (id info in infos) {
                if ([info
                        isKindOfClass:NSClassFromString(@"ArcTabGroupInfo")]) {
                  state->by_browser[browser_index].push_back({wrapper, info});
                }
              }
              if (--state->remaining == 0) {
                state->completed = true;
                for (const auto& batch : state->by_browser) {
                  state->groups->insert(state->groups->end(), batch.begin(),
                                        batch.end());
                }
                completion(state->groups, nil);
              }
            });
          }];
    }
  }

  void Find(int group_id, GroupCompletion completion) {
    Gather(^(std::shared_ptr<Groups> groups, NSString* error) {
      if (error) {
        completion(nil, nil, error);
        return;
      }
      for (const auto& group : *groups) {
        if (group.info.extensionGroupId == group_id &&
            group.info.groupId.length) {
          completion(group.browser, group.info, nil);
          return;
        }
      }
      completion(nil, nil,
                 base::SysUTF8ToNSString("No group with id: " +
                                         base::NumberToString(group_id) + "."));
    });
  }

  base::DictValue Serialize(ArcBrowser* browser, ArcTabGroupInfo* info) {
    base::DictValue value;
    value.Set("id", static_cast<int>(info.extensionGroupId));
    value.Set("windowId", ExtensionTabUtil::GetWindowId(Live(browser)));
    value.Set("title", base::SysNSStringToUTF8(info.title ?: @""));
    auto color = info.color >= 0 && info.color < 9
                     ? static_cast<tab_groups::TabGroupColorId>(info.color)
                     : tab_groups::TabGroupColorId::kGrey;
    value.Set("color", api::tab_groups::ToString(
                           ExtensionTabUtil::ColorIdToColor(color)));
    value.Set("collapsed", static_cast<bool>(info.collapsed));
    value.Set("shared", false);
    return value;
  }

  void ReadGroup(ArcBrowser* browser, NSString* group_id) {
    if (!Live(browser)) {
      FailString(kClosed);
      return;
    }
    id<ArcHostedGroupDelegate> delegate = BrowserExtensionDelegate(browser);
    if (![delegate
            respondsToSelector:@selector(
                                   browser:getTabGroupWithId:completion:)]) {
      FailString(kUnavailable);
      return;
    }
    scoped_refptr<HostedGroupsFunction> self(this);
    auto called = std::make_shared<bool>(false);
    [delegate browser:browser
        getTabGroupWithId:group_id
               completion:^(ArcTabGroupInfo* info) {
                 OnUI(^{
                   if (*called)
                     return;
                   *called = true;
                   if (!self->Live(browser)) {
                     self->FailString(kClosed);
                     return;
                   }
                   if (!info) {
                     self->Fail(@"Tab group was not found.");
                     return;
                   }
                   base::ListValue result;
                   result.Append(self->Serialize(browser, info));
                   self->Success(std::move(result));
                 });
               }];
  }

  void (^MutationCompletion(ArcBrowser* browser,
                            bool return_id))(ArcTabGroupResult*) {
    scoped_refptr<HostedGroupsFunction> self(this);
    auto called = std::make_shared<bool>(false);
    return ^(ArcTabGroupResult* result) {
      OnUI(^{
        if (*called)
          return;
        *called = true;
        if (!self->Live(browser)) {
          self->FailString(kClosed);
          return;
        }
        if (!result || !result.success) {
          self->Fail(result.error);
          return;
        }
        if (return_id) {
          self->Success(api::tabs::Group::Results::Create(
              static_cast<int>(result.extensionGroupId)));
        } else if (!self->has_callback()) {
          self->Success(base::ListValue());
        } else if (result.groupId.length) {
          self->ReadGroup(browser, result.groupId);
        } else {
          self->Fail(@"Tab group operation returned no group ID.");
        }
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
            bool require_group_support = false) {
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
      if (ordered && ![ordered containsObject:session_id])
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
  ~HostedGroupsFunction() override = default;
  base::WeakPtr<Profile> profile_;
};

class HostedTabGroupsGetFunction : public HostedGroupsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("tabGroups.get", TAB_GROUPS_GET)
  ResponseAction Run() override {
    auto params = api::tab_groups::Get::Params::Create(args());
    EXTENSION_FUNCTION_VALIDATE(params);
    Begin();
    scoped_refptr<HostedTabGroupsGetFunction> self(this);
    Find(params->group_id,
         ^(ArcBrowser* browser, ArcTabGroupInfo* info, NSString* error) {
           if (error)
             self->Fail(error);
           else
             self->ReadGroup(browser, info.groupId);
         });
    return RespondLater();
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
                *query->collapsed != static_cast<bool>(group.info.collapsed))
              continue;
            if (query->title &&
                !base::MatchPattern(
                    base::SysNSStringToUTF16(group.info.title ?: @""),
                    base::UTF8ToUTF16(*query->title)))
              continue;
            if (query->color != api::tab_groups::Color::kNone &&
                static_cast<NSInteger>(ExtensionTabUtil::ColorToColorId(
                    query->color)) != group.info.color)
              continue;
            if (query->shared.value_or(false))
              continue;
            result.Append(self->Serialize(group.browser, group.info));
          }
          self->Success(base::ListValue().Append(std::move(result)));
        },
        window_filter);
    return RespondLater();
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
    scoped_refptr<HostedTabGroupsUpdateFunction> self(this);
    Find(params->group_id, ^(ArcBrowser* browser, ArcTabGroupInfo* info,
                             NSString* error) {
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
      id<ArcHostedGroupDelegate> delegate = BrowserExtensionDelegate(browser);
      if (!properties ||
          ![delegate respondsToSelector:@selector(browser:updateTabGroupWithId:
                                                  properties:completion:)]) {
        self->FailString(kUnavailable);
        return;
      }
      [delegate browser:browser
          updateTabGroupWithId:info.groupId
                    properties:properties
                    completion:self->MutationCompletion(browser, false)];
    });
    return RespondLater();
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
    scoped_refptr<HostedTabGroupsMoveFunction> self(this);
    Find(params->group_id, ^(ArcBrowser* browser, ArcTabGroupInfo* info,
                             NSString* error) {
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
      id<ArcHostedGroupDelegate> delegate =
          BrowserExtensionDelegate(destination);
      if (![delegate respondsToSelector:@selector(browser:moveTabGroupWithId:
                                                  toNewIndex:completion:)]) {
        self->FailString(kUnavailable);
        return;
      }
      [delegate browser:destination
          moveTabGroupWithId:info.groupId
                  toNewIndex:index
                  completion:self->MutationCompletion(destination, false)];
    });
    return RespondLater();
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
           ^(ArcBrowser* browser, ArcTabGroupInfo* info, NSString* find_error) {
             if (find_error) {
               self->Fail(find_error);
               return;
             }
             self->Group(browser, info.groupId, session_ids);
           });
    } else {
      int window_id = extension_misc::kCurrentWindowId;
      if (params->options.create_properties &&
          params->options.create_properties->window_id)
        window_id = *params->options.create_properties->window_id;
      ArcBrowser* target = Window(window_id, &error);
      if (!target)
        return RespondNow(Error(std::move(error)));
      OnUI(^{
        self->Group(target, nil, session_ids);
      });
    }
    return RespondLater();
  }
  void Group(ArcBrowser* browser, NSString* group_id, NSArray* session_ids) {
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
    id<ArcHostedGroupDelegate> delegate = BrowserExtensionDelegate(browser);
    if (group_id) {
      if (![delegate respondsToSelector:@selector(browser:addTabsToGroupWithId:
                                                  sessionIds:completion:)]) {
        FailString(kUnavailable);
        return;
      }
      [delegate browser:browser
          addTabsToGroupWithId:group_id
                    sessionIds:session_ids
                    completion:MutationCompletion(browser, true)];
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
                            completion:MutationCompletion(browser, true)];
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
    if (!Tabs(ids, &batches, &error, nil, true))
      return RespondNow(Error(std::move(error)));
    for (const auto& batch : batches) {
      if (![BrowserExtensionDelegate(batch.browser)
              respondsToSelector:
                  @selector(
                      browser:removeTabsFromGroupWithSessionIds:completion:)])
        return RespondNow(Error(kUnavailable));
    }
    auto remaining = std::make_shared<size_t>(batches.size());
    scoped_refptr<HostedTabsUngroupFunction> self(this);
    for (const auto& batch : batches) {
      ArcBrowser* browser = batch.browser;
      auto called = std::make_shared<bool>(false);
      id<ArcHostedGroupDelegate> delegate = BrowserExtensionDelegate(browser);
      // An earlier synchronous callback may have closed another source window
      // or cleared its delegate before this iteration.
      if (!Live(browser) ||
          ![delegate
              respondsToSelector:
                  @selector(
                      browser:removeTabsFromGroupWithSessionIds:completion:)]) {
        OnUI(^{
          self->FailString(kClosed);
        });
        break;
      }
      [delegate browser:browser
          removeTabsFromGroupWithSessionIds:batch.ids
                                 completion:^(ArcTabGroupResult* result) {
                                   OnUI(^{
                                     if (*called || self->did_respond())
                                       return;
                                     *called = true;
                                     if (!self->Live(browser)) {
                                       self->FailString(kClosed);
                                       return;
                                     }
                                     if (!result || !result.success) {
                                       self->Fail(result.error);
                                       return;
                                     }
                                     if (--*remaining == 0)
                                       self->Success(api::tabs::Ungroup::
                                                         Results::Create());
                                   });
                                 }];
    }
    return RespondLater();
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
