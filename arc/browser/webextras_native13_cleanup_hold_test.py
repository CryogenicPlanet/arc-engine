#!/usr/bin/env python3
"""Actual staged gate with private snapshots/acks; no ps/lock/app/signal calls."""
import copy
from pathlib import Path
import sys
import types
import unittest
from unittest.mock import patch
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
SOURCE=ROOT/'logs/wp/webextras-native13-cleanup-hold-proposal/staged/arc/browser/webextras_native13_reference_control_held.py'
M=types.ModuleType('cleanup_control');M.__file__=str(SOURCE);exec(compile(SOURCE.read_bytes(),str(SOURCE),'exec'),M.__dict__)

class GateTests(unittest.TestCase):
    def test_exact_prefixes_and_pid_start_command_evidence(self):
        main=str(ROOT.parent/'arc/build/ArcModAgent-ours156.app/Contents/MacOS/Arc')
        helper=str(ROOT.parent/'arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/Helpers/Helper.app/Contents/MacOS/Helper')
        rows=M.parse_actor_inventory(f' 12 1 Fri Oct 23 12:10:00 2026 {main} --agent\n 13 12 Fri Oct 23 12:10:01 2026 {helper} --type=renderer\n 14 1 Fri Oct 23 12:10:02 2026 /bin/echo {main}\n',ROOT)
        self.assertEqual([r['pid'] for r in rows],[12,13]);self.assertEqual(rows[1]['born'],'Fri Oct 23 12:10:01 2026')
        with self.assertRaisesRegex(RuntimeError,'unparsed'):M.parse_actor_inventory('cannot establish inventory',ROOT)
    def gate(self,states,ack_at=1,emit_failure=False):
        events=[];rounds=0
        def inventory():
            state=states[min(rounds,len(states)-1)]
            if isinstance(state,Exception):raise state
            return state
        def emit(suffix,state):
            if emit_failure and suffix.endswith('verified.json') and rounds==0:raise OSError('receipt unavailable')
            events.append((suffix,copy.deepcopy(state)))
        def ack():
            first=next(s for name,s in events if name=='.cleanup-hold.json')
            if rounds<ack_at:return {'stale':True}
            return {'controlSHA256':'a'*64,'lockOwnerPID':99,'holdToken':first['holdToken'],'releaseWhenGone':True}
        def pause():
            nonlocal rounds
            rounds+=1
            if rounds>5:raise AssertionError('gate did not release under valid controls')
        with patch.object(M,'ancestor_owner',return_value=99):M.cleanup_release_gate(99,inventory,pause,ack,emit,control_sha='a'*64)
        return rounds,events
    def test_clean_inventory_releases_without_ack(self):
        rounds,events=self.gate([[]]);self.assertEqual(rounds,0);self.assertEqual(events[0][1]['status'],'NO_MATCHING_DISPOSABLE_ACTORS_OBSERVED')
    def test_live_actor_latches_until_absent_and_parent_ack(self):
        actor={'pid':9,'ppid':1,'born':'captured start','command':'exact disposable helper'}
        rounds,events=self.gate([[actor],[],[]],ack_at=2)
        self.assertEqual(rounds,2);self.assertEqual(events[0][1]['actors'],[actor]);self.assertIn('PARENT_ACKNOWLEDGED',events[-1][1]['status'])
    def test_unknown_inventory_latches_hold(self):
        rounds,events=self.gate([OSError('ps unavailable'),[]]);self.assertEqual(rounds,1);self.assertFalse(events[0][1]['inventoryComplete']);self.assertIn('ps unavailable',events[0][1]['error'])
    def test_receipt_write_failure_latches_hold(self):
        rounds,events=self.gate([[],[]],emit_failure=True);self.assertEqual(rounds,1);self.assertEqual(events[0][0],'.cleanup-hold.json')

if __name__=='__main__':unittest.main(verbosity=2)
