// Copyright 2026 ArcMod. Owned AI boundary snapshots, separate from model semantics.
#ifndef ARC_BROWSER_AI_BRIDGE_H_
#define ARC_BROWSER_AI_BRIDGE_H_

#import <Foundation/Foundation.h>
#include <deque>
#include <string>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#include "arc/browser/ai_c_abi.h"
#pragma clang diagnostic pop

namespace arc_ai {
namespace abi = mojoc;

// All pointers handed to the library refer to these owned snapshots, retained
// until the C invocation returns. As in Arc's utility shim, an asynchronous
// library must copy arguments it needs after returning from that invocation.
struct Input {
  std::deque<std::string> strings;
  std::deque<std::vector<float>> floats;
  std::deque<std::vector<abi::CommandBarRankingCandidate>> candidate_lists;
  std::vector<abi::String> texts;
  std::vector<abi::NomicEmbedTextItem> items;
  std::vector<abi::CommandBarRankingCandidate> candidates;
  std::vector<abi::CommandBarRankingExample> examples;
  abi::AIServiceConfiguration service{};
  abi::AIInferenceModel model{};
  abi::AIStreamedInferenceConfiguration inference{};
  abi::AIDistilBertClassifier classifier{};
  abi::NomicEmbedTextModel nomic{};
  abi::CommandBarRankingModel ranking{};
  abi::CommandBarRankingInput ranking_input{};
  abi::AIEmbeddingConfiguration embedding{};

  abi::String String(std::string text) {
    strings.push_back(std::move(text));
    const auto& stored = strings.back();
    return {stored.data(), static_cast<unsigned int>(stored.size())};
  }
  abi::FloatArray Floats(const std::vector<float>& values) {
    floats.push_back(values);
    return {floats.back().data(), static_cast<unsigned int>(values.size())};
  }
};

bool CopyServiceConfiguration(id object, Input& input);
bool CopyInferenceModel(id object, Input& input);
bool CopyInferenceConfiguration(id object, Input& input);
bool CopyClassifier(id object, Input& input);
bool CopyNomicModel(id object, Input& input);
bool CopyRankingModel(id object, Input& input);
void CopyNomicItems(NSArray* objects, Input& input);
bool CopyRankingInput(id object, Input& input);
void CopyRankingCandidates(NSArray* objects, Input& input);
void CopyRankingExamples(NSArray* objects, Input& input);
using Factory = id (^)();
Factory RankingResult(const abi::CommandBarRankingResult& result);
Factory RankingEvaluation(const abi::CommandBarRankingEvaluation& result);
Factory RankingTrainingResult(const abi::CommandBarRankingTrainingResult& result);
}  // namespace arc_ai
#endif
