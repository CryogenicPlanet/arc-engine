# Quit-state Closed suppression — APPLIED

Production arc/api/arc_web_contents.mm SHA256
ff957456ece2014edd6a0dda7b1424438b0b2b903fccbf257aae5becb82ceed9.
Historical promotion patch core_close_quit_suppression.patch SHA256
f5a933036f5042f4ff0786026ea9ffa6c6d1c68a42cc49b6a8b564b30af54e4b;
already applied, do not reapply it.

The original Closed handler reads browser_shutdown::IsTryingToQuit and suppresses
only UI Closed, while cleanup still runs. Receipt-time snapshot precedes existing
imminent/restoration callouts; only didClose delegate loading/query/send is gated.
Original flag identity is independently established by exact restart pref reset
strings and setter/getter; original TryToTerminate sets true before callbacks.
App owner has separately restored that transition, board140563. Core did not
edit app files. Source/raw evidence logs/wp/core.close-suppression-flag/.

This addresses a demonstrated producer condition relevant to webextras140398
bulk-close reentry FATAL; runtime flag was not measured in that failed run.
No global reentrancy bypass, arbitrary callback deferral, changed imminent,
public Close activation, physical destructor phase or logical-clear correction
is included. The remaining full-close integration stays held.

Actual current and full-seven-composition WC compilers exit0/empty; standard
check-file passed; fresh independent review no concrete P1/P2. Strict first
three owned companions+primitive+new adapters+zoom applies and reconstructs.
New accounting adapter patch bec65249 replaces historical c63b7f4: scope belongs
INSIDE the new guard. Other six staged files/API/semantics unchanged.
Evidence including exact sources, compiler argv/logs and review:
logs/wp/core.quit-suppression/. New runtime validation is pending; the previous
run7 window-binding passes predate this production change.
