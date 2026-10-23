// Copyright 2026 ArcMod.
#ifndef ARC_BROWSER_WEBEXTRAS_VALUES_H_
#define ARC_BROWSER_WEBEXTRAS_VALUES_H_

#include <string>
#include <vector>

namespace arc {
// Internal snapshots. The public const-void initializers copy these values;
// they never retain a pointer into a live accessibility tree.
struct AccessibilitySnapshot {
  int node_id = 0;
  std::string role;
  std::string name;
  std::string value;
  std::string description;
  bool is_clickable = false;
  bool is_editable = false;
  bool is_focusable = false;
  std::string child_tree_id;
  std::vector<int> children;
};
}  // namespace arc
#endif
