"""Breakpoint callback for lldb_trace.py. Runs inside lldb, records the first entry, disables the
breakpoint and auto-continues (returns False)."""

import time

import lldb

BP_INFO = {}  # breakpoint id -> (module, file_va, [(identity, selector), ...])
STATE = {"out": None, "started": 0.0, "rank": 0, "main_tid": None}


def hit(frame, bp_loc, extra_args, internal_dict):
    bp = bp_loc.GetBreakpoint()
    bp_id = bp.GetID()
    info = BP_INFO.get(bp_id)
    thread = frame.GetThread()
    process = thread.GetProcess()
    if info is not None and STATE["out"] is not None:
        mod, va, idents = info
        x1 = frame.FindRegister("x1").GetValueAsUnsigned()
        err = lldb.SBError()
        try:
            sel = process.ReadCStringFromMemory(x1, 256, err) if x1 else ""
        except Exception:  # x1 is not a readable C string (direct call, not objc_msgSend)
            sel = ""
        if not err.Success() or sel is None:
            sel = ""
        if STATE["main_tid"] is None:
            STATE["main_tid"] = process.GetThreadAtIndex(0).GetThreadID()
        STATE["rank"] += 1
        tid = thread.GetThreadID()
        STATE["out"].write("%d\t%d\t%s\t%s\t%s\t%s\t%d\t%d\t%s\t%s\t%s\n" % (
            STATE["rank"], (time.time() - STATE["started"]) * 1000, mod, hex(va), sel,
            "yes" if sel in {s for _, s in idents} else "no",
            thread.GetIndexID(), tid, "yes" if tid == STATE["main_tid"] else "no",
            thread.GetName() or "", " | ".join(i for i, _ in idents)))
        STATE["out"].flush()
    bp.SetEnabled(False)
    return False
