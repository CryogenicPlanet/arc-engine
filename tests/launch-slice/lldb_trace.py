"""Launch an ArcMod agent build under lldb and record the first entry of every Arc engine method
and app callback implementation.

Run with Xcode's Python and lldb module:
  PYTHONPATH="$(lldb -P)" xcrun python3 lldb_trace.py --app <ArcModAgent-x.app> --out <dir> [--] [chromium args]

Breakpoints sit on each distinct ArcCore IMP (specs/methods.tsv imp_va, unslid 1.170.0 VAs)
and each app protocol implementation (app-contract-joins.tsv app_imp). At a hit the tracer reads x1
as a C string (the selector, when entered through objc_msgSend) and the thread, then disables
the breakpoint. It records first
entries only: later calls, arguments and receivers are not traced. Output: trace.tsv, summary.json.
"""

import argparse
import csv
import json
import os
import sys
import subprocess
import threading
import time

import lldb

ENGINE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
METHODS = os.path.join(ENGINE_DIR, "specs/methods.tsv")
APP_JOINS = os.path.join(
    ENGINE_DIR,
    "research/2026-10-22-sol/evidence/agents/protocols/initial/evidence/app-contract-joins.tsv",
)


def load_targets():
    """Return {(module, file_va): [identity, ...]} for engine IMPs and app callback IMPs."""
    targets = {}
    with open(METHODS) as f:
        for row in csv.DictReader(f, delimiter="\t"):
            ident = "%s[%s%s%s]" % (
                row["kind"],
                row["class_name"],
                "(" + row["category"] + ")" if row["category"] else "",
                " " + row["selector"],
            )
            targets.setdefault(("ArcCore", int(row["imp_va"], 16)), []).append((ident, row["selector"]))
    with open(APP_JOINS) as f:
        for row in csv.DictReader(f, delimiter="\t"):
            ident = "%s[%s %s] <%s>" % (row["kind"], row["app_class"], row["selector"], row["protocol_name"])
            targets.setdefault(("Arc", int(row["app_imp"], 16)), []).append((ident, row["selector"]))
    return targets


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--app", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--max-seconds", type=float, default=600)
    ap.add_argument("args", nargs="*")
    opts = ap.parse_args()
    os.makedirs(opts.out, exist_ok=True)
    exe = os.path.join(opts.app, "Contents/MacOS/Arc")

    dbg = lldb.SBDebugger.Create()
    dbg.SetAsync(True)
    target = dbg.CreateTargetWithFileAndArch(exe, "arm64")
    if not target.IsValid():
        sys.exit("could not create target for " + exe)

    modules = {}
    for m in target.module_iter():
        name = m.GetFileSpec().GetFilename()
        if name in ("Arc", "ArcCore"):
            modules[name] = m
    missing = {"Arc", "ArcCore"} - set(modules)
    if missing:
        sys.exit("modules not found before launch: %s" % sorted(missing))

    # Module-qualified file addresses, set the way `breakpoint set -s <module> -a <file va>` does.
    # (SBTarget.BreakpointCreateBySBAddress on a pre-launch module did not plant usable sites.)
    targets = load_targets()
    bp_info = {}
    unresolved = []
    ci = dbg.GetCommandInterpreter()
    for (mod, va), idents in sorted(targets.items()):
        addr = modules[mod].ResolveFileAddress(va)
        section = addr.GetSection() if addr.IsValid() else None
        if section is None or not section.IsValid() or section.GetName() != "__text":
            unresolved.append((mod, hex(va), section.GetName() if section and section.IsValid() else ""))
            continue
        res = lldb.SBCommandReturnObject()
        ci.HandleCommand("breakpoint set -s %s -a 0x%x" % (mod, va), res)
        if not res.Succeeded():
            unresolved.append((mod, hex(va), res.GetError().strip()))
            continue
        bp = target.GetBreakpointAtIndex(target.GetNumBreakpoints() - 1)
        bp_info[bp.GetID()] = (mod, va, idents)

    # Callbacks run inside lldb: record the first entry, disable the breakpoint, auto-continue.
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import lldb_trace_cb as cb
    res = lldb.SBCommandReturnObject()
    ci.HandleCommand("command script import %s" % cb.__file__.replace(".pyc", ".py"), res)
    cb.BP_INFO.update(bp_info)
    for bp_id in bp_info:
        target.FindBreakpointByID(bp_id).SetScriptCallbackFunction("lldb_trace_cb.hit")
    trace = open(os.path.join(opts.out, "trace.tsv"), "w")
    trace.write("rank\tt_ms\tmodule\tfile_va\tx1_selector\tselector_match\tthread_index\tthread_id\tmain_thread\tthread_name\tcandidates\n")
    cb.STATE["out"] = trace

    # Launch through LaunchServices like tools/run-mod.sh (a direct exec of the app crashed on its first
    # tab open), and attach as the process starts. Breakpoints are module-relative, so they resolve when
    # ArcCore loads, about 2 s before the engine's first entry.
    log = os.path.join(opts.out, "app.log")
    started = time.time()
    cb.STATE["started"] = started

    def open_app():
        time.sleep(1.0)
        subprocess.run(["open", "-n", "-g", "-a", opts.app, "--stdout", log, "--stderr", log, "--args"] + opts.args)

    threading.Thread(target=open_app, daemon=True).start()
    attach = lldb.SBAttachInfo("Arc", True)  # wait for a new process named Arc
    err = lldb.SBError()
    process = target.Attach(attach, err)
    if not err.Success() or not process.IsValid():
        sys.exit("attach failed: %s" % err)
    pid = process.GetProcessID()
    path = subprocess.run(["ps", "-p", str(pid), "-o", "comm="], capture_output=True, text=True).stdout.strip()
    if os.path.realpath(path) != os.path.realpath(exe):
        process.Detach()
        sys.exit("attached to a different Arc process (%d, %s); detached" % (pid, path))
    with open(os.path.join(opts.out, "browser.pid"), "w") as f:
        f.write(str(pid))
    for _ in range(20):
        if process.GetState() == lldb.eStateStopped:
            break
        time.sleep(0.1)
    process.Continue()

    # Real stops reaching us are not ours (traps, signals, crashes). Log them; a trap that repeats
    # under a debugger never makes progress, so treat it as a crash.
    listener = dbg.GetListener()
    event = lldb.SBEvent()
    foreign_stops = {}
    crashed = False
    events = open(os.path.join(opts.out, "events.log"), "w")
    while True:
        if time.time() - started > opts.max_seconds:
            # Stop first and save every thread's stack, so a stall can be diagnosed.
            process.Stop()
            for _ in range(10):
                if process.GetState() == lldb.eStateStopped:
                    break
                time.sleep(0.5)
            with open(os.path.join(opts.out, "stacks-at-timeout.txt"), "w") as f:
                for thread in process:
                    f.write("thread %d %s reason=%d\n" % (thread.GetIndexID(), thread.GetName() or "", thread.GetStopReason()))
                    for frame in list(thread)[:14]:
                        f.write("    %s\n" % frame)
            process.Kill()
            break
        if not listener.WaitForEvent(1, event) or not lldb.SBProcess.EventIsProcessEvent(event):
            continue
        state = lldb.SBProcess.GetStateFromEvent(event)
        restarted = lldb.SBProcess.GetRestartedFromEvent(event)
        events.write("%d\t%s\trestarted=%s\n" % ((time.time() - started) * 1000, dbg.StateAsCString(state), restarted))
        events.flush()
        if state in (lldb.eStateExited, lldb.eStateDetached, lldb.eStateCrashed):
            break
        if state != lldb.eStateStopped or restarted:
            continue
        for thread in process:
            reason = thread.GetStopReason()
            if reason in (lldb.eStopReasonNone, lldb.eStopReasonInvalid):
                continue
            pc = thread.GetFrameAtIndex(0).GetPC()
            events.write("  stop thread=%d reason=%d %s at %s\n" % (thread.GetIndexID(), reason,
                thread.GetStopDescription(200), target.ResolveLoadAddress(pc)))
            if reason != lldb.eStopReasonBreakpoint:
                foreign_stops[pc] = foreign_stops.get(pc, 0) + 1
        if any(n > 3 for n in foreign_stops.values()):
            crashed = True
            process.Kill()
            break
        process.Continue()

    trace.close()
    resolved = sum(1 for i in bp_info if target.FindBreakpointByID(i).GetNumResolvedLocations() > 0)
    summary = {
        "app": opts.app,
        "args": opts.args,
        "breakpoints": len(bp_info),
        "unresolved_before_launch": unresolved,
        "resolved_at_end": resolved,
        "hits": cb.STATE["rank"],
        "crashed_on_foreign_trap": crashed,
        "foreign_stops": {hex(k): v for k, v in foreign_stops.items()},
        "exit_status": process.GetExitStatus(),
        "exit_description": process.GetExitDescription(),
        "seconds": round(time.time() - started, 1),
    }
    with open(os.path.join(opts.out, "summary.json"), "w") as f:
        json.dump(summary, f, indent=2)
    print(json.dumps(summary))


if __name__ == "__main__":
    main()
