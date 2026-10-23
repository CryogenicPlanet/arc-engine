#!/usr/bin/env python3
"""Compare actual canonical page/control outcomes with and without recording."""
import argparse
import json
from pathlib import Path

FIELDS = (
    'operation', 'stage', 'actionAttempted', 'textEditAttempted', 'actionAttemptCount',
    'textEditAttemptCount', 'nativeActionsPerformed', 'performClickReturned',
    'setterReturned', 'textReadbackMatchesFixedReply', 'initialMatchesExpected', 'phase',
    'refusalReason', 'selectedButtonTitle', 'nativeActionKind', 'nativeMethodReturned', 'accessibilityPressReturn', 'causalProfileIsolated', 'causalFixtureVerified',
)

def load_run(root, agent, page_names):
    run = json.loads((root / agent / 'launch-1/run.json').read_text())
    receipts = [{key: row.get(key) for key in FIELDS}
                for row in run['receipts'] if row['operation'] not in {'inventory', 'inspect-site-settings', 'snapshot-baseline', 'inspect-dismissed'}]
    page_steps = [row for row in run['steps'] if row['step'] in page_names]
    return run, receipts, page_steps

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--unrecorded', type=Path, required=True)
    parser.add_argument('--recorded', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--scenario', choices=['dialogs', 'dialogs-beforeunload', 'dialogs-auth-cancel', 'permissions-deny', 'cookies-storage-site-settings', 'context-menus'], default='dialogs')
    args = parser.parse_args()
    if args.out.exists():
        parser.error('output already exists')
    expected = {'dialogs': ['alert', 'confirm', 'prompt'], 'dialogs-beforeunload': ['beforeunload-stay', 'beforeunload-leave'], 'dialogs-auth-cancel': ['auth-cancel'], 'permissions-deny': ['deny'], 'cookies-storage-site-settings': ['deny', 'allow', 'site-settings'], 'context-menus': ['context-reload']}[args.scenario]
    page_names = {'dialogs': {'alert native completion', 'confirm cancel', 'prompt native text and completion'}, 'dialogs-beforeunload': {'stay cancels navigation', 'leave commits navigation'}, 'dialogs-auth-cancel': {'cancel displays fixed 401 response'}, 'permissions-deny': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion'}, 'cookies-storage-site-settings': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion', 'second origin starts with undecided permission', 'allow notification prompt', 'site settings reflects insecure HTTP fixture'}, 'context-menus': {'new native page context menu outside menu bar', 'contextual reload commits', 'context menu dismissed'}}[args.scenario]
    expected_actions = {'dialogs': ['press-alert-ok', 'press-confirm-cancel', 'set-prompt-text', 'press-prompt-ok'], 'dialogs-beforeunload': ['press-beforeunload-cancel', 'press-beforeunload-ok'], 'dialogs-auth-cancel': ['press-auth-cancel'], 'permissions-deny': ['press-deny'], 'cookies-storage-site-settings': ['press-deny', 'press-allow', 'press-site-settings'], 'context-menus': ['press-reload']}[args.scenario]
    results = []
    for agent in ['trace156', 'ours156']:
        plain, plain_actions, plain_steps = load_run(args.unrecorded, agent, page_names)
        recorded, recorded_actions, recorded_steps = load_run(args.recorded, agent, page_names)
        coverage = recorded.get('coverage') or {}
        plain_artifacts = plain.get('artifactBefore') or {}
        recorded_artifacts = recorded.get('artifactBefore') or {}
        shared_paths = set(plain_artifacts) & set(recorded_artifacts)
        # Recorder exists only in the recorded mode. Main, engine and helper
        # must be the identical bytes across both controlled runs.
        matching_pair_artifacts = len(plain_artifacts) == 3 and len(shared_paths) == 3 and all(
            plain_artifacts[path] == recorded_artifacts[path] for path in shared_paths)
        checks = {
            'unrecorded_pages_complete': plain['completedStages'] == expected,
            'recorded_pages_complete': recorded['completedStages'] == expected,
            'page_assertions_present_and_pass': len(plain_steps) == len(recorded_steps) == len(page_names)
                and all(row['outcome'] == 'pass' for row in plain_steps + recorded_steps),
            'page_outcomes_equal': plain_steps == recorded_steps,
            'expected_real_control_receipts': [row['operation'] for row in plain_actions] == [row['operation'] for row in recorded_actions] == expected_actions,
            'control_receipts_equal': plain_actions == recorded_actions,
            'no_uncertain_mutations': not plain.get('uncertainMutation') and not recorded.get('uncertainMutation'),
            'stable_unrecorded_artifacts': plain.get('artifactsStable') is True,
            'stable_recorded_artifacts': recorded.get('artifactsStable') is True,
            'same_main_engine_helper_across_modes': matching_pair_artifacts,
            'causal_isolation_when_required': args.scenario == 'dialogs' or bool(plain.get('profileIsolation') and recorded.get('profileIsolation')),
            'recorder_valid': not coverage.get('errors', ['missing coverage']) and coverage.get('callCount', 0) > 0,
        }
        results.append({'agent': agent, 'checks': checks, 'pass': all(checks.values()),
                        'unrecorded_cleanup': plain.get('cleanup'), 'recorded_cleanup': recorded.get('cleanup'),
                        'limitation': 'Independent page/control noninterference only. Browser shutdown, full boundary parity and visual acceptance remain separate.'})
    report = {'pass': all(row['pass'] for row in results), 'runs': results}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2))
    print(json.dumps(report, indent=2))
    raise SystemExit(0 if report['pass'] else 1)

if __name__ == '__main__':
    main()
