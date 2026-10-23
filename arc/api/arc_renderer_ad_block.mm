// Copyright 2026 ArcMod. Application-wide EasyList conversion and publication.
#import "arc/api/arc_internal.h"

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "arc/common/ad_block_browser.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/location.h"
#include "base/no_destructor.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/task/thread_pool.h"
#include "components/subresource_filter/core/common/indexed_ruleset.h"
#include "components/subresource_filter/tools/ruleset_converter/rule_stream.h"
#include "url/origin.h"

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end

namespace arc {
namespace {
struct ApplicationAdBlockState {
  AdBlockConfiguration configuration;
  base::RepeatingClosureList observers;
  uint64_t requested_ruleset_id = 0;
};

ApplicationAdBlockState& State() {
  static base::NoDestructor<ApplicationAdBlockState> state;
  return *state;
}

// Arc preprocesses $badfilter before handing text to RuleInputStream (original
// 0x4732008..0x47325a8). Preserve all non-badfilter options verbatim and remove
// the corresponding ordinary rule in a second pass, including across lists.
// TODO(renderer-contract): recover whether Arc canonicalizes option order or
// whitespace when comparing disabled rules. Do not invent normalization here.
std::string WithoutBadFilter(std::string_view line, bool* is_badfilter) {
  *is_badfilter = false;
  const size_t dollar = line.rfind('$');
  if (dollar == std::string_view::npos || line.ends_with('/')) {
    return std::string(line);
  }
  std::vector<std::string> retained;
  for (auto option : base::SplitStringPiece(line.substr(dollar + 1), ",",
                                           base::KEEP_WHITESPACE,
                                           base::SPLIT_WANT_ALL)) {
    if (option == "badfilter") {
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
  explicit IndexOutput(subresource_filter::RulesetIndexer* indexer)
      : indexer_(indexer) {}
  bool PutUrlRule(const url_pattern_index::proto::UrlRule& rule) override {
    indexer_->AddUrlRule(rule);
    return true;
  }
  bool PutStyleRule(const url_pattern_index::proto::StyleRule& rule) override {
    indexer_->AddStyleRuleFromProto(rule);
    return true;
  }
  bool Finish() override { return true; }

 private:
  // Stack-local adapter; it never outlives the stack-local indexer.
  const raw_ptr<subresource_filter::RulesetIndexer> indexer_;
};

base::File CompileRules(std::vector<std::string> lists, uint64_t ruleset_id) {
  std::vector<std::string> lines;
  std::set<std::string> disabled;
  for (const auto& list : lists) {
    std::istringstream input(list);
    std::string line;
    while (std::getline(input, line)) {
      bool badfilter = false;
      auto ordinary = WithoutBadFilter(line, &badfilter);
      if (badfilter) {
        disabled.insert(std::move(ordinary));
      } else {
        lines.push_back(std::move(line));
      }
    }
  }
  std::string filtered;
  for (const auto& line : lines) {
    if (!disabled.contains(line)) {
      filtered += line;
      filtered += '\n';
    }
  }
  subresource_filter::RulesetIndexer indexer(ruleset_id);
  IndexOutput output(&indexer);
  auto input = subresource_filter::RuleInputStream::Create(
      std::make_unique<std::istringstream>(std::move(filtered)),
      subresource_filter::RulesetFormat::kFilterList);
  if (!input || !subresource_filter::TransferRules(input.get(), &output, &output)) {
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
  return readonly;
}

void PublishRules(uint64_t generation, base::File file) {
  auto& state = State();
  if (state.requested_ruleset_id != generation || !file.IsValid()) {
    return;
  }
  state.configuration.indexed_ruleset = std::move(file);
  state.configuration.ruleset_id = generation;
  state.observers.Notify();
}
}  // namespace

const AdBlockConfiguration& GetAdBlockConfiguration() {
  return State().configuration;
}

base::CallbackListSubscription ObserveAdBlockConfiguration(
    base::RepeatingClosure callback) {
  return State().observers.Add(std::move(callback));
}

void SetRendererBlockLists(std::vector<std::string> lists) {
  const uint64_t generation = ++State().requested_ruleset_id;
  // Original SetBlockLists converts on a background task, then publishes.
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock(), base::TaskPriority::USER_VISIBLE},
      base::BindOnce(&CompileRules, std::move(lists), generation),
      base::BindOnce(&PublishRules, generation));
}

void SetRendererExcludedDomains(std::vector<std::string> domains) {
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
  State().observers.Notify();
}
}  // namespace arc

@implementation ArcBrowserApplication (RendererAdBlock)
- (void)setBlockLists:(NSArray<NSString*>*)lists {
  std::vector<std::string> native;
  for (NSString* list in lists) {
    native.push_back(ArcUTF8(list));
  }
  arc::SetRendererBlockLists(std::move(native));
}
- (void)setExcludedDomains:(NSArray<NSString*>*)domains {
  std::vector<std::string> native;
  for (NSString* domain in domains) {
    native.push_back(ArcUTF8(domain));
  }
  arc::SetRendererExcludedDomains(std::move(native));
}
@end
