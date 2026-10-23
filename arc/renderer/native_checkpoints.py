#!/usr/bin/env python3
"""Bounded, exact-PID observation of the disposable original-engine trace156 app.

Read-only preflight (never attaches):
  python3 arc/renderer/native_checkpoints.py --preflight
Attach after the parent harness has produced renderer-request.json.ready.json:
  python3 arc/renderer/native_checkpoints.py --attach --pid PID \
    --ready results/RUN/trace156/renderer-request.json.ready.json \
    --out results/RUN/trace156/native-checkpoints.jsonl --seconds 90

Wait for OUT.ready.json before submitting the operation to be observed. This
script neither launches nor signals the app, invokes native functions, changes
preferences, nor grants debugging privileges. Breakpoints perturb scheduling;
absence of a hit is evidence only for this bounded observation window.
"""

import argparse
import ctypes
import hashlib
import json
import os
from pathlib import Path
import plistlib
import signal
import subprocess
import sys
import threading
import time


ROOT = Path(__file__).resolve().parents[2]
APP = ROOT.parent / "arc/build/ArcModAgent-trace156.app"
EXE = APP / "Contents/MacOS/Arc"
# Same original image as arc/common/renderer_contract_evidence.py's default.
IMAGE = APP / "Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
IMAGE_SHA256 = "b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d"
IMAGE_UUID = "4C4C44A0-5555-3144-A1B3-01C201C55814"
# Header-relative arm64 addresses and original instruction bytes, before traps.
CHECKPOINTS = {
    "request_gate": (0x42BD860, "20050034"),
    "compiler_worker": (0x42BDA30, "ff4301d1"),
    "postcompile": (0x42BDA4C, "c03804b0"),
    "ui_completion": (0x42C0538, "ffc300d1"),
    "cosmetic_setter": (0x472F990, "fc6fbba9"),
    "cosmetic_broadcast": (0x4730A08, "ff4307d1"),
    "url_broadcast": (0x4730BB4, "fc6fbaa9"),
    "exclusion_broadcast": (0x4730FA0, "f657bda9"),
}
_sink = None
_sink_lock = threading.Lock()
_labels = {}
_counts = {}
_hits = 0
_hit_limit = 512
_deadline = 0.0


def emit(event, **fields):
    row = {"event": event, "timestamp": time.time(), "monotonic": time.monotonic(), **fields}
    with _sink_lock:
        if _sink is not None:
            _sink.write(json.dumps(row, sort_keys=True) + "\n")
            _sink.flush()


def process_identity(pid):
    if pid <= 1:
        raise ValueError("PID must be an explicit positive app PID")
    libproc = ctypes.CDLL("/usr/lib/libproc.dylib", use_errno=True)
    libproc.proc_pidpath.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_uint32]
    libproc.proc_pidpath.restype = ctypes.c_int
    buf = ctypes.create_string_buffer(4096)
    if libproc.proc_pidpath(pid, buf, len(buf)) <= 0:
        raise RuntimeError("cannot read exact PID executable via proc_pidpath")
    executable = Path(os.fsdecode(buf.value))
    if executable != EXE or executable.resolve() != EXE:
        raise RuntimeError(f"PID executable is not the exact trace156 executable: {executable}")
    started = subprocess.check_output(
        ["/bin/ps", "-p", str(pid), "-o", "lstart="], text=True, timeout=3,
        env=dict(os.environ, LC_ALL="C")).strip()
    if not started:
        raise RuntimeError("PID no longer exists")
    return {"pid": pid, "executable": str(executable), "started": started}


def validate(pid=None, ready=None):
    if APP.is_symlink() or EXE.is_symlink() or IMAGE.is_symlink():
        raise RuntimeError("refusing symlinked app/executable/ArcCore image")
    if APP.resolve() != APP or EXE.resolve() != EXE or IMAGE.resolve() != IMAGE:
        raise RuntimeError("refusing redirected app/executable/ArcCore image")
    with (APP / "Contents/Info.plist").open("rb") as f:
        plist = plistlib.load(f)
    if (plist.get("ArcModAgentSession") != "trace156" or
            plist.get("CFBundleIdentifier") != "company.thebrowser.Browser.mod.agent.trace156" or
            plist.get("CFBundleExecutable") != "Arc"):
        raise RuntimeError("trace156 bundle/session identity mismatch")
    with IMAGE.open("rb") as f:
        hasher = hashlib.sha256()
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            hasher.update(chunk)
        digest = hasher.hexdigest()
    if digest != IMAGE_SHA256:
        raise RuntimeError("ArcCore is not the pinned pristine contract-evidence image")
    result = {"image": str(IMAGE), "image_sha256": digest, "image_uuid": IMAGE_UUID,
              "bundle": plist["CFBundleIdentifier"], "session": "trace156"}
    if pid is not None:
        ready = Path(ready).absolute()
        if (ready.name != "renderer-request.json.ready.json" or ready.parent.name != "trace156" or
                not ready.is_relative_to(ROOT / "results") or ready.resolve() != ready):
            raise RuntimeError("ready path must be a real results/<run>/trace156 probe ready file")
        info = json.loads(ready.read_text())
        if type(info.get("pid")) is not int or info["pid"] != pid or info.get("installed") is not True:
            raise RuntimeError("probe ready file does not identify the requested installed main PID")
        identity = process_identity(pid)
        # A ready file written before this process began is stale.
        started = time.mktime(time.strptime(identity["started"], "%a %b %d %H:%M:%S %Y"))
        if ready.stat().st_mtime < started:
            raise RuntimeError("probe ready file predates this PID's start")
        result.update(identity)
        result["ready"] = str(ready)
    return result


def checkpoint(frame, bp_loc, _internal_dict):
    """LLDB callback: bounded metadata only, always automatically resume."""
    global _hits
    bp = bp_loc.GetBreakpoint()
    label = _labels.get(bp.GetID(), "unknown")
    _hits += 1
    _counts[label] = _counts.get(label, 0) + 1
    thread = frame.GetThread()
    fields = {}
    if label == "request_gate":
        # This instruction branches on the just-returned feature-11 bool.
        # Record only that known scalar, never arbitrary arguments or memory.
        fields["feature_11"] = bool(frame.FindRegister("w0").GetValueAsUnsigned() & 1)
    emit("checkpoint", checkpoint=label, pid=thread.GetProcess().GetProcessID(),
         thread_id=thread.GetThreadID(), thread_index=thread.GetIndexID(),
         address=hex(frame.GetPC()), offset=hex(CHECKPOINTS[label][0]), hit=_hits, **fields)
    if _counts[label] >= 128:
        bp.SetEnabled(False)
        emit("checkpoint_cap", checkpoint=label, count=_counts[label])
    if _hits >= _hit_limit or time.monotonic() >= _deadline:
        thread.GetProcess().GetTarget().DisableAllBreakpoints()
    return False


def observe(debugger):
    """Called only inside batch LLDB, never on ordinary CLI preflight."""
    import lldb
    global _sink, _deadline, _hit_limit
    cfg = json.loads(os.environ["ARC_NATIVE_CHECKPOINT_CONFIG"])
    out = Path(cfg["out"])
    process = None
    target = None
    status = "error"
    interrupted = threading.Event()
    old_handlers = {}
    for sig in (signal.SIGINT, signal.SIGTERM):
        old_handlers[sig] = signal.signal(sig, lambda _s, _f: interrupted.set())
    try:
        _sink = out.open("x", buffering=1)
        identity = validate(cfg["pid"], cfg["ready"])
        if identity != cfg["identity"]:
            raise RuntimeError("identity changed between preflight and LLDB attachment")
        emit("verified", **identity)
        debugger.SetAsync(True)
        debugger.HandleCommand("settings set target.process.detach-keeps-stopped false")
        debugger.HandleCommand("settings set target.process.interrupt-timeout 3")
        debugger.HandleCommand("settings set plugin.process.gdb-remote.packet-timeout 5")
        target = debugger.CreateTarget(str(EXE))
        listener = lldb.SBListener("arc-native-checkpoints")
        error = lldb.SBError()
        emit("attach_started", pid=cfg["pid"])
        process = target.AttachToProcessWithID(listener, cfg["pid"], error)
        if error.Fail() or not process.IsValid():
            raise RuntimeError(f"exact PID attach failed (no privilege changes attempted): {error}")
        if process.GetProcessID() != cfg["pid"] or process_identity(cfg["pid"]) != {
                k: identity[k] for k in ("pid", "executable", "started")}:
            raise RuntimeError("attached process identity changed")
        modules = [m for m in target.modules if Path(str(m.GetFileSpec())).resolve() == IMAGE]
        if len(modules) != 1 or modules[0].GetUUIDString().upper() != IMAGE_UUID:
            raise RuntimeError("loaded ArcCore module path/UUID mismatch")
        header = modules[0].GetObjectFileHeaderAddress().GetLoadAddress(target)
        if header == lldb.LLDB_INVALID_ADDRESS:
            raise RuntimeError("ArcCore ASLR header is not loaded")
        _hit_limit = cfg["max_hits"]
        _deadline = time.monotonic() + cfg["seconds"]
        for label, (offset, expected) in CHECKPOINTS.items():
            error = lldb.SBError()
            actual = process.ReadMemory(header + offset, 4, error)
            if error.Fail() or actual.hex() != expected:
                raise RuntimeError(f"runtime instruction mismatch at {label}")
            bp = target.BreakpointCreateByAddress(header + offset)
            if not bp.IsValid() or bp.GetNumLocations() != 1:
                raise RuntimeError(f"could not resolve bounded breakpoint {label}")
            _labels[bp.GetID()] = label
            bp.SetScriptCallbackFunction(__name__ + ".checkpoint")
            bp.SetAutoContinue(True)
            emit("breakpoint", checkpoint=label, address=hex(header + offset), offset=hex(offset))
        # Discard the original attach-stop event before watching subsequent
        # stops, otherwise it can look like an unrelated stop after Continue.
        pending = lldb.SBEvent()
        while listener.GetNextEvent(pending):
            pass
        error = process.Continue()
        if error.Fail():
            raise RuntimeError(f"could not resume after installing breakpoints: {error}")
        emit("armed", pid=cfg["pid"], header=hex(header), seconds=cfg["seconds"], max_hits=_hit_limit)
        with Path(str(out) + ".ready.json").open("x") as ready:
            json.dump({"pid": cfg["pid"], "armed": True, "header": hex(header),
                       "deadline_monotonic": _deadline}, ready)
        while time.monotonic() < _deadline and _hits < _hit_limit and not interrupted.is_set():
            event = lldb.SBEvent()
            if not listener.WaitForEvent(1, event):
                continue
            state = lldb.SBProcess.GetStateFromEvent(event)
            if state in (lldb.eStateExited, lldb.eStateDetached):
                emit("process_finished", state=state)
                break
            if state == lldb.eStateStopped and not lldb.SBProcess.GetRestartedFromEvent(event):
                # Auto-continue breakpoint events normally have restarted=True.
                # Never silently continue a crash, signal, or someone else's stop.
                raise RuntimeError("unexpected non-auto-continued target stop")
        status = "interrupted" if interrupted.is_set() else "complete"
    except Exception as exc:
        emit("error", message=str(exc))
        print(f"native checkpoints: {exc}", file=sys.stderr)
    finally:
        if process is not None and process.IsValid():
            if process.GetState() not in (lldb.eStateExited, lldb.eStateDetached):
                if process.GetState() == lldb.eStateRunning:
                    error = process.Stop()
                    emit("cleanup_stop", success=error.Success(), message=str(error))
                if target is not None:
                    target.DeleteAllBreakpoints()
                error = process.Detach(False)
                emit("detach", success=error.Success(), message=str(error), keep_stopped=False)
                if error.Fail():
                    status = "cleanup_unconfirmed"
        emit("finished", status=status, hits=_hits, counts=_counts)
        if _sink is not None:
            _sink.close()
            _sink = None
        for sig, handler in old_handlers.items():
            if handler is not None:
                signal.signal(sig, handler)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--preflight", action="store_true", help="verify only; default")
    mode.add_argument("--attach", action="store_true", help="perform bounded exact-PID observation")
    parser.add_argument("--pid", type=int)
    parser.add_argument("--ready", type=Path)
    parser.add_argument("--out", type=Path)
    parser.add_argument("--seconds", type=int, default=90, choices=range(1, 91), metavar="1..90")
    parser.add_argument("--max-hits", type=int, default=512, choices=range(1, 513), metavar="1..512")
    args = parser.parse_args()
    if (args.pid is None) != (args.ready is None):
        parser.error("--pid and --ready must be supplied together")
    if args.attach and (args.pid is None or args.out is None):
        parser.error("--attach requires --pid, --ready and --out")
    identity = validate(args.pid, args.ready)
    if not args.attach:
        print(json.dumps({"preflight": "verified", "attached": False, **identity}, indent=2))
        return 0
    out = args.out.absolute()
    if not out.is_relative_to(ROOT / "results") or out.resolve() != out or out.parent != Path(identity["ready"]).parent:
        parser.error("--out must be a real path in the same trace156 results directory as --ready")
    for path in (out, Path(str(out) + ".ready.json"), Path(str(out) + ".lldb.log")):
        if path.exists() or path.is_symlink():
            parser.error(f"refusing to overwrite existing output: {path}")
    cfg = {"pid": args.pid, "ready": identity["ready"], "out": str(out),
           "seconds": args.seconds, "max_hits": args.max_hits, "identity": identity}
    env = dict(os.environ, ARC_NATIVE_CHECKPOINT_CONFIG=json.dumps(cfg), PYTHONDONTWRITEBYTECODE="1")
    command = ["xcrun", "lldb", "--batch", "--no-lldbinit", "-o",
               "command script import " + json.dumps(str(Path(__file__).resolve())),
               "-o", "script native_checkpoints.observe(lldb.debugger)",
               "-o", "process detach --keep-stopped false", "-o", "quit"]
    # Child is an attached debugger, never a launcher. No app PID receives a signal.
    with Path(str(out) + ".lldb.log").open("x") as log:
        child = subprocess.Popen(command, env=env, stdout=log, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
        try:
            child.wait(timeout=110)
        except (subprocess.TimeoutExpired, KeyboardInterrupt):
            child.send_signal(signal.SIGINT)  # Cancel the debugger, not the app.
            try:
                child.wait(timeout=8)
            except subprocess.TimeoutExpired:
                print(f"ERROR: debugger {child.pid} did not confirm detach; parent must inspect cleanup. "
                      "No app signal or security change was attempted.", file=sys.stderr)
                return 3
    rows = [json.loads(line) for line in out.read_text().splitlines()] if out.exists() else []
    finished = [row for row in rows if row.get("event") == "finished"]
    if not finished or finished[-1]["status"] != "complete":
        print(f"ERROR: observation incomplete; inspect {out} and {out}.lldb.log", file=sys.stderr)
        return 1
    print(json.dumps(finished[-1], sort_keys=True))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError, subprocess.SubprocessError) as exc:
        print(f"native checkpoints: {exc}", file=sys.stderr)
        sys.exit(1)
