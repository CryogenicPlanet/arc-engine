#!/usr/bin/env python3
"""Actual private-applied comparator controls, never shared recorder edits/apps."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-pip-episode-comparison'
PATCH = ROOT / 'arc/browser/webextras_pip_episode_comparison.patch'
GOLD = ROOT / 'results/golden/arc-1.170-media-pip/launch-1/record.jsonl'
OURS = ROOT / 'results/webextras/media-pip-current-run1/launch-1/record.jsonl'
PIP = '->engine -ArcWebContentsMediaSession pipController'
CTRL = 'ArcWebContentsMediaSessionPiPController'


def read(path):
    return [json.loads(line) for line in path.read_text().splitlines()]


def event(rows, seq):
    return next(r for r in rows if r['seq'] == seq)


def resequence(rows):
    # Derived insertion/reordering controls only. Original captures stay intact.
    for seq, row in enumerate(rows):
        row['seq'] = seq
    return rows


def callback(selector, args, receiver='<ADK.WebContentChromium#126>', depth=2):
    common = dict(thread='main', depth=depth, dir='engine->app', kind='-',
                  cls='ADK.WebContentChromium', sel=selector, self=receiver, **{'from': 'engine'})
    return [dict(common, ev='call', args=args), dict(common, ev='ret', ret=None)]


def synthetic(offset, fresh, url='https://example.test/watch?video=alpha'):
    """Different URL/IDs from recorded fixture: no /a or local-host assumption."""
    rows = []
    def ref(cls, n):
        return f'<{cls}#{n + offset}>'
    browser, wc, ms, delegate = ref('ArcBrowser', 1), ref('ArcWebContents', 2), ref('ArcWebContentsMediaSession', 3), ref('ADK.WebContentChromium', 4)
    c0, c1 = ref(CTRL, 5), ref(CTRL, 6 if fresh else 5)
    sid = ref('ArcSessionID', 7)
    def begin(cls, sel, receiver, args=(), depth=0, direction='->engine', caller='app'):
        row = dict(ev='call', thread='main', depth=depth, dir=direction, kind='-', cls=cls, sel=sel, self=receiver, args=list(args), **{'from': caller})
        rows.append(row)
        return row
    def end(row, value=None):
        rows.append(dict({k:v for k,v in row.items() if k not in ('ev', 'args')}, ev='ret', ret=value))
    def pair(cls, sel, receiver, value=None, args=(), depth=0):
        r = begin(cls, sel, receiver, args, depth); end(r, value)
    pair('ArcBrowserApplication', 'createBrowser:withDelegate:forWindow:', ref('ArcBrowserApplication', 8), browser, [ref('ArcBrowserContext', 9), None, 'window'])
    new = begin('ADK.ChromiumBrowser', 'browser:didCreateNewWebContents:isActive:', ref('ADK.ChromiumBrowser', 10), [browser, wc, True], direction='engine->app', caller='engine')
    pair('ArcWebContents', 'mediaSession', wc, ms, depth=1)
    pair('ArcWebContentsMediaSession', 'setDelegate:', ms, args=[delegate], depth=1)
    pair('ArcWebContents', 'url', wc, 'url:' + url, depth=1)
    pair('ArcWebContents', 'sessionId', wc, sid, depth=1)
    pair('ArcSessionID', 'identifier', sid, 73 + offset, depth=1)
    end(new)
    created = begin('ADK.WebContentChromium', 'webContents:pipControllerCreated:', delegate, [wc, c0], direction='engine->app', caller='engine'); end(created)
    show = begin('ADK.WebContentChromium', 'webContentsShowPiP:', delegate, [wc], direction='engine->app', caller='engine')
    pair('ArcWebContents', 'mediaSession', wc, ms, depth=1)
    pair('ArcWebContentsMediaSession', 'pipController', ms, c1, depth=1)
    pair(CTRL, 'videoView', c1, ref('PiPNSView', 11), depth=1)
    pair(CTRL, 'sourceElementBounds', c1, '{{1, 2}, {3, 4}}', depth=1)
    end(show)
    hide = begin('ADK.WebContentChromium', 'webContentsHidePiP:', delegate, [wc], direction='engine->app', caller='engine'); end(hide)
    return resequence(rows)


class PiPEpisodeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-pip-episode-test-')
        cls.directory = Path(cls.temp.name)
        cls.baseline = cls.directory / 'baseline'
        cls.repo = cls.directory / 'candidate'
        shutil.copytree(EVIDENCE / 'baseline', cls.baseline)
        shutil.copytree(EVIDENCE / 'baseline', cls.repo)
        for command in (['git', 'apply', '--check', str(PATCH)], ['git', 'apply', str(PATCH)]):
            subprocess.run(command, cwd=cls.repo, check=True, capture_output=True)
        for relative in ('tests/recorder/diff.py', 'tests/recorder/pip_episode.py'):
            if (cls.repo / relative).read_bytes() != (EVIDENCE / 'staged' / relative).read_bytes():
                raise AssertionError('strict patch reconstruction differs: ' + relative)
        cls.pins = json.loads((EVIDENCE / 'input-pins.json').read_text())
        cls.pins[str(PATCH.relative_to(ROOT))] = hashlib.sha256(PATCH.read_bytes()).hexdigest()
        for relative in ('arc/browser/webextras_fixture_inputs.py', 'arc/browser/webextras_frontend_comparison.py'):
            cls.pins[relative] = hashlib.sha256((ROOT / relative).read_bytes()).hexdigest()
        cls.counter = 0

    @classmethod
    def tearDownClass(cls):
        try:
            for relative, digest in cls.pins.items():
                if hashlib.sha256((ROOT / relative).read_bytes()).hexdigest() != digest:
                    raise AssertionError('input changed: ' + relative)
        finally:
            cls.temp.cleanup()

    def compare(self, left, right, flags=()):
        type(self).counter += 1
        directory = self.directory / f'case-{self.counter}'
        directory.mkdir()
        paths = []
        for name, rows in [('golden', left), ('ours', right)]:
            path = directory / name
            if isinstance(rows, Path):
                shutil.copyfile(rows, path)
            elif isinstance(rows, str):
                path.write_text(rows)
            else:
                path.write_text(''.join(json.dumps(r, separators=(',', ':')) + '\n' for r in rows))
            paths.append(path)
        results = []
        for label, repo in [('baseline', self.baseline), ('candidate', self.repo)]:
            report = directory / f'{label}.json'
            result = subprocess.run([sys.executable, '-B', str(repo / 'tests/recorder/diff.py'), *map(str, paths), '--json', str(report), *flags], capture_output=True)
            raw = report.read_bytes() if report.exists() else None
            results.append((result.returncode, result.stdout, result.stderr, raw))
        return results

    def unchanged(self, left, right, flags=()):
        before, after = self.compare(left, right, flags)
        self.assertEqual(after, before)
        return json.loads(after[3]) if after[3] else None

    def removes_only_pip(self, left=GOLD, right=OURS, flags=()):
        before, after = self.compare(left, right, flags)
        self.assertEqual((after[0], after[2]), (before[0], before[2]))
        a, b = json.loads(before[3]), json.loads(after[3])
        removed = [r for r in a['return_mismatch'] if r['method'] == PIP]
        self.assertEqual(len(removed), 1)
        a['return_mismatch'] = [r for r in a['return_mismatch'] if r['method'] != PIP]
        self.assertEqual(b, a)  # Every other list, row, callback and report key.
        return b

    def rows(self):
        return read(GOLD), read(OURS)

    def test_actual_nonempty_default_one_row_only(self):
        report = self.removes_only_pip()
        self.assertEqual(report['callback_sequence']['golden'], 172)
        self.assertEqual(report['callback_sequence']['ours'], 170)
        self.assertIn('->engine -ArcMediaPosition currentTime', [r['method'] for r in report['return_mismatch']])

    def test_actual_argument_mode_one_row_only(self):
        self.removes_only_pip(flags=['--compare-boundary-arguments'])

    def test_actual_self_comparisons_unchanged(self):
        for path in (GOLD, OURS):
            with self.subTest(path=path): self.unchanged(path, path)

    def test_generic_public_graph_without_fixture_constants(self):
        self.removes_only_pip(synthetic(0, True), synthetic(100, False))

    def test_different_owner_urls_retain_exact_baseline(self):
        self.unchanged(synthetic(0, True), synthetic(100, False, 'https://example.test/another-video'))

    def test_wrong_owner(self):
        a,b=self.rows();event(a,4156)['args'][0]='<ArcWebContents#999>';self.unchanged(a,b)

    def test_wrong_delegate(self):
        a,b=self.rows()
        for seq in (4156,4195):event(a,seq)['self']='<ADK.WebContentChromium#999>'
        self.unchanged(a,b)

    def test_wrong_getter_pair(self):
        a,b=self.rows();event(a,4274)['self']='<ArcWebContentsMediaSession#999>';self.unchanged(a,b)

    def test_wrong_getter_ancestry(self):
        a,b=self.rows()
        for r in a:
            if 4271 <= r['seq'] <= 4274:r['depth']-=1
        self.unchanged(a,b)

    def test_replacement_creation_before_getter(self):
        a,b=self.rows();i=a.index(event(a,4271));a[i:i]=callback('webContents:pipControllerCreated:',['<ArcWebContents#118>',f'<{CTRL}#999>']);self.unchanged(resequence(a),b)

    def test_hide_before_getter(self):
        a,b=self.rows();i=a.index(event(a,4271));a[i:i]=callback('webContentsHidePiP:',['<ArcWebContents#118>']);self.unchanged(resequence(a),b)

    def test_destroy_before_getter(self):
        a,b=self.rows();i=a.index(event(a,4271));a[i:i]=callback('webContentsPiPControllerDestroyed:',['<ArcWebContents#118>']);self.unchanged(resequence(a),b)

    def test_malformed_lifecycle_lanes_retain_exact_baseline(self):
        for selector,args in [('webContents:pipControllerCreated:',['<ArcWebContents#118>',f'<{CTRL}#999>']),('webContentsPiPControllerDestroyed:',['<ArcWebContents#118>'])]:
            for field,value in [('dir','bogus'),('dir','->engine'),('kind','?'),('kind','+'),('from','OtherImage.dylib')]:
                with self.subTest(selector=selector,field=field,value=value):
                    a,b=self.rows();extra=callback(selector,args)
                    for r in extra:r[field]=value
                    i=a.index(event(a,4271));a[i:i]=extra;self.unchanged(resequence(a),b)

    def test_lifecycle_unknown_phase_retain_exact_baseline(self):
        a,b=self.rows();extra=callback('webContents:pipControllerCreated:',['<ArcWebContents#118>',f'<{CTRL}#999>'])
        for r in extra:r['ev']='unknown'
        i=a.index(event(a,4271));a[i:i]=extra;self.unchanged(resequence(a),b)

    def test_extra_owner(self):
        a,b=self.rows();i=a.index(event(a,4156));extra=callback('browser:didCreateNewWebContents:isActive:',['<ArcBrowser#89>','<ArcWebContents#999>',True],'<ADK.ChromiumBrowser#94>',1)
        for r in extra:r['cls']='ADK.ChromiumBrowser'
        a[i:i]=extra;self.unchanged(resequence(a),b)

    def test_missing_owner_url(self):
        a,b=self.rows();a=[r for r in a if not (r.get('cls')=='ArcWebContents' and r.get('sel')=='url' and r.get('from')=='app')];self.unchanged(a,b)

    def test_conflicting_session_identifier(self):
        a,b=self.rows();event(a,3236)['ret']=407265286;self.unchanged(a,b)

    def test_conflicting_raw_url_hidden_by_existing_port_mask_rejects(self):
        a,b=self.rows();event(a,3657)['ret']='url:http://127.0.0.1:54614/a';self.unchanged(a,b)

    def test_malformed_session_identifier_boolean(self):
        a,b=self.rows();event(a,3232)['ret']=True;self.unchanged(a,b)

    def test_missing_caller_retains_baseline(self):
        a,b=self.rows();event(a,4271).pop('from');self.unchanged(a,b)

    def test_unselected_caller_retains_baseline(self):
        a,b=self.rows()
        for seq in (4271,4274):event(a,seq)['from']='OtherImage.dylib'
        self.unchanged(a,b)

    def test_duplicate_sequence_counter_retains_baseline(self):
        a,b=self.rows();event(a,4274)['seq']=4273;self.unchanged(a,b)

    def test_unrelated_reference_shaped_string_stays_different(self):
        a,b=self.rows()
        for rows,token in [(a,f'<{CTRL}#150>'),(b,f'<{CTRL}#132>')]:
            row=next(r for r in rows if r.get('ev')=='call' and r.get('sel')=='webContents:pipSourceTitleChanged:');row['args'][1]=token
        report=self.removes_only_pip(a,b)
        self.assertIn('engine->app -ADK.WebContentChromium webContents:pipSourceTitleChanged:',[r['method'] for r in report['arg_mismatch']])

    def test_getter_payload_return_stays_different(self):
        a,b=self.rows();event(b,4414)['ret']='{{145, 453}, {128, 128}}';report=self.removes_only_pip(a,b)
        self.assertIn(f'->engine -{CTRL} sourceElementBounds',[r['method'] for r in report['return_mismatch']])

    def test_video_view_return_stays_different(self):
        a,b=self.rows();event(b,4408)['ret']='<DifferentView#999>';report=self.removes_only_pip(a,b)
        self.assertIn(f'->engine -{CTRL} videoView',[r['method'] for r in report['return_mismatch']])

    def test_invalid_controller_return_retains_baseline(self):
        a,b=self.rows();event(a,4274)['ret']=None;self.unchanged(a,b)

    def test_getter_conflicting_member_receiver_retains_baseline(self):
        a,b=self.rows();event(a,4274)['ret']=f'<{CTRL}#999>';self.unchanged(a,b)

    def test_callback_scalar_argument_stays_different(self):
        a,b=self.rows();row=next(r for r in b if r.get('ev')=='call' and r.get('sel')=='webContents:pipPlaybackStateChanged:');row['args'][1]=999
        report=self.removes_only_pip(a,b)
        self.assertIn('engine->app -ADK.WebContentChromium webContents:pipPlaybackStateChanged:',[r['method'] for r in report['arg_mismatch']])

    def test_callback_count_and_sequence_changes_remain(self):
        a,b=self.rows();b += callback('webContents:pipPlaybackStateChanged:',['<ArcWebContents#102>',999],'<ADK.WebContentChromium#110>',1)
        report=self.removes_only_pip(a,resequence(b))
        self.assertEqual(report['callback_sequence']['ours'],171)
        row=next(r for r in report['count_mismatch'] if r['method'].endswith('webContents:pipPlaybackStateChanged:'));self.assertEqual((row['golden'],row['ours']),(6,7))

    def test_getter_count_change_rejects_alias_and_remains(self):
        a,b=self.rows();block=copy.deepcopy([r for r in a if 4271 <= r['seq'] <= 4274]);i=a.index(event(a,4271));a[i:i]=block
        report=self.unchanged(resequence(a),b)
        row=next(r for r in report['count_mismatch'] if r['method']==PIP);self.assertEqual((row['golden'],row['ours']),(2,1))

    def test_no_alias_after_hide(self):
        a,b=self.rows()
        for rows,seqs,at in [(a,(4271,4274),4665),(b,(4401,4404),4805)]:
            block=[copy.deepcopy(r) for r in rows if seqs[0] <= r['seq'] <= seqs[1]]
            for r in block:r['depth']-=1
            i=rows.index(event(rows,at))+1;rows[i:i]=block;resequence(rows)
        self.unchanged(a,b)

    def test_single_getter_after_hide_is_not_an_active_episode(self):
        a,b=synthetic(0,True),synthetic(100,False)
        for rows in (a,b):
            moved=[r for r in rows if r['cls']==CTRL or (r['cls']=='ArcWebContentsMediaSession' and r['sel']=='pipController')]
            rows[:]=[r for r in rows if r not in moved]
            for r in moved:r['depth']=0
            rows.extend(moved);resequence(rows)
            self.assertEqual(sum(r['ev']=='call' and r['sel']=='pipController' for r in rows),1)
        self.unchanged(a,b)

    def test_single_hide_before_show_rejects_without_count_change(self):
        a,b=synthetic(0,True),synthetic(100,False)
        moved=[r for r in a if r['sel']=='webContentsHidePiP:'];a[:]=[r for r in a if r not in moved]
        i=next(i for i,r in enumerate(a) if r['ev']=='call' and r['sel']=='webContentsShowPiP:');a[i:i]=moved
        self.unchanged(resequence(a),b)

    def test_unrelated_scalar_setter_argument_remains(self):
        a,b=self.rows();row=next(r for r in b if r.get('ev')=='call' and r.get('sel')=='setIsLoading:' and r.get('from')=='engine');row['args']=[123]
        report=self.removes_only_pip(a,b,['--compare-boundary-arguments'])
        self.assertIn('->engine -ArcWebContents setIsLoading:',[r['method'] for r in report['arg_mismatch']])

    def test_non_pip_actual_scenarios_byte_exact(self):
        for scenario in ('launch-slice','devtools','pdf'):
            path=ROOT/f'results/golden/arc-1.170-{scenario}/launch-1/record.jsonl'
            with self.subTest(scenario=scenario):self.unchanged(path,path)

    def test_v1_and_empty_non_pip_byte_exact(self):
        self.unchanged('', '')
        a,b=self.rows()
        for rows in (a,b):
            for r in rows:r.pop('from',None)
        self.unchanged(a,b)

    def test_existing_error_help_and_strict_options_byte_exact(self):
        for flags in (['--help'],['--compare-boundary'],['--unknown'],['--fixture-image','../bad'],['--compare-boundary-arguments']):
            with self.subTest(flags=flags):self.unchanged('', '', flags)

    def test_automatic_freezer_eight_inputs_and_all_four_frontend_canaries(self):
        relative='arc/browser/webextras_fixture_inputs.py'
        target=self.repo/relative;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/relative,target)
        freezer=types.ModuleType('private_freezer');exec(compile(target.read_bytes(),str(target),'exec'),freezer.__dict__)
        path=ROOT/'arc/browser/webextras_frontend_comparison.py'
        frontend=types.ModuleType('private_frontend');exec(compile(path.read_bytes(),str(path),'exec'),frontend.__dict__)
        snapshot=self.directory/'automatic-eight-input-snapshot'
        state=freezer.freeze(self.repo,snapshot,'WebextrasNativeProbe.dylib')
        self.assertTrue(state['capability']['verified'])
        self.assertEqual(len(state['files']),8)
        self.assertIn('tests/recorder/pip_episode.py',state['files'])
        support=frontend.capability(snapshot,'WebextrasNativeProbe.dylib')
        self.assertTrue(support['verified'],support)
        self.assertEqual(len(support['cases']),4)
        self.assertEqual(freezer.verify(snapshot),state)


if __name__ == '__main__':
    unittest.main(verbosity=2)
