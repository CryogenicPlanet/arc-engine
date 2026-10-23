#!/usr/bin/env python3
"""Compare actual canonical page/control outcomes with and without recording."""
import argparse
import json
from pathlib import Path

KNOWN_AGENTS = ('trace156', 'ours156')


def parse_agents(value):
    agents = value.split(',')
    if not agents or any(agent not in KNOWN_AGENTS for agent in agents) or len(set(agents)) != len(agents):
        raise argparse.ArgumentTypeError('--agents requires a nonempty, duplicate-free comma-separated subset of trace156,ours156')
    return agents


FIELDS = (
    'operation', 'stage', 'actionAttempted', 'textEditAttempted', 'actionAttemptCount',
    'textEditAttemptCount', 'nativeActionsPerformed', 'performClickReturned',
    'setterReturned', 'textReadbackMatchesFixedReply', 'initialMatchesExpected', 'phase',
    'normalActionAttemptCount', 'revealAttemptCount', 'intentPersisted', 'revealAttemptBudgetConsumed',
    'revealFirstInventoryCaptured', 'revealFirstInventoryEligible',
    'refusalReason', 'selectedButtonTitle', 'selectedButtonLabel', 'nativeActionKind', 'nativeMethodReturned', 'accessibilityPressReturn', 'causalProfileIsolated', 'causalFixtureVerified',
)


PREPARATION_READ_ONLY_OPERATIONS = {'inventory-native-window', 'inspect-native-window'}
PREPARATION_OPERATIONS = PREPARATION_READ_ONLY_OPERATIONS | {'prepare-native-window'}
PREPARATION_FIELDS = (
    'preparationInvocationCount', 'totalPhysicalActions', 'preparationBudgetConsumed',
    'preparationIntentPersisted', 'preparationAlreadyReadyBefore',
)
ACTIVATION_FIELDS = (
    'activationInvocationCount', 'activationBudgetConsumed', 'activationIntentPersisted',
    'activationAlreadyActiveBefore',
)
WINDOW_STATE_FIELDS = (
    'candidateWindowKnown', 'sameCandidateControl', 'sameCandidateWindow', 'windowVisible',
    'canBecomeKeyWindow', 'canBecomeMainWindow', 'windowIsKey', 'windowIsMain',
    'appKeyWindowMatches', 'appMainWindowMatches', 'keyOrMainReady', 'candidateScopeValid',
    'applicationActive', 'applicationHidden', 'truncated', 'uniqueSiteCenterCount',
)


def preparation_state_projection(state):
    """Compare fixed observations only; PID/time/opaque identities stay per-run."""
    return {key: state.get(key) for key in WINDOW_STATE_FIELDS} if isinstance(state, dict) else None


def preparation_comparison(run, native_app_activation=False):
    """No request IDs, elapsed times, foreground PIDs or window pointers compare."""
    receipts = run.get('receipts', [])
    inspections = [row for row in receipts if row.get('operation') == 'inspect-native-window']
    operations = {'inventory-native-window', 'prepare-native-window'}
    fields = PREPARATION_FIELDS
    if native_app_activation:
        operations.add('activate-native-app'); fields += ACTIVATION_FIELDS
    selected = [row for row in receipts if row.get('operation') in operations]
    if inspections:
        selected.append(inspections[-1])
    return [{**{key: row.get(key) for key in ('operation', 'stage') + fields},
             'windowState': preparation_state_projection(row.get('windowState')),
             'before': preparation_state_projection(row.get('before')),
             'after': preparation_state_projection(row.get('after'))}
            for row in selected]


def _preparation_scope(state, require_ready=False):
    if not isinstance(state, dict):
        return False
    booleans = WINDOW_STATE_FIELDS[:-1]
    if not all(type(state.get(key)) is bool for key in booleans):
        return False
    if not all(state[key] is True for key in (
            'candidateWindowKnown', 'sameCandidateControl', 'sameCandidateWindow', 'windowVisible', 'candidateScopeValid')):
        return False
    if state['truncated'] is not False or type(state.get('uniqueSiteCenterCount')) is not int or state['uniqueSiteCenterCount'] != 1:
        return False
    actual_ready = ((state['windowIsKey'] and state['appKeyWindowMatches']) or
                    (state['windowIsMain'] and state['appMainWindowMatches']))
    return state['keyOrMainReady'] is actual_ready and (not require_ready or actual_ready)


def native_window_preparation_complete(run, native_app_activation=False):
    """While Notification is pending: one window invocation, same-window proof, then denial."""
    receipts = run.get('receipts', [])
    if not isinstance(receipts, list) or not 4 <= len(receipts) <= 40 or not all(isinstance(row, dict) for row in receipts):
        return False
    operations = PREPARATION_OPERATIONS | {'activate-native-app'} if native_app_activation else PREPARATION_OPERATIONS
    activations = [row for row in receipts if row.get('operation') == 'activate-native-app'] if native_app_activation else []
    inventories = [row for row in receipts if row.get('operation') == 'inventory-native-window']
    preparations = [row for row in receipts if row.get('operation') == 'prepare-native-window']
    observations = [row for row in receipts if row.get('operation') == 'inspect-native-window']
    if len(inventories) != 1 or len(preparations) != 1 or not observations:
        return False
    initial, preparation = inventories[0], preparations[0]
    index = {id(row): position for position, row in enumerate(receipts)}
    first_normal = next((position for position, row in enumerate(receipts) if row.get('operation') == 'inventory'), None)
    if (first_normal is None or not index[id(initial)] < index[id(preparation)] < index[id(observations[0])]
            or index[id(observations[-1])] >= first_normal):
        return False
    # Every preparation observation must precede all normal inventory/actions.
    if any(row.get('operation') not in operations for row in receipts[:first_normal]):
        return False
    if any(row.get('operation') in operations for row in receipts[first_normal:]):
        return False
    if native_app_activation:
        if len(activations) != 1 or not index[id(initial)] < index[id(activations[0])] < index[id(preparation)]:
            return False
        activation = activations[0]
        if not all(activation.get(key) is True for key in (
                'actionAttempted', 'nativeMethodReturned', 'activationBudgetConsumed', 'activationIntentPersisted')):
            return False
        if activation.get('nativeActionKind') != 'NSApplication.activate':
            return False
        if not all(_preparation_scope(activation.get(key)) for key in ('before', 'after', 'windowState')):
            return False
        if (type(activation.get('activationAlreadyActiveBefore')) is not bool or
                activation['activationAlreadyActiveBefore'] is not activation['before']['applicationActive']):
            return False
    identity = initial.get('id')
    if (not isinstance(identity, str) or not identity or
            not all(isinstance(row.get('id'), str) and row['id'] for row in receipts) or
            len({row['id'] for row in receipts}) != len(receipts)):
        return False
    if any(row.get('preparationInventoryID') != identity for row in inventories + activations + preparations + observations):
        return False
    if not _preparation_scope(initial.get('windowState')) or not _preparation_scope(preparation.get('before')):
        return False
    if not _preparation_scope(preparation.get('after')) or not all(_preparation_scope(row.get('windowState')) for row in observations):
        return False
    if not _preparation_scope(observations[-1].get('windowState'), require_ready=True):
        return False
    if not all(row.get('stage') == 'deny' and not row.get('refusalReason') and
               row.get('causalProfileIsolated') is True and row.get('causalFixtureVerified') is True and
               row.get('mainThread') is True and row.get('functionalAcceptance') is False and row.get('preparationNotificationPendingVerified') is True
               for row in inventories + activations + preparations + observations):
        return False
    if any(row.get('actionAttempted') is not False or row.get('nativeMethodReturned') is not False or row.get('nativeActionKind') is not None
           for row in inventories + observations):
        return False
    if not all(preparation.get(key) is True for key in (
            'actionAttempted', 'nativeMethodReturned', 'preparationBudgetConsumed', 'preparationIntentPersisted')):
        return False
    if preparation.get('nativeActionKind') != 'NSWindow.makeKeyWindow':
        return False
    if type(preparation.get('preparationAlreadyReadyBefore')) is not bool or preparation['preparationAlreadyReadyBefore'] is not preparation['before']['keyOrMainReady']:
        return False
    preparation_index = index[id(preparation)]
    activation_index = index[id(activations[0])] if native_app_activation else None
    normal_count = reveal_count = 0
    for position, row in enumerate(receipts):
        operation = row.get('operation')
        if operation == 'press-prompt-site-center':
            reveal_count += 1
            if reveal_count > 1:
                return False
        elif operation in {'press-deny', 'press-allow', 'press-site-settings'}:
            normal_count += 1
        for key, expected in {'normalActionAttemptCount': normal_count, 'revealAttemptCount': reveal_count,
                              'actionAttemptCount': normal_count + reveal_count}.items():
            if type(row.get(key)) is not int or row[key] != expected:
                return False
        fields = ('preparationInvocationCount', 'actionAttemptCount', 'nativeActionsPerformed', 'totalPhysicalActions')
        if not all(type(row.get(key)) is int and row[key] >= 0 for key in fields):
            return False
        expected_preparation = int(position >= preparation_index)
        if row['preparationInvocationCount'] != expected_preparation or row['nativeActionsPerformed'] != row['actionAttemptCount']:
            return False
        if row.get('preparationBudgetConsumed') is not bool(expected_preparation) or row.get('preparationIntentPersisted') is not bool(expected_preparation):
            return False
        activation_count = int(position >= activation_index) if native_app_activation else 0
        if native_app_activation:
            if type(row.get('activationInvocationCount')) is not int or row['activationInvocationCount'] != activation_count:
                return False
            if row.get('activationBudgetConsumed') is not bool(activation_count) or row.get('activationIntentPersisted') is not bool(activation_count):
                return False
        if row['totalPhysicalActions'] != activation_count + row['preparationInvocationCount'] + row['actionAttemptCount']:
            return False
        if position <= index[id(observations[-1])] and row['actionAttemptCount'] != 0:
            return False
    if type(run.get('totalPhysicalActions')) is not int or run['totalPhysicalActions'] != receipts[-1]['totalPhysicalActions']:
        return False
    if native_app_activation and (type(run.get('activationInvocationCount')) is not int or run['activationInvocationCount'] != 1):
        return False
    elapsed = run.get('preparationObservationElapsedMs')
    if type(elapsed) not in (int, float) or not 0 <= elapsed <= 5000:
        return False
    if (type(run.get('preparationInvocationCount')) is not int or run['preparationInvocationCount'] != 1 or
            run.get('preparationReadyObserved') is not True or run.get('preparationOutcomeConfirmed') is not True or
            run.get('preparationUncertain') is not False or run.get('uncertainMutation')):
        return False
    denials = [row for row in receipts if row.get('operation') == 'press-deny']
    if len(denials) != 1:
        return False
    denial = denials[0]; qualified = denial.get('before') or {}; buttons = qualified.get('expectedButtonCounts') or {}
    if (denial.get('stage') != 'deny' or denial.get('actionAttempted') is not True or denial.get('nativeMethodReturned') is not True or
            denial.get('selectedButtonLabel') != 'Not Now' or denial.get('refusalReason') or qualified.get('truncated') is not False):
        return False
    if not all(type(qualified.get(key)) is int and qualified[key] == 1 for key in (
            'matchingGroupCount', 'exactMessageCount', 'notificationsIconCount')):
        return False
    if not all(type(buttons.get(key)) is int and buttons[key] == 1 for key in ('Not Now', 'Allow')):
        return False
    deny_steps = [row for row in run.get('steps', []) if row.get('step') == 'deny notification prompt']
    return len(deny_steps) == 1 and deny_steps[0].get('outcome') == 'pass'


def load_run(root, agent, page_names, native_window_preparation=False, native_app_activation=False):
    run = json.loads((root / agent / 'launch-1/run.json').read_text())
    fields = FIELDS + PREPARATION_FIELDS if native_window_preparation else FIELDS
    if native_app_activation:
        fields += ACTIVATION_FIELDS
    excluded = {'inventory', 'inspect-site-settings', 'snapshot-baseline', 'inspect-dismissed'}
    if native_window_preparation:
        excluded |= PREPARATION_READ_ONLY_OPERATIONS
    receipts = [{key: row.get(key) for key in fields}
                for row in run['receipts'] if row['operation'] not in excluded]
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


def permission_reveal_complete(run, native_window_preparation=False, native_app_activation=False):
    """A reveal is accepted only with its one native attempt and later denial."""
    excluded = {'inventory', 'inspect-site-settings', 'snapshot-baseline', 'inspect-dismissed'}
    if native_window_preparation:
        excluded |= PREPARATION_OPERATIONS
    if native_app_activation:
        excluded.add('activate-native-app')
    actions = [row for row in run.get('receipts', []) if row.get('operation') not in excluded]
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
    parser.add_argument('--agents', type=parse_agents, default=list(KNOWN_AGENTS), help='comma-separated agents to compare across recording modes (default: trace156,ours156)')
    parser.add_argument('--scenario', choices=['dialogs', 'dialogs-beforeunload', 'dialogs-auth-cancel', 'permissions-deny', 'cookies-storage-site-settings', 'context-menus'], default='dialogs')
    parser.add_argument('--prompt-reveal', action='store_true', help='allow one evidenced native Site Control Center reveal before canonical denial')
    parser.add_argument('--native-window-preparation', action='store_true', help='require one evidenced native makeKeyWindow while the first Notification request is pending')
    parser.add_argument('--native-app-activation', action='store_true', help='require one evidenced native NSApplication.activate before key-window preparation')
    args = parser.parse_args()
    if args.native_app_activation and (not args.native_window_preparation or not args.prompt_reveal or args.scenario not in {'permissions-deny', 'cookies-storage-site-settings'}):
        parser.error('--native-app-activation requires --native-window-preparation, --prompt-reveal and a permission scenario')
    if args.native_window_preparation and (not args.prompt_reveal or args.scenario not in {'permissions-deny', 'cookies-storage-site-settings'}):
        parser.error('--native-window-preparation requires --prompt-reveal and a permission scenario')
    if args.prompt_reveal and args.scenario not in {'permissions-deny', 'cookies-storage-site-settings'}:
        parser.error('--prompt-reveal is only valid for the two permission scenarios')
    if args.out.exists():
        parser.error('output already exists')
    expected = {'dialogs': ['alert', 'confirm', 'prompt'], 'dialogs-beforeunload': ['beforeunload-stay', 'beforeunload-leave'], 'dialogs-auth-cancel': ['auth-cancel'], 'permissions-deny': ['deny'], 'cookies-storage-site-settings': ['deny', 'allow', 'site-settings'], 'context-menus': ['context-reload']}[args.scenario]
    page_names = {'dialogs': {'alert native completion', 'confirm cancel', 'prompt native text and completion'}, 'dialogs-beforeunload': {'stay cancels navigation', 'leave commits navigation'}, 'dialogs-auth-cancel': {'cancel displays fixed 401 response'}, 'permissions-deny': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion'}, 'cookies-storage-site-settings': {'cookie/local/session storage roundtrip', 'deny notification prompt', 'cookie deletion', 'second origin starts with undecided permission', 'allow notification prompt', 'site settings reflects insecure HTTP fixture'}, 'context-menus': {'new native page context menu outside menu bar', 'contextual reload commits', 'context menu tracking ended'}}[args.scenario]
    expected_actions = {'dialogs': ['press-alert-ok', 'press-confirm-cancel', 'set-prompt-text', 'press-prompt-ok'], 'dialogs-beforeunload': ['press-beforeunload-cancel', 'press-beforeunload-ok'], 'dialogs-auth-cancel': ['press-auth-cancel'], 'permissions-deny': ['press-deny'], 'cookies-storage-site-settings': ['press-deny', 'press-allow', 'press-site-settings'], 'context-menus': ['press-reload']}[args.scenario]
    results = []
    for agent in args.agents:
        plain, plain_actions, plain_steps = load_run(args.unrecorded, agent, page_names, args.native_window_preparation, args.native_app_activation)
        recorded, recorded_actions, recorded_steps = load_run(args.recorded, agent, page_names, args.native_window_preparation, args.native_app_activation)
        coverage = recorded.get('coverage') or {}
        plain_artifacts = plain.get('artifactBefore') or {}
        recorded_artifacts = recorded.get('artifactBefore') or {}
        shared_paths = set(plain_artifacts) & set(recorded_artifacts)
        # Recorder exists only in the recorded mode. Main, engine and helper
        # must be the identical bytes across both controlled runs.
        matching_pair_artifacts = len(plain_artifacts) == 3 and len(shared_paths) == 3 and all(
            plain_artifacts[path] == recorded_artifacts[path] for path in shared_paths)
        allowed_actions = [expected_actions, ['press-prompt-site-center'] + expected_actions] if args.prompt_reveal else [expected_actions]
        if args.native_window_preparation:
            allowed_actions = [['prepare-native-window'] + actions for actions in allowed_actions]
        if args.native_app_activation:
            allowed_actions = [['activate-native-app'] + actions for actions in allowed_actions]
        checks = {
            'unrecorded_pages_complete': plain['completedStages'] == expected,
            'recorded_pages_complete': recorded['completedStages'] == expected,
            'page_assertions_present_and_pass': len(plain_steps) == len(recorded_steps) == len(page_names)
                and all(row['outcome'] == 'pass' for row in plain_steps + recorded_steps),
            'page_outcomes_equal': plain_steps == recorded_steps,
            'expected_real_control_receipts': [row['operation'] for row in plain_actions] == [row['operation'] for row in recorded_actions]
                and [row['operation'] for row in plain_actions] in allowed_actions,
            'permission_reveal_complete': not args.prompt_reveal or (permission_reveal_complete(plain, args.native_window_preparation, args.native_app_activation) and permission_reveal_complete(recorded, args.native_window_preparation, args.native_app_activation)),
            'control_receipts_equal': plain_actions == recorded_actions,
            'context_lifecycle_complete': args.scenario != 'context-menus' or (context_lifecycle_complete(plain) and context_lifecycle_complete(recorded)),
            'no_uncertain_mutations': not plain.get('uncertainMutation') and not recorded.get('uncertainMutation'),
            'stable_unrecorded_artifacts': plain.get('artifactsStable') is True,
            'stable_recorded_artifacts': recorded.get('artifactsStable') is True,
            'same_main_engine_helper_across_modes': matching_pair_artifacts,
            'causal_isolation_when_required': args.scenario == 'dialogs' or bool(plain.get('profileIsolation') and recorded.get('profileIsolation')),
            'recorder_valid': not coverage.get('errors', ['missing coverage']) and coverage.get('callCount', 0) > 0,
        }
        if args.native_window_preparation:
            checks['native_window_preparation_complete'] = native_window_preparation_complete(plain, args.native_app_activation) and native_window_preparation_complete(recorded, args.native_app_activation)
            checks['native_window_preparation_equal'] = preparation_comparison(plain, args.native_app_activation) == preparation_comparison(recorded, args.native_app_activation)
        results.append({'agent': agent, 'checks': checks, 'pass': all(checks.values()),
                        'unrecorded_cleanup': plain.get('cleanup'), 'recorded_cleanup': recorded.get('cleanup'),
                        'limitation': 'Independent page/control noninterference only. Browser shutdown, full boundary parity and visual acceptance remain separate.'})
    report = {'pass': all(row['pass'] for row in results), 'runs': results,
              'scope': {'agents': args.agents, 'comparison': 'Recorded versus unrecorded page/control outcomes for selected agents only.'}}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2))
    print(json.dumps(report, indent=2))
    raise SystemExit(0 if report['pass'] else 1)

if __name__ == '__main__':
    main()
