// Copyright 2026 ArcMod. Application-wide EasyList conversion and publication.
#import "arc/api/arc_internal.h"

#include <memory>
#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "arc/common/ad_block_browser.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/raw_ref.h"
#include "base/no_destructor.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "components/subresource_filter/core/common/indexed_ruleset.h"
#include "components/subresource_filter/tools/ruleset_converter/rule_stream.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "url/origin.h"

ARC_EXPORT @interface ArcBrowserApplication : NSObject
- (BOOL)isFeatureEnabled:(NSUInteger)identifier;
@end

namespace arc {
namespace {
struct ApplicationAdBlockState {
  AdBlockConfiguration configuration;
  base::RepeatingClosureList observers;
  uint64_t requested_ruleset_id = 0;
  __weak id application = nil;
};

ApplicationAdBlockState& State() {
  static base::NoDestructor<ApplicationAdBlockState> state;
  return *state;
}

// Arc preprocesses $badfilter before handing text to RuleInputStream (original
// 0x4732008..0x47325a8). Preserve all non-badfilter options verbatim and remove
// the corresponding parsed URL rule in a second pass. Original keys are
// protobuf serialization, so option normalization belongs to RuleInputStream.
std::string WithoutBadFilter(std::string_view line, bool* is_badfilter) {
  *is_badfilter = false;
  const size_t dollar = line.rfind('$');
  if (dollar == std::string_view::npos || line.ends_with('/')) {
    return std::string(line);
  }
  std::vector<std::string> retained;
  for (auto option : base::SplitStringPiece(line.substr(dollar + 1), ",",
                                           base::TRIM_WHITESPACE,
                                           base::SPLIT_WANT_NONEMPTY)) {
    if (option.substr(0, option.find('=')) == "badfilter") {
      *is_badfilter = true;
    } else {
      retained.emplace_back(option);
    }
  }
  if (!*is_badfilter) {
    return std::string(line);
  }
  std::string result(line.substr(0, dollar));
  if (!retained.empty()) {
    result += '$';
    result += base::JoinString(retained, ",");
  }
  return result;
}

// Using TransferRules preserves upstream amendment of cosmetic-rule anchors,
// domain exceptions, and unsupported rule filtering before indexing.
class IndexOutput final : public subresource_filter::RuleOutputStream {
 public:
  IndexOutput(subresource_filter::RulesetIndexer* indexer,
              const std::set<std::string>& disabled)
      : indexer_(indexer), disabled_(disabled) {}
  bool PutUrlRule(const url_pattern_index::proto::UrlRule& rule) override {
    std::string key;
    rule.AppendPartialToString(&key);
    if (!disabled_->contains(key)) {
      has_url_rules_ |= indexer_->AddUrlRule(rule);
    }
    return true;
  }
  bool PutStyleRule(const url_pattern_index::proto::StyleRule& rule) override {
    styles_.push_back(rule);
    return true;
  }
  bool Finish() override { return true; }
  void IndexStyles() {
    // StyleRuleIndexer requires site-specific entries before generic entries
    // for the same selector, so exclusions cannot target an implicit index.
    std::stable_partition(styles_.begin(), styles_.end(),
                          [](const auto& rule) { return !rule.domains().empty(); });
    for (const auto& rule : styles_) {
      has_style_rules_ |= indexer_->AddStyleRuleFromProto(rule);
    }
  }

  bool has_url_rules() const { return has_url_rules_; }
  bool has_style_rules() const { return has_style_rules_; }

 private:
  bool has_url_rules_ = false;
  bool has_style_rules_ = false;
  // Stack-local adapter; it never outlives the stack-local indexer.
  const raw_ptr<subresource_filter::RulesetIndexer> indexer_;
  const raw_ref<const std::set<std::string>> disabled_;
  std::vector<url_pattern_index::proto::StyleRule> styles_;
};

base::File CompileRules(std::vector<std::string> lists, uint64_t ruleset_id,
                        bool* success, bool* has_cosmetic_rules) {
  if (lists.empty()) {
    *success = true;
    return {};
  }
  std::vector<std::string> lines;
  std::set<std::string> disabled;
  for (const auto& list : lists) {
    std::istringstream input(list);
    std::string line;
    while (std::getline(input, line)) {
      bool badfilter = false;
      auto ordinary = WithoutBadFilter(line, &badfilter);
      if (badfilter) {
        auto badfilter_input = subresource_filter::RuleInputStream::Create(
            std::make_unique<std::istringstream>(std::move(ordinary)),
            subresource_filter::RulesetFormat::kFilterList);
        if (badfilter_input && badfilter_input->FetchNextRule() == url_pattern_index::proto::RULE_TYPE_URL) {
          std::string key;
          badfilter_input->GetUrlRule().AppendPartialToString(&key);
          disabled.insert(std::move(key));
        }
      } else {
        lines.push_back(std::move(line));
      }
    }
  }
  std::string filtered;
  for (const auto& line : lines) {
    filtered += line;
    filtered += '\n';
  }
  subresource_filter::RulesetIndexer indexer(ruleset_id);
  IndexOutput output(&indexer, disabled);
  auto input = subresource_filter::RuleInputStream::Create(
      std::make_unique<std::istringstream>(std::move(filtered)),
      subresource_filter::RulesetFormat::kFilterList);
  if (!input || !subresource_filter::TransferRules(input.get(), &output, &output)) {
    return {};
  }
  output.IndexStyles();
  *has_cosmetic_rules = output.has_style_rules();
  if (!output.has_url_rules() && !output.has_style_rules()) {
    *success = true;
    return {};
  }
  indexer.Finish();

  base::FilePath temp_dir;
  base::FilePath path;
  if (!base::GetTempDir(&temp_dir)) {
    return {};
  }
  auto writable = base::CreateAndOpenTemporaryFileInDir(temp_dir, &path);
  if (!writable.IsValid()) {
    return {};
  }
  const bool written = writable.WriteAtCurrentPosAndCheck(indexer.data());
  base::File readonly;
  if (written) {
    readonly = base::File(path, base::File::FLAG_OPEN | base::File::FLAG_READ);
  }
  // Both descriptors remain valid after unlink; no renderer filesystem access.
  base::DeleteFile(path);
  *success = readonly.IsValid();
  return readonly;
}

using DeferredRulesetFile = std::unique_ptr<base::File, base::OnTaskRunnerDeleter>;

struct CompiledRules {
  DeferredRulesetFile file;
  bool success;
  bool has_cosmetic_rules;
};

CompiledRules CompileRulesForReply(
    std::vector<std::string> lists,
    uint64_t ruleset_id,
    scoped_refptr<base::SequencedTaskRunner> close_runner) {
  bool success = false;
  bool has_cosmetic_rules = false;
  auto file = CompileRules(std::move(lists), ruleset_id, &success, &has_cosmetic_rules);
  return {DeferredRulesetFile(new base::File(std::move(file)),
                              base::OnTaskRunnerDeleter(std::move(close_runner))),
          success, has_cosmetic_rules};
}

void PublishRules(uint64_t generation, CompiledRules result) {
  auto& state = State();
  if (state.requested_ruleset_id != generation || !result.success) {
    return;
  }
  // File::operator= closes its previous descriptor and cannot run that close
  // on UI. Stale results/cancelled replies retain the same MayBlock deleter.
  DeferredRulesetFile previous(
      new base::File(std::move(state.configuration.indexed_ruleset)),
      base::OnTaskRunnerDeleter(result.file.get_deleter().task_runner_));
  state.configuration.indexed_ruleset = std::move(*result.file);
  state.configuration.ruleset_id = generation;
  state.configuration.has_ruleset_update = true;
  state.configuration.has_cosmetic_rules = result.has_cosmetic_rules;
  if (state.configuration.enabled) {
    state.observers.Notify();
  }
}
}  // namespace

void ReleaseRendererRulesetFile(base::File file) {
  if (file.IsValid()) {
    auto close_runner = base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
         base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
    DeferredRulesetFile deferred(new base::File(std::move(file)),
                                 base::OnTaskRunnerDeleter(std::move(close_runner)));
  }
}

const AdBlockConfiguration& GetAdBlockConfiguration() {
  return State().configuration;
}

base::CallbackListSubscription ObserveAdBlockConfiguration(
    base::RepeatingClosure callback) {
  return State().observers.Add(std::move(callback));
}

bool RendererAdBlockFeatureEnabled() {
  return State().application && [State().application isFeatureEnabled:11];
}

void RefreshRendererAdBlockFeature() {
  if (![NSThread isMainThread]) {
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&RefreshRendererAdBlockFeature));
    return;
  }
  auto& state = State();
  const bool enabled = state.application && [state.application isFeatureEnabled:11];
  if (enabled != state.configuration.enabled) {
    state.configuration.enabled = enabled;
    if (!enabled) {
      ReleaseRendererRulesetFile(std::move(state.configuration.indexed_ruleset));
      state.configuration.has_ruleset_update = false;
    }
    if (enabled) {
      // Re-enabling republishes retained exclusions; disabling sends nothing.
      ++state.configuration.config_version;
      state.observers.Notify();
    }
  }
}

void SetRendererAdBlockApplication(id application) {
  if (![NSThread isMainThread]) {
    __weak id weakApplication = application;
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(^{
      SetRendererAdBlockApplication(weakApplication);
    }));
    return;
  }
  State().application = application;
  RefreshRendererAdBlockFeature();
  InitializeRendererProcessBridge();
}

void SetRendererBlockLists(std::vector<std::string> lists) {
  if (![NSThread isMainThread]) {
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&SetRendererBlockLists, std::move(lists)));
    return;
  }
  // The public category refreshes the feature state first. Native 0x42bd858
  // drops disabled requests before conversion/publication or generation changes.
  if (!State().configuration.enabled) {
    return;
  }
  const uint64_t generation = ++State().requested_ruleset_id;
  auto close_runner = base::ThreadPool::CreateSequencedTaskRunner(
      {base::MayBlock(), base::TaskPriority::USER_VISIBLE,
       base::TaskShutdownBehavior::BLOCK_SHUTDOWN});
  // Original SetBlockLists converts on a background task, then publishes.
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock(), base::TaskPriority::BEST_EFFORT},
      base::BindOnce(&CompileRulesForReply, std::move(lists), generation, std::move(close_runner)),
      base::BindOnce(&PublishRules, generation));
}

void SetRendererExcludedDomains(std::vector<std::string> domains) {
  if (![NSThread isMainThread]) {
    content::GetUIThreadTaskRunner({})->PostTask(
        FROM_HERE, base::BindOnce(&SetRendererExcludedDomains, std::move(domains)));
    return;
  }
  std::vector<std::string> origins;
  for (const auto& domain : domains) {
    // Verified original setter 0x47307b0..0x4730820 prepends this literal and
    // constructs a URL origin DTO, rather than parsing domain suffix patterns.
    auto origin = url::Origin::Create(GURL("https://" + domain));
    if (!origin.opaque()) {
      origins.push_back(origin.Serialize());
    }
  }
  State().configuration.excluded_origins = std::move(origins);
  ++State().configuration.config_version;
  // Native manager 0x42971f0 caches strings before its feature-11 guard.
  // Disabled changes become visible only at the next enabled publication.
  if (State().configuration.enabled) {
    State().observers.Notify();
  }
}
}  // namespace arc

@implementation ArcBrowserApplication (RendererAdBlock)
- (void)setBlockLists:(NSArray<NSString*>*)lists {
  arc::SetRendererAdBlockApplication(self);
  std::vector<std::string> native;
  for (NSString* list in lists) {
    native.push_back(ArcUTF8(list));
  }
  arc::SetRendererBlockLists(std::move(native));
}
- (void)setExcludedDomains:(NSArray<NSString*>*)domains {
  arc::SetRendererAdBlockApplication(self);
  std::vector<std::string> native;
  for (NSString* domain in domains) {
    native.push_back(ArcUTF8(domain));
  }
  arc::SetRendererExcludedDomains(std::move(native));
}
@end

@implementation ArcBrowserContext (RendererAdBlockDebug)
- (void)getAdBlockDebugInfo:(NSURL*)url completion:(void (^)(NSString*))completion {
  if (!completion) {
    return;
  }
  if (![NSThread isMainThread]) {
    __weak ArcBrowserContext* weakContext = self;
    NSURL* copiedURL = [url copy];
    void (^copiedCompletion)(NSString*) = [completion copy];
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, base::BindOnce(^{
      ArcBrowserContext* context = weakContext;
      if (context) {
        [context getAdBlockDebugInfo:copiedURL completion:copiedCompletion];
      } else {
        copiedCompletion(@"BrowserContext is headless");
      }
    }));
    return;
  }
  if (![self profile]) {
    completion(@"BrowserContext is headless");
    return;
  }
  arc::RefreshRendererAdBlockFeature();
  if (!arc::GetAdBlockConfiguration().enabled) {
    completion(@"Native ad blocker feature disabled");
    return;
  }
  arc::GetRendererAdBlockDebugInfo(
      [self profile], ArcGURL(url), base::BindOnce(^(const std::string& info) {
        completion(ArcNSString(info));
      }));
}
@end
