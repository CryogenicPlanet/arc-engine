#!/usr/bin/env python3
"""Actual private-applied comparator controls, never shared recorder edits/apps."""
import copy
import hashlib
import importlib.util
import json
import re
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-pip-episode-after-favicon'
PATCH = ROOT / 'arc/browser/webextras_pip_episode_after_favicon.patch'
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


def strict_apply(source, patch):
    lines=source.splitlines(keepends=True); p=patch.splitlines(keepends=True); out=[]; pos=0;i=0;n=0
    while i<len(p):
        m=re.match(r'@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@',p[i])
        if not m:i+=1;continue
        start=int(m[1]); oldcount=int(m[2] or 1);newcount=int(m[4] or 1);at=start-1 if oldcount else start
        assert at>=pos;out+=lines[pos:at];pos=at;old=[];new=[];i+=1
        while i<len(p) and p[i][:1] in (' ','+','-') and not p[i].startswith(('--- ','+++ ')):
            if p[i][0] in ' -':old.append(p[i][1:])
            if p[i][0] in ' +':new.append(p[i][1:])
            i+=1
        assert len(old)==oldcount and len(new)==newcount
        assert lines[pos:pos+oldcount]==old,(start,'context mismatch')
        out+=new;pos+=oldcount;n+=1
    assert n;return ''.join(out+lines[pos:])


class PiPEpisodeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-pip-episode-test-')
        cls.directory = Path(cls.temp.name)
        cls.baseline = cls.directory / 'baseline'
        cls.repo = cls.directory / 'candidate'
        shutil.copytree(EVIDENCE / 'baseline', cls.baseline)
        shutil.copytree(EVIDENCE / 'baseline', cls.repo)
        patch=PATCH.read_text()
        first,second=patch.split('diff --git a/tests/recorder/pip_episode.py',1)
        path=cls.repo/'tests/recorder/diff.py'
        path.write_text(strict_apply(path.read_text(),first))
        (cls.repo/'tests/recorder/pip_episode.py').write_text(strict_apply('', 'diff --git a/tests/recorder/pip_episode.py'+second))
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

    def test_all_seven_archived_pairs_exact_delta_and_raw_pins(self):
        manifest=json.loads((ROOT/'results/webextras/accepted-comparator-run1-offline-20261023/manifest.json').read_text())
        summary=[]
        destination=EVIDENCE/'seven-pairs'
        destination.mkdir(exist_ok=True)
        for row in manifest['results']:
            for flags in ((),('--compare-boundary-arguments',)):
                name=row['scenario']+'-'+row['launch']+('-arguments' if flags else '-default')
                with self.subTest(name=name):
                    before,after=self.compare(Path(row['argv'][3]),Path(row['argv'][4]),flags)
                    self.assertEqual((before[0],before[2]),(after[0],after[2]))
                    if before[3] is None:
                        self.assertEqual(before,after)
                        self.assertEqual(before[0],2)
                        for label,result in [('projected-nativeview-favicon',before),('plus-pip',after)]:
                            (destination/(name+'-'+label+'.stdout')).write_bytes(result[1])
                            (destination/(name+'-'+label+'.stderr')).write_bytes(result[2])
                        summary.append(dict(name=name,flags=list(flags),exit=[before[0],after[0]],removed=[],unsupportedBaselineExact=True,report=None,firstTwoLines={'projectedNativeViewFavicon':before[1].decode().splitlines()[:2],'plusPiP':after[1].decode().splitlines()[:2]},stderr=before[2].decode()))
                        continue
                    a,b=json.loads(before[3]),json.loads(after[3])
                    removed=[r for r in a['return_mismatch'] if r['method']==PIP] if row['scenario']=='media-pip' else []
                    self.assertEqual(len(removed),1 if row['scenario']=='media-pip' else 0)
                    expected=copy.deepcopy(a)
                    for r in removed:expected['return_mismatch'].remove(r)
                    self.assertEqual(b,expected)
                    if not removed:self.assertEqual(before,after)
                    for label,result in [('projected-nativeview-favicon',before),('plus-pip',after)]:
                        (destination/(name+'-'+label+'.json')).write_bytes(result[3])
                        (destination/(name+'-'+label+'.stdout')).write_bytes(result[1])
                        (destination/(name+'-'+label+'.stderr')).write_bytes(result[2])
                    summary.append(dict(name=name,flags=list(flags),exit=[before[0],after[0]],removed=removed,
                        firstTwoLines={'projectedNativeViewFavicon':before[1].decode().splitlines()[:2],'plusPiP':after[1].decode().splitlines()[:2]},
                        everyOtherReportValueExact=True,defaultOrArgumentStdoutExact=before[1]==after[1]))
        (destination/'summary.json').write_text(json.dumps(summary,indent=2)+'\n')
        self.assertEqual(len(summary),14)

    def test_nativeview_original_38_controls_unchanged_by_pip(self):
        controls=ROOT/'logs/wp/core.nativeview-comparator-increment/controls'
        cases=sorted(controls.glob('*/golden.jsonl'))
        self.assertEqual(len(cases),38)
        # Replay each original invocation's exact optional policy modes.
        invocations=json.loads((ROOT/'logs/wp/core.nativeview-comparator-increment/invocations.json').read_text())
        by_case={row['case']:row for row in invocations if '/stage/' in row['script']}
        self.assertEqual(len(by_case),38)
        for path in cases:
            invocation=by_case[path.parent.name]
            args=invocation['argv']
            flags=args[args.index('--json')+2:]
            with self.subTest(case=path.parent.name):self.unchanged(path,path.parent/'ours.jsonl',flags)

    def test_all_57_app_favicon_controls_and_56_actual_loads_unchanged(self):
        app=ROOT/'logs/wp/app.legacy77775.favicon-after-nativeview-proposal'
        source=app/'scope-controls-source.py'
        namespace={};exec(compile(source.read_bytes(),str(source),'exec'),namespace)
        controls=namespace['checks'];self.assertEqual(len(controls),57)
        results=[]
        for repo in (self.baseline,self.repo):
            path=repo/'tests/recorder/diff.py'
            for name in ('download_identity','google_q','omnibox_receivers'):sys.modules.pop(name,None)
            sys.path.insert(0,str(path.parent))
            try:
                module={'__name__':'private_favicon_control','__file__':str(path)}
                exec(compile(path.read_bytes(),str(path),'exec'),module)
                self.assertEqual(Path(sys.modules['download_identity'].__file__).resolve(),(path.parent/'download_identity.py').resolve())
            finally:sys.path.pop(0)
            actor=[]
            for name,a,b,equal,unchanged in controls:
                with self.subTest(repo=repo.name,control=name):
                    original_a,original_b=copy.deepcopy(a),copy.deepcopy(b)
                    aa=module['favicon_header_args'](a,a.get('args'));bb=module['favicon_header_args'](b,b.get('args'))
                    self.assertEqual((aa==bb),equal)
                    self.assertEqual((a,b),(original_a,original_b))
                    if unchanged:self.assertEqual(aa,a.get('args'))
                    loaded=a.get('ev')==b.get('ev')=='call'
                    if loaded:
                        folder=self.directory/('favicon-'+repo.name+'-'+name);folder.mkdir()
                        ga,ob=folder/'golden.jsonl',folder/'ours.jsonl'
                        ga.write_text(json.dumps(a)+'\n');ob.write_text(json.dumps(b)+'\n')
                        ga_load=module['load'](ga);ob_load=module['load'](ob)
                        self.assertEqual(ga_load==ob_load,equal)
                        actor.append((name,ga_load,ob_load))
            results.append(actor)
        self.assertEqual(results[0],results[1])
        (EVIDENCE/'favicon-control-summary.json').write_text(json.dumps(dict(pureControls=57,positive=4,negative=53,actualLoads=56,projectedVsPiPExact=True),indent=2)+'\n')

    def test_all_four_archived_app_favicon_pairs_unchanged_by_pip(self):
        app=ROOT/'logs/wp/app.legacy77775.favicon-after-nativeview-proposal'
        pairs=json.loads((app/'raw-inputs.json').read_text())
        self.assertEqual(len(pairs),4)
        summary=[]
        for name,pair in pairs.items():
            before,after=self.compare(app/pair['golden']['copy'],app/pair['ours']['copy'])
            self.assertEqual(before,after)
            self.assertEqual(before[0],0)
            summary.append(dict(name=name,entireOutputsExact=True,firstTwoLines=after[1].decode().splitlines()[:2]))
        (EVIDENCE/'favicon-four-pair-summary.json').write_text(json.dumps(summary,indent=2)+'\n')

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
        (EVIDENCE/'automatic-eight-capability.json').write_text(json.dumps(dict(frozen=state,frontend=support),indent=2)+'\n')


if __name__ == '__main__':
    unittest.main(verbosity=2)
