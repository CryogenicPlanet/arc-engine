#!/usr/bin/env python3
"""Run a UI fixture command with fresh disposable agent profiles; restore only after exit."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import uuid

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--agents', required=True)
parser.add_argument('--journal', type=Path, required=True)
parser.add_argument('command', nargs=argparse.REMAINDER)
args = parser.parse_args()
agents = args.agents.split(',')
command = args.command[1:] if args.command[:1] == ['--'] else args.command
if not agents or len(set(agents)) != len(agents) or any(a not in {'trace156', 'ours156'} for a in agents) or not command:
    parser.error('exact disposable agents and a command are required')
root = Path(__file__).resolve().parents[2]
lock_pid = int((root / 'logs/arccore-build.lock/pid').read_text())
ancestor = os.getpid()
for _ in range(32):
    if ancestor == lock_pid:
        break
    if ancestor <= 1:
        raise RuntimeError('shared lock ancestry required')
    ancestor = int(subprocess.check_output(['ps', '-p', str(ancestor), '-o', 'ppid='], timeout=2).strip())
else:
    raise RuntimeError('shared lock ancestry required')
if lock_pid <= 1 or ancestor != lock_pid:
    raise RuntimeError('invalid shared lock owner')
base = Path.home() / 'Library/Application Support'
apps = [root.parent / 'arc/build' / ('ArcModAgent-' + agent + '.app') for agent in agents]

def running_pids():
    lines = subprocess.check_output(['ps', '-axo', 'pid=,command='], timeout=3).decode().splitlines()
    found = []
    for line in lines:
        parts = line.strip().split(None, 1)
        if len(parts) != 2:
            continue
        for app in apps:
            exe = str(app / 'Contents/MacOS/Arc')
            prefix = str(app / 'Contents/Frameworks') + '/'
            if parts[1] == exe or parts[1].startswith(exe + ' ') or parts[1].startswith(prefix):
                found.append(int(parts[0]))
                break
    return found

if running_pids():
    raise RuntimeError('refusing profile isolation while exact agent browser/helpers run')
if not args.journal.is_absolute() or args.journal.exists():
    parser.error('journal must be a fresh absolute path')
args.journal.parent.mkdir(parents=True, exist_ok=True, mode=0o700)
nonce = 'arc-ui-profile-' + uuid.uuid4().hex
states = []

def save(status):
    content = {'nonce': nonce, 'status': status, 'profiles': states}
    temporary = args.journal.with_suffix('.tmp')
    fd = os.open(temporary, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
    with os.fdopen(fd, 'w') as stream:
        json.dump(content, stream, indent=2)
    temporary.replace(args.journal)

save('prepared')
code = 1
command_started = False
command_completed = False
try:
    for agent in agents:
        profile = base / ('ArcModAgent-' + agent)
        backup = base / (profile.name + '.ui-probe-' + nonce + '.backup')
        if profile.is_symlink() or backup.exists() or backup.is_symlink() or (profile.exists() and not profile.is_dir()):
            raise RuntimeError('unexpected profile type or backup collision')
        state = {'agent': agent, 'path': str(profile), 'backup': str(backup), 'existed': profile.exists(), 'phase': 'prepared'}
        if state['existed']:
            original = profile.stat()
            state.update(original_device=original.st_dev, original_inode=original.st_ino)
        states.append(state); save('isolating')
        if state['existed']:
            state['phase'] = 'backing-up'; save('isolating')
            profile.rename(backup)
        state['phase'] = 'original-backed-up'; save('isolating')
        profile.mkdir(mode=0o700)
        info = profile.stat()
        state.update(device=info.st_dev, inode=info.st_ino, phase='isolated'); save('isolating')
        marker = profile / '.arc-ui-probe-profile.json'
        marker.write_text(json.dumps({'nonce': nonce, 'agent': agent, 'device': info.st_dev, 'inode': info.st_ino}))
        marker.chmod(0o600); save('isolated')
    env = dict(os.environ, ARC_UI_PROFILE_ISOLATION_NONCE=nonce)
    command_started = True
    result = subprocess.run(command, env=env, timeout=240)
    code = result.returncode
    command_completed = code >= 0
finally:
    if command_started and not command_completed:
        save('preserved-incomplete-command')
        raise RuntimeError('fixture command interrupted; preserving profiles/backups for reconciliation')
    pids = running_pids()
    if pids:
        save('preserved-running-processes')
        raise RuntimeError('agent processes remain; preserving profiles and backups: ' + repr(pids))
    for state in reversed(states):
        profile, backup = Path(state['path']), Path(state['backup'])
        if state['phase'] in {'prepared', 'backing-up'}:
            if not backup.exists() and not backup.is_symlink():
                unchanged = not profile.exists() and not profile.is_symlink() if not state['existed'] else (
                    not profile.is_symlink() and profile.is_dir() and
                    (profile.stat().st_dev, profile.stat().st_ino) == (state['original_device'], state['original_inode']))
                if not unchanged:
                    save('preserved-incomplete-backup'); raise RuntimeError('incomplete backup identity unresolved')
                state['phase'] = 'restored'; save('restoring'); continue
            if profile.exists() or profile.is_symlink() or not state['existed']:
                save('preserved-incomplete-backup'); raise RuntimeError('incomplete backup identity unresolved')
            state['phase'] = 'original-backed-up'
        if state['existed']:
            if backup.is_symlink() or not backup.is_dir():
                save('preserved-backup-identity-change'); raise RuntimeError('original profile backup missing or unsafe')
            original = backup.stat()
            if (original.st_dev, original.st_ino) != (state['original_device'], state['original_inode']):
                save('preserved-backup-identity-change'); raise RuntimeError('original profile backup identity changed')
        elif backup.exists() or backup.is_symlink():
            save('preserved-unexpected-backup'); raise RuntimeError('unexpected backup appeared')
        if state['phase'] == 'original-backed-up':
            # No fresh directory was created; restore the untouched original.
            if profile.exists() or profile.is_symlink():
                save('preserved-unexpected-profile'); raise RuntimeError('unexpected profile appeared during incomplete isolation')
        else:
            if profile.is_symlink() or not profile.is_dir():
                save('preserved-profile-identity-change'); raise RuntimeError('isolated profile identity changed')
            info = profile.stat()
            marker = profile / '.arc-ui-probe-profile.json'
            if (info.st_dev, info.st_ino) != (state['device'], state['inode']) or marker.is_symlink() or json.loads(marker.read_text()).get('nonce') != nonce:
                save('preserved-profile-identity-change'); raise RuntimeError('isolated profile marker/inode changed')
            shutil.rmtree(profile)
        if state['existed']:
            if backup.is_symlink() or not backup.is_dir():
                save('preserved-backup-identity-change'); raise RuntimeError('original profile backup missing or unsafe')
            backup.rename(profile)
        state['phase'] = 'restored'; save('restoring')
    save('restored')
raise SystemExit(code)
