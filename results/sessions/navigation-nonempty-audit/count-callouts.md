# Exact incoming count callouts and minimal setter sequence

Original setter443d0c8..443d7b4, including outlined/cold blocks, has exactly these ObjC message sites:

-443d148: first countByEnumeratingWithState:objects:count:, selector pointere28c970.
-443d214: next batch of the SAME fast-enumeration selector through savedx21.
-443d250: element session_navigation, selector pointere291128.
-443d238: objc_enumerationMutation, not an array count query.

The count: argument of NSFastEnumeration is batch buffer capacity16 (w4=0x10 at443d144/443d210), NOT a query of the incoming NSArray length. No incoming -count is sent before or during this setter route. Full direct call-site census and selector pointer reads are appended to original-disassembly.txt.

The actual empty-history guard is LATER in backend4340c10:4340c30 loads `[x1+8]`, the length of the native handle vector assembled from successful element enumeration;4340c34 branches zero to4340e0c. This is not a read of an NSArray object. Current count-property guard530..533 and reserve535 add TWO app-object method callouts and judge reported count rather than actual yielded entries.

Minimal faithful sequence for an existing valid native tab:

```objc
CHECK(_tab);
_navigations = navigations;  // Strong, exact input identity; before app-array messages.
std::vector<sessions::SerializedNavigationEntry> entries;
for (ArcSessionNavigation* navigation in navigations) {
  // Keep current supported-member/native-peer checks and value copy.
  entries.push_back(*[navigation chromeNavigation]);
}
CHECK_GT(entries.size(), 0u);
_tab->navigations = std::move(entries);
```

Do not call navigations.count for guard or reserve. A normal empty/nil input yields no entries and still fails the nonempty guard. For a custom array whose reported count differs from enumeration, original judges actual yielded items, so the vector-size guard matches that recovered boundary. This is a conditional static semantic observation; no custom-array fixture, native DTO fabrication, or deliberate failure was executed.

This changes neither selected index nor exact retained-array identity. Old/new native-vector commit still happens only after conversion and the nonempty check. Invalid peer/member timing outside this valid-native/nonempty scope is not broadened here.
