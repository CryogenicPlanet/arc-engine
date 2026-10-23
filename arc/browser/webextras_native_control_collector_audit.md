# Native-control collector audit

Read-only source and saved-artifact audit. No helper, native-mod, shared source,
app, profile, runtime, AX call, lock, test execution or coverage change. Evidence
is in `logs/wp/webextras-native-control-collector-audit/`; the independent child
`ax_saved_artifacts` separately inspected existing recordings without writes.

**Proven:** the collector does not invent a window or coerce AXApplication into
AXWindow. `agent-native.swift:24` creates the AXApplication root for the verified
PID. It traverses that root, then reads that root's actual AXWindows attribute
(lines51–52). reportedWindowCount is the array length; reportedWindowRoles is a
fresh AXRole read from each returned entry; observedWindowCount counts only
literal AXWindow nodes (line70). Thus count1/roles[AXApplication]/observed0 is
representable and explicitly distinguishes returned entries from actual window
roles. It is not evidence that a native AXWindow existed.

The walker compares CFEqual within CFHash buckets before adding nodes
(lines28–31). If an AXWindows entry aliases the root or another previously
visited element, fallback traversal adds no second node. This is a **source
possibility**, not proven raw identity: saved snapshots contain no CFEqual/root-
alias bit, per-returned-element PID or AX attribute error code. No claim that the
recorded entry equals the application root follows from its role alone.

Neither collector nor NativeController selects the frontmost/key window.
NativeController re-verifies one loopback listener's PID/executable per snapshot
or action; Swift independently requires the exact agent PID, bundle, executable
suffix and existing AX trust. Actions additionally require snapshot-owned handle,
current PID, path/fingerprint and advertised capability. No AXFocusedWindow or
NSApp.keyWindow fallback occurs. Foreground PID and app active/hidden state are
observations, not eligibility gates. Returned child AX element PIDs are not
individually recorded. Collection is not an atomic role/identity snapshot.

| Saved set | Snapshots | Literal AXWindow / AXButton | Window metadata |
|---|---:|---|---|
| webextras/devtools-current-run1 |4; PIDs11304/11519 |0 /0 throughout |reported1, roles[AXApplication], observed0 |
| ours/devtools |4; PIDs84675/85070 |0 /0 throughout |same metadata |
| golden/arc-1.170-devtools |9; PIDs10486/11034 |1 /26 or27 |reported/observed metadata absent |

Both ours sets have266 saved elements, no AXPopUpButton or 0.windows paths, and
no exact Toggle Console or Inspect Element element. All snapshots report
truncated=false and webContentExcluded=true. Golden exposes both requested
controls as enabled AXButtons with AXPress at0.0.16.5 and0.0.16.7, beneath its
AXWindow0.0. Disabled Chromium menu items named Inspect Elements / JavaScript
Console occur in **both** engines; they are not these canonical controls and do
not justify substitute labels/actions. No role or enabled assertion should change.

Current ours snapshots have appActive=false, hidden=false and foreground173;
golden foreground886 also differs from both golden target PIDs, yet its buttons
are accessible/enabled. Foreground difference alone therefore does not explain
missing controls. Golden lacks appActive/hidden/key-window fields; actual key-
window state remains unknown. This audit establishes no nativeView hosting,
pane, OS visibility, window construction or engine accessibility cause.

Canonical devtools.ts first creates/loads the fixture page, then presses exact
Toggle Console or Inspect Element. `native.ts:9–24` only attempts menu-open/
AXCancel/AXRaise recovery when exactly one matching command exists but is
disabled. Ours has zero matches, so that recovery branch does not apply and
observed() throws. Both saved ours runs report that error before a successful
frontend-open assertion. The complete current-run1 recordings contain devTools
getter/init/setDelegate, but no openConsoleView/openInspectView; golden contains
those calls and ensuing display callbacks. Missing genuine controls therefore
block exercise of those public opening methods; no open-method parity is proven.
No native-actions file exists, which alone would not prove no attempted action
because logging happens after action returns. Snapshot IDs lack recorder seq/
UTC joins, so exact API-relative snapshot timing is unrecorded.

Live Swift/native.ts/CDP helper and scenario/native.ts/devtools.ts match the
current-run1 frozen source inputs. Live launch-slice/run.ts has since changed;
this audit used the frozen runner for current-run1 timing/redaction/action-log
claims. Its readiness gate checks PID-matched sidebar-ready file plus CDP list,
not presence of native AXWindow/buttons. Golden has no corresponding frozen
helper source/binary provenance in that result directory, so identical historical
helper version cannot be asserted. Original nine snapshots are not nine ours
snapshots; the two ours sets are distinct runs, not byte mirrors.

**Actionable owner request:** retain current canonical role/control assertions.
CoreWindows already owns genuine window/role investigation; this audit requests
no duplicate runtime or guessed hosting fix. If its existing evidence cannot
distinguish returned-application aliases, ask the native-helper owner for a
separate approved diagnostic recording each AXWindows entry's raw role, attribute
result, element PID and CFEqual-to-root/previous-node before dedup. Keep counts
and roles truthful and distinguish sidebar/CDP readiness from AX readiness.
Those identity fields are presently missing; no helper change or new run is
part of this audit, and uncovered DevTools coverage remains blocked.
