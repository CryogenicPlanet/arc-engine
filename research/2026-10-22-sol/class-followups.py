#!/usr/bin/env python3
"""Versioned Sol corrections, fresh review and a fail-closed synthesis barrier.

Runs only on the existing research VM. No SSH, VM management or credentials.
Original specs/reviews remain immutable. Failed jobs require lead inspection.
"""
import argparse
import csv
import datetime
import fcntl
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time

ROOT = Path('/home/exedev/work')
ROLES = tuple(f'spec-{i}' for i in range(1, 5))


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''):
            h.update(block)
    return h.hexdigest()


def inventory(directory):
    return {str(p.relative_to(ROOT)): digest(p) for p in sorted(directory.rglob('*'))
            if p.is_file() and '__pycache__' not in p.parts}


def rows(path):
    with path.open(newline='') as stream:
        return list(csv.DictReader(stream, delimiter='\t'))


def previous(role):
    return f'spec-{(int(role[-1]) - 2) % 4 + 1}'


def specdir(role):
    return ROOT / 'agents' / role / 'revisions/v1'


def reviewdir(role):
    return ROOT / 'agents' / role / 'revision-reviews/v1'


def jobname(role, phase):
    return role + '-revision-v1' + ('-review' if phase == 'review' else '')


def stamp(job, state):
    now = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')
    with (ROOT / 'STATUS').open('a') as stream:
        stream.write(f'{now} JOB {job} {state}\n')


def inputs(role, phase):
    result = inventory(ROOT / 'agents' / role / 'specs')
    result.update(inventory(ROOT / 'agents' / previous(role) / 'classreview'))
    if phase == 'review':
        result.update(inventory(specdir(role)))
    result['class-followups.py'] = digest(Path(__file__))
    result['inputs/context/class-assignments.json'] = digest(ROOT / 'inputs/context/class-assignments.json')
    return result


def expected_classes(role):
    return set(json.loads((ROOT / 'inputs/context/class-assignments.json').read_text())['groups'][role]['classes'])


def method_ids(path):
    records = rows(path)
    fields = ('class_name', 'category', 'kind', 'selector', 'type_encoding')
    identities = [tuple(r[f] for f in fields) + (int(r['imp_va'], 0),) for r in records]
    assert len(identities) == len(set(identities))
    return set(identities)


def validate(role, phase):
    original = ROOT / 'agents' / role / 'specs'
    directory = specdir(role) if phase == 'correct' else reviewdir(role)
    findings = rows(ROOT / 'agents' / previous(role) / 'classreview/findings.tsv')
    expected_rows = {str(i) for i in range(1, len(findings) + 1)}
    assert (directory / 'report.md').stat().st_size > 0
    if phase == 'correct':
        assert {p.stem for p in (directory / 'classes').glob('*.md') if p.stat().st_size} == expected_classes(role)
        assert method_ids(directory / 'methods.tsv') == method_ids(original / 'methods.tsv')
        ledger = rows(directory / 'corrections.tsv')
        actual = [r['finding_row'] for r in ledger]
        assert len(actual) == len(set(actual)) and set(actual) == expected_rows
        assert all(r.get('evidence', '').strip() and r.get('disposition', '').strip() for r in ledger)
        # Existing raw evidence must not be silently regenerated to fit the prose.
        for name in ('bodies', 'evidence', 'sources'):
            for path in (original / name).rglob('*'):
                if path.is_file() and '__pycache__' not in path.parts:
                    assert digest(path) == digest(directory / path.relative_to(original)), path
    else:
        ledger = rows(directory / 'finding-dispositions.tsv')
        actual = [r['finding_row'] for r in ledger]
        assert len(actual) == len(set(actual)) and set(actual) == expected_rows
        assert all(r.get('status', '').strip() and r.get('evidence', '').strip() for r in ledger)
        classes = [r['class_name'] for r in rows(directory / 'class-review.tsv')]
        assert len(classes) == len(set(classes)) and set(classes) == expected_classes(role)
        assert (directory / 'findings.tsv').is_file()
        assert all(r['severity'].strip().lower() in ('high', 'medium', 'low')
                   for r in rows(directory / 'findings.tsv')), 'Unknown finding severity'


def prompt(role, phase):
    original = ROOT / 'agents' / role / 'specs'
    initial_review = ROOT / 'agents' / previous(role) / 'classreview'
    target = specdir(role) if phase == 'correct' else reviewdir(role)
    common = f'''You are a fresh GPT-6.1-Sol/high research session on arcmod-research, included 2-CPU/4-GiB pool.
Do not spawn agents, use credentials/integrations other than LLM, access other VMs, build/install packages,
change native apps/profiles, send messages, push or commit. Main engine VM is PAUSED: never SSH/resume it.
Read inputs under {ROOT}/inputs without mutation. Stay below one CPU/~500 MiB; heavy scans use heavy.lock.
Use {ROOT}/read-function.py for bounded original engine bodies and copied binaries for segment-aware disassembly.
All addresses need image/version/unslid-VA identity. Chromium pin d0c02bfad08ec92a6ffcf989e53cd995e6381a4c;
CEF pin 426920c33ef22551e0b8f48cf343d623ac25642d. Decompilation is evidence, not a sound ABI or behavior guarantee.
834 counts declarations on imported classes, not proven calls. Do not sum Ghidra enclosing spans or infer unmatched=proprietary.
Read {ROOT}/inputs/context/lead-review.md, {ROOT}/inputs/context/body-evidence-review.md
and {ROOT}/inputs/context/acceptance-evidence-review.md.
Writer: {role}. Frozen original specs: {original}. Completed original independent review: {initial_review}.
Read that review's report.md, findings.tsv, class-review.tsv and relevant actual body/instruction/source evidence.
Number findings by DATA ROW starting at 1, preserving every original row. Never edit original specs/review/shared inputs/harness.
Only write beneath {target}. All other directories are read-only. Do not run old reproduction scripts that write original outputs.
Do not claim runtime compatibility, complete callees, main-thread delivery or teardown suppression without evidence.
'''
    if phase == 'correct':
        return common + f'''
Correct every review finding in the seeded full copy at {target}. Keep all assigned class files and all 491
declared method identities/types/IMPs exactly; preserve categories and kind. Edit affected class prose, methods.tsv annotations
and coverage.json counts consistently. Preserve copied raw bodies/evidence/sources byte-for-byte. New bounded evidence may
be added separately, with provenance, if needed. Relative links from classes to ../bodies and ../evidence still work.
Prefer removing unsupported assertions and recording exact unknowns over guessing a replacement behavior.
Recheck the problematic bodies and relevant cross-report binary observations yourself; do not accept reviewer prose alone.
Write corrections.tsv with columns finding_row,class_name,selector,disposition,before,after,evidence,remaining_unknown.
Every original finding needs exactly one evidence-backed row, even if no change is justified. Also scan nearby analogous
descriptions for the same error and record any extra changes in report.md. New report.md must summarize corrections,
coverage (classes/method identities), validation, scope and remaining contract gaps. Do not regenerate original tools
or change input artifacts to make validation pass. This is a corrected research specification, not implementation work.
'''
    return common + f'''
Freshly review the corrected full class group at {specdir(role)} without editing it. Compare its corrections.tsv to every
original finding and verify actual exact bodies/binary/source citations. Review all changed prose and annotations, and
spot-check analogous methods. Check full assigned class/method coverage and original evidence immutability.
Write finding-dispositions.tsv: finding_row,status,evidence,remaining_unknown. Status is resolved, resolved_with_limits,
or unresolved_claim; each finding needs one row. An unknown behavior clearly stated as unknown may be resolved_with_limits.
Write class-review.tsv with class_name,status,evidence,limits, exactly one row per assigned class.
Write findings.tsv (severity,class_name,selector,claim,evidence,required_correction) for remaining/new problems; header-only
is allowed if none. Use high/medium for material inaccurate/unsupported assertions, low for minor presentation issues.
Write report.md with coverage, evidence checks, original-finding dispositions and publishability limitations. This is a
fresh independent research review, not runtime testing or a claim every method's transitive behavior is recovered.
'''


def worker(role, phase):
    job = jobname(role, phase)
    with (ROOT / (role + '.lock')).open('a') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return
        assert (ROOT / (previous(role) + '-classreview.done')).exists()
        if phase == 'review':
            assert (ROOT / (jobname(role, 'correct') + '.done')).exists()
        if (ROOT / (job + '.done')).exists():
            return
        identity_path = ROOT / (job + '.run.json')
        assert not identity_path.exists(), 'Prior attempt exists: inspect; use a new explicit version, never blind retry'
        target = specdir(role) if phase == 'correct' else reviewdir(role)
        assert not target.exists(), 'Never overwrite a revision directory'
        if phase == 'correct':
            shutil.copytree(ROOT / 'agents' / role / 'specs', target, ignore=shutil.ignore_patterns('__pycache__'))
            (target / 'report.md').rename(target / 'original-report.md')
        else:
            target.mkdir(parents=True)
        before = inputs(role, phase)
        text = prompt(role, phase)
        prompt_path = ROOT / 'prompts' / (job + '.md')
        assert not prompt_path.exists()
        prompt_path.write_text(text)
        before[str(prompt_path.relative_to(ROOT))] = digest(prompt_path)
        identity_path.write_text(json.dumps(before, indent=2) + '\n')
        (ROOT / (job + '.pid')).write_text(str(os.getpid()) + '\n')
        stamp(job, 'START')
        argv = ['nice', '-n', '10', 'timeout', '--signal=TERM', '--kill-after=30s', '90m',
                'codex', 'exec', '--ignore-user-config', '--skip-git-repo-check',
                '--dangerously-bypass-approvals-and-sandbox', '--disable', 'multi_agent',
                '-C', str(target), '-m', 'gpt-6.1-sol', '-c', 'model_reasoning_effort=high',
                '-c', 'model_provider=exe-llm', '-c', 'model_providers.exe-llm.name="exe-llm"',
                '-c', 'model_providers.exe-llm.base_url="https://llm.int.exe.xyz/v1"',
                '-c', 'model_providers.exe-llm.wire_api="responses"',
                '-c', 'model_providers.exe-llm.requires_openai_auth=false', '--json',
                '-o', str(ROOT / 'out' / (job + '.md')), '-']
        try:
            started = time.time()
            log = ROOT / 'logs' / (job + '.jsonl')
            with log.open('w') as output:
                result = subprocess.run(argv, input=text, text=True, stdout=output, stderr=subprocess.STDOUT)
            assert result.returncode == 0, f'CLI exit {result.returncode}'
            events = []
            for line in log.open():
                try:
                    event = json.loads(line)
                except ValueError:
                    continue
                if event.get('type') in ('turn.completed', 'turn.failed', 'error'):
                    events.append(event)
            assert events and events[-1]['type'] == 'turn.completed', 'No terminal turn.completed'
            assert (target / 'report.md').stat().st_mtime >= started
            assert (ROOT / 'out' / (job + '.md')).stat().st_size > 0
            after = inputs(role, phase)
            after[str(prompt_path.relative_to(ROOT))] = digest(prompt_path)
            assert before == after, 'Frozen input changed during job'
            validate(role, phase)
            (ROOT / (job + '.completion.json')).write_text(json.dumps({
                'inputs': before, 'outputs': inventory(target), 'terminal_event': events[-1]}, indent=2) + '\n')
            (ROOT / (job + '.done')).touch()
            stamp(job, 'DONE')
        except Exception as error:
            stamp(job, 'FAILED ' + str(error))
            raise


def launch(role, phase):
    job = jobname(role, phase)
    if (ROOT / (job + '.run.json')).exists():
        return
    with (ROOT / 'logs' / ('launch-' + job + '.log')).open('a') as log:
        subprocess.Popen([sys.executable, str(Path(__file__).resolve()), 'worker', role, phase],
                         stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)


def publish():
    selected = []
    for role in ROLES:
        for phase in ('correct', 'review'):
            validate(role, phase)
            proof = json.loads((ROOT / (jobname(role, phase) + '.completion.json')).read_text())
            directory = specdir(role) if phase == 'correct' else reviewdir(role)
            assert proof['outputs'] == inventory(directory), 'Completed output changed'
            current = inputs(role, phase)
            key = 'prompts/' + jobname(role, phase) + '.md'
            current[key] = digest(ROOT / key)
            assert proof['inputs'] == current, 'Completed input changed'
        review = reviewdir(role)
        assert not [r for r in rows(review / 'findings.tsv') if r['severity'].strip().lower() in ('high', 'medium')], role + ': material review findings need lead'
        assert all(r['status'] in ('resolved', 'resolved_with_limits') for r in rows(review / 'finding-dispositions.tsv')), role + ': unresolved original finding'
        selected.append({'writer': role, 'classes': sorted(expected_classes(role)),
                         'specs': str(specdir(role)), 'review': str(review),
                         'spec_sha256': inventory(specdir(role)), 'review_sha256': inventory(review)})
    assert len({c for item in selected for c in item['classes']}) == 146
    assert sum(len(method_ids(specdir(role) / 'methods.tsv')) for role in ROLES) == 1964
    data = json.dumps({'version': 1, 'classes': 146, 'declared_methods': 1964, 'selected': selected}, indent=2) + '\n'
    temp = ROOT / '.CLASS-REVISIONS-READY.tmp'
    temp.write_text(data)
    temp.replace(ROOT / 'CLASS-REVISIONS-READY')


def watch():
    with (ROOT / 'class-followups.lock').open('a') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return
        (ROOT / 'class-followups.pid').write_text(str(os.getpid()) + '\n')
        deadline = time.monotonic() + 21600
        try:
            while time.monotonic() < deadline:
                status = (ROOT / 'STATUS').read_text()
                watched = [jobname(role, phase) for role in ROLES for phase in ('correct', 'review')]
                for job in watched:
                    if (ROOT / (job + '.run.json')).exists() and not (ROOT / (job + '.done')).exists():
                        pidfile = ROOT / (job + '.pid')
                        assert pidfile.exists() and (Path('/proc') / pidfile.read_text().strip()).exists(), 'Worker disappeared: ' + job
                watched += [role + '-classreview' for role in ROLES] + ['protocols-initial']
                assert not any('JOB ' + job + ' FAILED' in status for job in watched), 'Predecessor failed: inspect logs, no duplicate launch'
                for role in ROLES:
                    if (ROOT / (previous(role) + '-classreview.done')).exists():
                        launch(role, 'correct')
                    if (ROOT / (jobname(role, 'correct') + '.done')).exists():
                        launch(role, 'review')
                if all((ROOT / (jobname(role, 'review') + '.done')).exists() for role in ROLES):
                    publish()
                    (ROOT / 'class-followups.done').touch()
                    stamp('class-followups', 'DONE')
                    return
                time.sleep(30)
            raise TimeoutError('Six-hour revision deadline; inspect state')
        except Exception as error:
            stamp('class-followups', 'FAILED ' + str(error))
            raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=('watch', 'worker'))
    parser.add_argument('role', choices=ROLES, nargs='?')
    parser.add_argument('phase', choices=('correct', 'review'), nargs='?')
    args = parser.parse_args()
    if args.action == 'watch':
        watch()
    else:
        assert args.role and args.phase
        try:
            worker(args.role, args.phase)
        except Exception as error:
            stamp(jobname(args.role, args.phase), 'FAILED ' + str(error))
            raise
