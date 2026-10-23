#!/usr/bin/env python3
"""Preserve raw diff and separately compare the explicitly owned UI boundary."""
import argparse
import json
from pathlib import Path
import subprocess

CLASSES = {
    'ArcWebDialog', 'ArcWebLoginDialog', 'ArcPermissionRequest', 'ArcConfirmInfobar',
    'ArcHungRendererDialog', 'ArcExternalProtocolDialog', 'ArcDevToolsConnectionDialog',
    'ArcContextMenuElementInfo', 'ArcFormControlInfo', 'ArcSelectOption', 'ArcMenuModel',
    'ArcMenuItemModel', 'ArcNotification', 'ArcNotificationButton', 'ArcNotificationItem',
    'ArcDisplayNameOrOrigin', 'ArcKeyboardAccelerator',
}
CALLBACKS = {
    'webContents:willRunJavaScriptAlertPanel:withMessage:completion:',
    'webContents:willRunJavaScriptConfirmPanel:withMessage:completion:',
    'webContents:willRunJavaScriptTextInputPanel:withMessage:withDefaultText:completion:',
    'webContentsWillCloseJavaScriptPanel:',
    'webContents:runJavaScriptApplicationModalBeforeUnloadPanel:withMessage:withPrimaryButtonLabel:completion:',
    'webContentsCloseJavaScriptApplicationModalBeforeUnloadPanel:',
    'webContents:willAskForPermission:withRequests:withQuietUIReason:completion:',
    'webContentsClosePermission:', 'webContents:willShowContextMenu:withElement:',
    'webContents:showWebLoginDialog:', 'webContentsDidCloseWebLoginDialog:',
    'webContents:didAutoFillWebLoginDialog:', 'webContents:showExternalProtocolDialog:',
    'webContents:showHungRendererDialog:', 'webContentsHideHungRendererDialog:',
    'webContents:showWebUIDialog:', 'webContents:didCloseWebUIDialog:',
    'webContents:willShowConfirmInfobar:', 'webContents:willCloseConfirmInfobar:',
    'menuItemModelChanged:', 'menuItemModelRemoved:',
}

# app_callbacks.inc86-87 identifies the concrete receiver. Do not treat every
# unrelated app close:/display: selector as a notification callback.
RECEIVER_CALLBACKS = {
    ('ADK.ADKApplication', 'display:'), ('ADK.ADKApplication', 'close:'),
    ('_TtC3ADK14ADKApplication', 'display:'), ('_TtC3ADK14ADKApplication', 'close:'),
}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--golden', type=Path, required=True)
    parser.add_argument('--ours', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[2]
    if not args.golden.is_file() or not args.ours.is_file():
        parser.error('both existing transcripts are required')
    args.out.mkdir(parents=True, exist_ok=False)
    for name, source in [('golden', args.golden), ('ours', args.ours)]:
        with source.open() as stream, (args.out / (name + '.ui.jsonl')).open('w') as dest:
            for line in stream:
                row = json.loads(line)
                if row.get('cls') in CLASSES or (row.get('dir') == 'engine->app' and (row.get('sel') in CALLBACKS or (row.get('cls'), row.get('sel')) in RECEIVER_CALLBACKS)):
                    dest.write(line)
    for label, golden, ours in [
        ('raw', args.golden, args.ours),
        ('ui', args.out / 'golden.ui.jsonl', args.out / 'ours.ui.jsonl'),
    ]:
        result = subprocess.run([
            'python3', str(root / 'tests/recorder/diff.py'), str(golden), str(ours),
            '--json', str(args.out / (label + '.json')),
        ], check=True, text=True, capture_output=True)
        (args.out / (label + '.txt')).write_text(result.stdout)
        print(label + ': ' + '\n'.join(result.stdout.splitlines()[:2]))
    (args.out / 'scope.json').write_text(json.dumps({
        'classes': sorted(CLASSES), 'callbacks': sorted(CALLBACKS),
        'receiver_callbacks': sorted(RECEIVER_CALLBACKS),
        'golden': str(args.golden.resolve()), 'ours': str(args.ours.resolve()),
        'scope': 'Explicit UI ownership only; raw diff retained. No transcript value normalization beyond the existing shared diff tool.',
    }, indent=2))

if __name__ == '__main__':
    main()
