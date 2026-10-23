#!/usr/bin/env python3
"""Lead-only SSH orchestration. Never mutates arcmod-engine or copies credentials."""
import argparse
import fcntl
import json
from pathlib import Path
import re
import shlex
import subprocess
import tarfile
import uuid

ROOT = Path(__file__).resolve().parent
RESEARCH = "arcmod-research.exe.xyz"
ENGINE = "arcmod-engine.exe.xyz"
WORKERS = ("abi", "cef", "arc-layer", "matcher")
SPEC_WORKERS = ("spec-1", "spec-2", "spec-3", "spec-4")
SSH = ["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=15"]


def require_running_engine():
    result=subprocess.run(SSH+["exe.dev","ls --json"],check=True,capture_output=True,text=True)
    vm=next(v for v in json.loads(result.stdout)['vms'] if v['vm_name']=='arcmod-engine')
    if vm['status']!='running':
        raise RuntimeError('arcmod-engine is '+vm['status']+'; refusing SSH or implicit resume')


def remote(host, command, **kwargs):
    if host==ENGINE: require_running_engine()
    return subprocess.run(SSH + [host, command], check=True, **kwargs)


def launch(name, stage):
    job = f"{name}-{stage}"
    command = ("cd /home/exedev/work && "
               f"(nohup bash ./run-agent.sh {shlex.quote(name)} {shlex.quote(stage)} "
               f">logs/launch-{shlex.quote(job)}.log 2>&1 </dev/null &)")
    remote(RESEARCH, command)


def setup():
    remote(RESEARCH, "mkdir -p ~/work/prompts ~/work/logs ~/work/out ~/work/agents")
    assets=("run-agent.sh","validate-stage.py","read-function.py","index-corpus.py",
            "reconcile-completion.py","notify-completion.py","auto-synthesis.sh")
    stage="/home/exedev/work/.harness-"+uuid.uuid4().hex
    remote(RESEARCH, "mkdir "+shlex.quote(stage))
    subprocess.run(["scp", "-q", *[str(ROOT/name) for name in assets],
                    RESEARCH+":"+stage+"/"], check=True)
    for name in assets:
        remote(RESEARCH, "mv "+shlex.quote(stage+"/"+name)+" "+shlex.quote("/home/exedev/work/"+name))
    remote(RESEARCH, "rmdir "+shlex.quote(stage))
    subprocess.run(["scp", "-q", *map(str, sorted((ROOT / "prompts").glob("*.md"))),
                    RESEARCH + ":work/prompts/"], check=True)


def status():
    remote(RESEARCH, "date -u; cat ~/work/STATUS 2>/dev/null; "
           "ps -eo pid,ppid,etime,comm | rg 'codex|run-agent' || true; free -h")
    listing=remote("exe.dev","ls --json",capture_output=True,text=True)
    vm=next(v for v in json.loads(listing.stdout)['vms'] if v['vm_name']=='arcmod-engine')
    if vm['status']=='running':
        remote(ENGINE, "tail -3 ~/work/STATUS; "
               "rg 'DecompileAll:' ~/work/logs/ghidra-decompile.log | tail -3")
    else:
        print('arcmod-engine state:',vm['status'],'(no SSH/resume attempted)')


def collect():
    destination = ROOT / "collected"
    destination.mkdir(exist_ok=True)
    # SQLite is a regenerable working cache and may still be changing mid-stage.
    archive = destination / "research-results.tar.gz"
    with archive.open("wb") as output:
        remote(RESEARCH, "tar czf - --exclude='*.sqlite*' --exclude='*/__pycache__' "
               "--exclude='*/.venv' -C /home/exedev/work agents out STATUS", stdout=output)
    with tarfile.open(archive) as bundle:
        bundle.extractall(destination, filter="data")
    print(destination)


def feed_decompile(extra_only=False):
    """Only ship completed output; select all shards with named Arc methods.

    Includes shards containing Arc source-path references. Main VM is read-only.
    The lead can transfer additional specifically requested shards after review.
    """
    # Serialize local lead invocations. Published inputs are immutable.
    with (ROOT / "feed.lock").open("a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        _feed_decompile(extra_only)


def _feed_decompile(extra_only):
    published = subprocess.run(SSH + [RESEARCH, "test -f ~/work/inputs/decompile/READY"])
    if published.returncode == 0 and not extra_only:
        for name in WORKERS: launch(name, "decompile")
        return
    if published.returncode not in (0, 1):
        raise RuntimeError("Unable to check published input state")
    if extra_only and published.returncode != 0:
        raise RuntimeError("Publish the initial decompile batch first")
    remote(ENGINE,
        "test -n \"$(find ~/work/decomp/arccore-1.170.0 -maxdepth 1 -name '*.c' -print -quit)\" "
        "&& grep -q 'PHASE B DONE' ~/work/STATUS "
        "&& grep -q 'DecompileAll: done=' ~/work/logs/ghidra-decompile.log",
        capture_output=True)
    program = r'''
import pathlib,re,tarfile,sys,hashlib,json,io
w=pathlib.Path('/home/exedev/work')
base=w/'decomp/arccore-1.170.0'
selected=set(EXTRA_SHARDS)
if INCLUDE_ARC:
    for line in (w/'decomp/arccore-1.170.0-functions.tsv').open():
        parts=line.rstrip('\n').split('\t')
        if len(parts)>=4 and re.match(r'^_?Arc[^:]*::',parts[3]):
            selected.add('0x%x.c' % (int(parts[0],16)>>20))
    for p in base.glob('*.c'):
        if p.name in selected: continue
        # Stream lines to avoid holding a large shard in memory.
        with p.open(errors='replace') as f:
            if any('../../arc/' in line or 'arc.mojom.' in line for line in f): selected.add(p.name)
with tarfile.open(fileobj=sys.stdout.buffer,mode='w|gz') as t:
    manifest=[]
    for name in sorted(selected):
        p=base/name
        if not p.is_file():raise RuntimeError('Missing requested shard '+name)
        h=hashlib.sha256()
        with p.open('rb') as f:
            for block in iter(lambda:f.read(1048576),b''):h.update(block)
        manifest.append({'file':name,'bytes':p.stat().st_size,'sha256':h.hexdigest()})
        t.add(p,arcname=name)
    data=json.dumps(manifest).encode()
    info=tarfile.TarInfo('MANIFEST.json');info.size=len(data)
    t.addfile(info,io.BytesIO(data))
'''
    requests = ROOT / "reviewed-shards.txt"
    extra = requests.read_text().splitlines() if requests.exists() else []
    extra = [line.strip() for line in extra if line.strip() and not line.startswith("#")]
    if any(not re.fullmatch(r"0x[0-9a-f]+\.c", name) for name in extra):
        raise ValueError("Invalid reviewed shard name")
    if extra_only and not extra:
        raise ValueError("Supplementary transfer requires reviewed-shards.txt entries")
    program = program.replace("EXTRA_SHARDS", repr(extra)).replace("INCLUDE_ARC", repr(not extra_only))
    batch = "batch-" + uuid.uuid4().hex
    destination = "/home/exedev/work/inputs/decompile"
    if extra_only:
        remote(RESEARCH, "mkdir -p ~/work/inputs/decompile-extra")
        destination = "/home/exedev/work/inputs/decompile-extra/" + batch
    stage = "/home/exedev/work/inputs/.decompile-" + uuid.uuid4().hex
    remote(RESEARCH, "mkdir " + shlex.quote(stage))
    require_running_engine()
    sender = subprocess.Popen(SSH + [ENGINE, "python3 -c " + shlex.quote(program)],
                              stdout=subprocess.PIPE)
    receiver = subprocess.run(SSH + [RESEARCH, "tar xzf - -C " + shlex.quote(stage)],
                              stdin=sender.stdout)
    sender.stdout.close()
    send_rc = sender.wait()
    if send_rc or receiver.returncode:
        raise RuntimeError(f"decompile transfer failed: {send_rc}/{receiver.returncode}")
    verify = '''
import pathlib,json,hashlib,sys,datetime
p=pathlib.Path(sys.argv[1])
manifest=json.loads((p/'MANIFEST.json').read_text())
assert manifest, 'Empty shard manifest'
for entry in manifest:
 f=p/entry['file'];assert f.stat().st_size==entry['bytes']
 h=hashlib.sha256()
 with f.open('rb') as stream:
  for block in iter(lambda:stream.read(1048576),b''):h.update(block)
 assert h.hexdigest()==entry['sha256'],f
(p/'READY').write_text(datetime.datetime.now(datetime.timezone.utc).isoformat())
destination=pathlib.Path(sys.argv[2])
assert not destination.exists(), 'Published destination already exists'
p.rename(destination)
print('Published',len(manifest),'verified shards')
'''
    remote(RESEARCH, "python3 -c " + shlex.quote(verify) + " " + shlex.quote(stage) + " " + shlex.quote(destination))
    if extra_only:
        print("Additional immutable evidence published:", destination)
        print("Lead must explicitly dispatch follow-up analysis of this batch.")
    else:
        for name in WORKERS:
            launch(name, "decompile")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=["setup", "launch-initial", "status", "collect", "feed-decompile", "feed-extra", "launch-quality", "launch-protocols", "launch-specs", "launch-class-reviews", "launch-review"])
    action = parser.parse_args().action
    if action == "setup": setup()
    elif action == "launch-initial":
        for worker in WORKERS: launch(worker, "initial")
    elif action == "status": status()
    elif action == "collect": collect()
    elif action == "feed-decompile": feed_decompile()
    elif action == "feed-extra": feed_decompile(extra_only=True)
    elif action == "launch-quality": launch("quality", "initial")
    elif action == "launch-protocols": launch("protocols", "initial")
    elif action == "launch-specs":
        for worker in SPEC_WORKERS: launch(worker, "specs")
    elif action == "launch-class-reviews":
        for worker in SPEC_WORKERS: launch(worker, "classreview")
    elif action == "launch-review": launch("review", "synthesis")
