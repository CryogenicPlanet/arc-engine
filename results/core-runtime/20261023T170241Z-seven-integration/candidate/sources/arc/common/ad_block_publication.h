// Copyright 2026 ArcMod. Component publication decisions on the service sequence.
#ifndef ARC_COMMON_AD_BLOCK_PUBLICATION_H_
#define ARC_COMMON_AD_BLOCK_PUBLICATION_H_

namespace arc {
// Native cosmetics decide validity/live feature for each current bound agent.
// Callers supply only already-bound destinations, in their existing order.
template <typename Agents, typename Valid, typename Feature,
          typename Update, typename Reset>
void PublishAdBlockCosmetics(Agents&& agents, Valid&& valid, Feature&& feature,
                            Update&& update, Reset&& reset) {
  for (auto* agent : agents) {
    if (valid() && feature()) {
      update(*agent);
    } else {
      reset(*agent);
    }
  }
}

// Native URL publication makes one decision, shared by all agents and the local
// dealer. Its callback lifetime/cancellation belongs to the profile service.
template <typename Agents, typename Valid, typename Feature,
          typename Update, typename Reset, typename Local>
void PublishAdBlockURL(Agents&& agents, Valid&& valid, Feature&& feature,
                       Update&& update, Reset&& reset, Local&& local) {
  const bool publish = valid() && feature();
  for (auto* agent : agents) {
    if (publish) {
      update(*agent);
    } else {
      reset(*agent);
    }
  }
  local(publish);
}
}  // namespace arc
#endif  // ARC_COMMON_AD_BLOCK_PUBLICATION_H_
