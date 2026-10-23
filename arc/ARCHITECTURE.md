# ArcCore on Chromium: architecture contract

Goal: Arc's app (1.168.0) runs on an `ArcCore.framework` built from Chromium source, **feature complete and
behaving 1-1 with Arc's own engine at the app↔engine boundary**, so Arc can follow Chromium releases.
This file is binding for everyone writing code under `arc/`. Decisions here are made by the lead; ask the lead
(through your report) before deviating.

## Acceptance: the boundary diff, not compilation

- `tests/recorder/ArcEngineRecorder.dylib` logs every call across the boundary (app→engine for the 146 Arc
  classes, engine→app for the 270 app callbacks): arguments, returns, thread, nesting.
- Golden transcripts come from Arc's own engine (`../arc/build/ArcModAgent-trace156.app`, Arc 1.170 engine):
  `bun tests/launch-slice/run.ts --app <app> --out results/golden/<scenario> --record [--scenario <name>]`.
- A method is done when `python3 tests/recorder/diff.py <golden>/launch-N/record.jsonl <ours>/launch-N/record.jsonl`
  shows no missing call, return mismatch or callback mismatch for it, in every scenario that exercises it.
  Allowed differences (timing, object counts from caches) must be listed in `arc/DIFF-ALLOWLIST.md` with a reason.
- Specs: `specs/<Class>.md` (reviewed static research) say what each method does and which Chromium code the real
  engine calls. Use them; the transcript decides when they disagree.

## Layout and ownership

- `arc/api/<area>.mm`: ObjC classes. One primary `@implementation` per class, in exactly one file. Extra
  surface on a class owned by someone else goes in a category in your own file (`@implementation ArcWebContents
  (DevTools)`), never by editing their file.
- `arc/api/arc_internal.h`: shared internal declarations (native accessors, conversions). Add to it only
  through the lead or in a clearly separated section with your work package name.
- `arc/browser/`: C++ (observers, the BrowserWindow implementation, helpers).
- Unimplemented methods are stubs installed at load (`arc/api/arc_api_stubs.mm`, table from
  `arc/tools/gen_api_table.py`). After adding or removing a primary `@implementation`, rerun the generator.
- Declare every class you implement with `ARC_EXPORT` (default visibility) and subclass `NSObject`, exactly like
  the real metadata (`arc/tools/arccore-1.170.0-objc.tsv`: superclass, selectors, type encodings, properties).
  Selectors and type encodings must match the metadata exactly. Methods with C++ types in their encoding are
  ArcCore-internal; you don't need to reproduce them (use your own internal accessors instead).

## Object model

- **Identity.** One ObjC wrapper per native object for its lifetime. Get-or-create it through the native object's
  user data (`content::WebContentsUserData`, `Profile::SetUserData`, `base::SupportsUserData`), never by
  allocating a new wrapper per call. The app compares and caches these objects.
- **Ownership.** Native objects are owned by Chromium (or by the wrapper where Arc's API makes the app the
  owner, e.g. a WebContents not yet given to a browser). Wrappers hold `base::WeakPtr`/`raw_ptr`-safe references
  and must tolerate the native object going away (return the documented empty value, never crash).
- **Threads.** Everything on the UI (main) thread. Hop with `content::GetUIThreadTaskRunner({})` before
  touching wrappers or calling the app. Completion blocks are invoked on the main thread unless the golden
  transcript shows otherwise.
- **Delegates/observers.** Held weakly (`__weak`) like Arc's engine. Call optional protocol methods only after
  `respondsToSelector:`. Observer lists: `NSHashTable weakObjectsHashTable`, or an equivalent weak registration (e.g. weak
  registration tokens) where deferred/cancellable delivery needs it; observers are never retained.
- **Observable state.** Where the golden shows the engine calling its own setters (`-[ArcWebContents setTitle:]`,
  `setIsLoading:`, ...), state changes must go through those ObjC setters (the app observes them with KVO).
  Declare them as `@property (nonatomic)` so KVO works.
- **Errors.** `NSError` with domain `ArcErrorDomain` (`company.thebrowser.Arc`).
- **Values.** Convert with the helpers in `arc_internal.h` (strings, URLs, colors, images, times).

## Browser, windows, web contents, views

- `ArcBrowser` wraps a Chrome `Browser` (`Browser::Create`) whose `BrowserWindow` is `arc::ArcBrowserWindow`
  (`arc/browser/arc_browser_window.{h,cc}`). That window draws nothing: Arc's Swift UI is the UI. It reports to
  the `ArcBrowserDelegate` (tab creation, fullscreen, extension popups/toolbar) and answers Chrome's queries
  (bounds from the app's NSWindow given via `setWindow:`, active state, etc.). Start from the no-op behavior of
  `chrome/test/base/test_browser_window.*` and route what Arc's delegate protocol covers.
- Chrome's tab strip holds the browser's WebContents so extensions (`chrome.tabs`, tab groups API forwarded to
  the app) and Chrome services see real tabs.
- `ArcWebContents` wraps `content::WebContents`. Tab helpers are attached like Chrome's tabs
  (`TabHelpers::AttachTabHelpers`) for WebContents created by Arc. Its `content::WebContentsDelegate` and
  `WebContentsObserver` translate to Arc's navigation/UI/top-band-color delegates and to its observable setters.
- `nativeView` is the WebContents' NSView (`GetNativeView().GetNativeNSView()`), placed by the app in its own
  views. `createPaneHostView` returns an NSView container (`WebContentsPaneHostView`, ArcCore-internal class)
  that `attachToPaneHostView:`/`detachFromPaneHostView:` move the web contents view into and out of.

## Chrome UI that expects a BrowserView

Arc's windows aren't views `BrowserView`s, so Chrome code that looks one up gets null. Every such path a page
or the app can reach must do what Arc's engine does, found with a CDP scenario recorded on Arc's engine first:
- forward it to the app's callback (save offers: `webContents:didSubmitCredentials:`/`AddressEntry`/
  `CreditCardInfo`; tab-modal dialogs: `webContents:showsAuxiliaryWindow:` with the dialog's window), or
- give Chrome a host object from `ArcBrowserWindow` (`GetAutofillBubbleHandler`, `GetWebContentsModalDialogHost*`;
  `BrowserWindow::FindBrowserWindowWithWebContents` returns our window in host mode), or
- keep the web feature disabled when Arc's engine had it off (FedCM; see the `web-features` golden).
Never leave a lookup returning null where Chrome dereferences it: that's a crash on an ordinary page.

## Host mode (patches)

Chromium changes live only in `patches/*.patch` (applied by `sync-chromium.sh`). Keep them minimal and gated on
`--arcmod-host-app` where behavior differs from Chrome. A new patch needs the lead.

## Build, run, check (shared machine, one `out/Base`)

- Builds and app runs are serialized: `arc/tools/locked.sh <command>` (one lock for build + run).
  Typical: `arc/tools/locked.sh sh -c './build-babysit.sh && ./try-engine.sh 30'`.
- Fast syntax check of one file without the lock: `arc/tools/check-file.sh arc/api/<file>.mm`.
- Never touch `/Applications/Arc.app`, the real Arc profile, `~/Library/Application Support/Arc`, or ArcMod's
  normal data. Agent apps and their data under `ArcModAgent-*` are disposable. No VMs, no pushes, no commits
  (the lead reviews and commits).
- Chromium style: `-Werror`, ARC, `base::NoDestructor` for statics, no raw `strcmp`-style buffer calls,
  `std::string_view`, `base::SysNSStringToUTF8` etc.
