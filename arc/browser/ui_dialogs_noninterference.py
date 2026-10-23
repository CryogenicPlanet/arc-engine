#!/usr/bin/env python3
"""Compare actual canonical page/control outcomes with and without recording."""
import argparse
import json
from pathlib import Path

FIELDS = (
    'operation', 'stage', 'actionAttempted', 'textEditAttempted', 'actionAttemptCount',
    'textEditAttemptCount', 'nativeActionsPerformed', 'performClickReturned',
    'setterReturned', 'textReadbackMatchesFixedReply', 'initialMatchesExpected', 'phase',
    'normalActionAttemptCount', 'revealAttemptCount', 'intentPersisted', 'revealAttemptBudgetConsumed',
    'revealFirstInventoryCaptured', 'revealFirstInventoryEligible',
    'refusalReason', 'selectedButtonTitle', 'selectedButtonLabel', 'nativeActionKind', 'nativeMethodReturned', 'accessibilityPressReturn', 'causalProfileIsolated', 'causalFixtureVerified',
)

def load_run(root, agent, page_names):
    run = json.loads((root / agent / 'launch-1/run.json').read_text())
    receipts = [{key: row.get(key) for key in FIELDS}
                for row in run['receipts'] if row['operation'] not in {'inventory', 'inspect-site-settings', 'snapshot-baseline', 'inspect-dismissed'}]
    page_steps = [row for row in run['steps'] if row['step'] in page_names]
    return run, receipts, page_steps


def context_lifecycle_complete(run):
    """Require each run's own menu lifecycle; sequence IDs are local to a process."""
    terminal = [row for row in run.get('receipts', []) if row.get('operation') == 'inspect-dismissed']
    if not terminal:
        return False
    row = terminal[-1]
    begin, attempt, end = (row.get(key) for key in (
        'attemptedTrackingMenuBeginSequence', 'nativeAttemptSequence', 'matchingTrackingEndSequence'))
    if not all(type(value) is int for value in (begin, attempt, end)) or not 0 < begin < attempt < end:
        return False
    return all(row.get(key) is True for key in (
        'trackingObserverInstalled', 'trackingComplete', 'matchingTrackingEndObserved',
        'trackingEndedAfterAttempt', 'postAXComplete', 'postAXNoExtras')) and all(
        type(row.get(key)) is int and row[key] == value for key, value in {
            'actionAttemptCount': 1, 'nativeActionsPerformed': 1, 'newTrackingBeginCount': 1,
            'activeTrackingMenuCount': 0, 'newTrackingMenuCount': 0, 'trackingReloadControlCount': 0,
            'newContextMenuCount': 0, 'reloadControlCount': 0, 'newNativeWindowCount': 0,
        }.items()) and row.get('truncated') is False


def permission_reveal_complete(run):
    """A reveal is accepted only with its one native attempt and later denial."""
    actions = [row for row in run.get('receipts', []) if row.get('operation') not in {
        'inventory', 'inspect-site-settings', 'snapshot-baseline', 'inspect-dismissed'}]
    reveals = [row for row in actions if row.get('operation') == 'press-prompt-site-center']
    if not reveals:
        return True
    if len(reveals) != 1 or actions[0] is not reveals[0]:
        return False
    row = reveals[0]
    if row.get('stage') != 'deny' or row.get('phase') != 'deny' or row.get('refusalReason'):
        return False
    if not all(row.get(key) is True for key in (
        'actionAttempted', 'nativeMethodReturned', 'intentPersisted', 'revealAttemptBudgetConsumed',
        'causalProfileIsolated', 'causalFixtureVerified', 'revealFirstInventoryCaptured', 'revealFirstInventoryEligible')):
        return False
    if not all(type(row.get(key)) is int and row[key] == value for key, value in {
        'actionAttemptCount': 1, 'nativeActionsPerformed': 1, 'normalActionAttemptCount': 0, 'revealAttemptCount': 1}.items()):
        return False
    kind = row.get('nativeActionKind')
    if kind not in {'performClick', 'accessibilityPerformPress'}:
        return False
    if kind == 'accessibilityPerformPress' and type(row.get('accessibilityPressReturn')) is not bool:
        return False
    before = row.get('before') or {}
    if before.get('truncated') is not False or not all(type(before.get(key)) is int and before[key] == value for key, value in {
        'matchingGroupCount': 0, 'promptSiteCenterButtonCount': 1,
        'actionAttemptCount': 0, 'nativeActionsPerformed': 0, 'normalActionAttemptCount': 0, 'revealAttemptCount': 0}.items()):
        return False
    first_id = row.get('revealFirstInventoryID')
    first = [receipt for receipt in run.get('receipts', []) if receipt.get('id') == first_id and receipt.get('operation') == 'inventory']
    if not isinstance(first_id, str) or len(first) != 1:
        return False
    if not all(first[0].get(key) is True for key in ('revealFirstInventoryCaptured', 'revealFirstInventoryEligible')):
        return False
    receipts = run.get('receipts', [])
    first_inventory_index = next((index for index, receipt in enumerate(receipts) if receipt.get('operation') == 'inventory'), None)
    reveal_index = next((index for index, receipt in enumerate(receipts) if receipt is row), None)
    if first_inventory_index is None or reveal_index is None or first_inventory_index >= reveal_index or receipts[first_inventory_index] is not first[0]:
        return False
    if first[0].get('stage') != 'deny' or first[0].get('truncated') is not False or not all(
        type(first[0].get(key)) is int and first[0][key] == value for key, value in {
            'matchingGroupCount': 0, 'promptSiteCenterButtonCount': 1,
            'actionAttemptCount': 0, 'nativeActionsPerformed': 0, 'normalActionAttemptCount': 0, 'revealAttemptCount': 0}.items()):
        return False
    for index, action in enumerate(actions[1:], start=1):
        if not all(type(action.get(key)) is int and action[key] == value for key, value in {
            'actionAttemptCount': index + 1, 'nativeActionsPerformed': index + 1,
            'normalActionAttemptCount': index, 'revealAttemptCount': 1}.items()):
            return False
    if len(actions) < 2 or actions[1].get('operation') != 'press-deny':
        return False
    denial = actions[1]
    qualified = denial.get('before') or {}
    if denial.get('actionAttempted') is not True or denial.get('nativeMethodReturned') is not True or denial.get('selectedButtonLabel') != 'Not Now':
        return False
    if qualified.get('truncated') is not False or not all(type(qualified.get(key)) is int and qualified[key] == 1 for key in (
        'matchingGroupCount', 'exactMessageCount', 'notificationsIconCount')):
        return False
    buttons = qualified.get('expectedButtonCounts') or {}
    if not all(type(buttons.get(key)) is int and buttons[key] == 1 for key in ('Not Now', 'Allow')):
        return False
    deny_steps = [step for step in run.get('steps', []) if step.get('step') == 'deny notification prompt']
    return (len(deny_steps) == 1 and deny_steps[0].get('outcome') == 'pass' and not run.get('uncertainMutation')
            and run.get('revealPromptObserved') is True and run.get('revealOutcomeConfirmed') is True and run.get('revealUncertain') is False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--unrecorded', type=Path, required=True)
    parser.add_argument('--recorded', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--scenario', choices=['dialogs', 'dialogs-beforeunload', 'dialogs-auth-cancel', 'permissions-deny', 'cookies-storage-site-settings', 'context-menus'], default='dialogs')
    parser.add_argument('--prompt-reveal', action='store_true', help='allow one evidenced native Site Control Center reveal before canonical denial')
    args = parser.parse_args()
    if args.prompt_reveal and args.scenario not in {'permissions-deny', 'cookies-storage-site-settings'}:
        parser.error('--prompt-reveal is only valid for the two permission scenarios')
    if args.out.exists():
        parser.error('output already exists')
    expected = {'dialogs': ['alert', 'confirm', 'prompt'], 'dialogs-beforeunload': ['beforeunload-stay', 'beforeunload-leave'], 'dialogs-auth-cancel': ['auth-cancel'], 'permissions-deny': ['deny'], 'cookies-storage-site-settings': ['deny', 'allow', 'site-settings'], 'context-menus': ['context-reload']}[args.scenario]
    page_names = {'dialogs': {'alert native completion', 'confirm cancel', 'prompt native text and completion'}, 'dialogs-beforeunload': {'stay cancels navigation', 'leave commits navigation'}, 'dialogs-auth-cancel': {'cancel displays fixed 401 response'}, 'permissions-deny': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion'}, 'cookies-storage-site-settings': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion', 'second origin starts with undecided permission', 'allow notification prompt', 'site settings reflects insecure HTTP fixture'}, 'context-menus': {'new native page context menu outside menu bar', 'contextual reload commits', 'context menu tracking ended'}}[args.scenario]
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
            'expected_real_control_receipts': [row['operation'] for row in plain_actions] == [row['operation'] for row in recorded_actions]
                and [row['operation'] for row in plain_actions] in ([expected_actions, ['press-prompt-site-center'] + expected_actions] if args.prompt_reveal else [expected_actions]),
            'permission_reveal_complete': not args.prompt_reveal or (permission_reveal_complete(plain) and permission_reveal_complete(recorded)),
            'control_receipts_equal': plain_actions == recorded_actions,
            'context_lifecycle_complete': args.scenario != 'context-menus' or (context_lifecycle_complete(plain) and context_lifecycle_complete(recorded)),
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
