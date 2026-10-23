#!/usr/bin/env python3
"""Private receipt-binding controls only. No entrypoint/lock/build/app calls."""
import copy
import json
from pathlib import Path
import sys
import types
import unittest
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
PATH=ROOT/'arc/browser/webextras_native13_reference_control_held.py'
M=types.ModuleType('native13_control');M.__file__=str(PATH);exec(compile(PATH.read_bytes(),str(PATH),'exec'),M.__dict__)

class Controls(unittest.TestCase):
    def setUp(self):
        self.identity={'files':{'resources.pak':{'sha256':'a'*64}}}
        self.proof={'contentEqual':True,'built':copy.deepcopy(self.identity),'packaged':copy.deepcopy(self.identity)}
        self.receipt={'passed':True,'status':'RESOURCE_BYTES_VERIFIED','expectHostGate':'present','builtPackagedPakExactMatch':True,'runtimePakSHA256':'a'*64,'builtPakSHA256':'a'*64,'resources':[{'textualId':name,'expectHostGate':'present','generatedExactMatch':True,'grdInputExactMatch':True,'intermediateExactMatch':True} for name in M.RESOURCE_IDS],'nativePackSanity':{name:{'version':5,'nativeLoaderChecksPassed':True,'writerLookupDeterminismPolicyPassed':True} for name in ('intermediate','built','packaged')}}
    def test_matching_receipt(self):
        self.assertEqual(M.bind_resource_receipt(self.receipt,self.proof,self.identity,copy.deepcopy(self.identity)),'a'*64)
    def test_different_returned_cleanup_identity(self):
        changed=copy.deepcopy(self.identity);changed['files']['resources.pak']['sha256']='b'*64
        with self.assertRaisesRegex(RuntimeError,'identity mismatch'):M.bind_resource_receipt(self.receipt,self.proof,self.identity,changed)
    def test_different_checker_receipt(self):
        self.receipt['runtimePakSHA256']='b'*64
        with self.assertRaisesRegex(RuntimeError,'not tied'):M.bind_resource_receipt(self.receipt,self.proof,self.identity,self.identity)
    def test_absent_gate_and_incomplete_native_proof(self):
        for key,value in [('expectHostGate','absent'),('nativePackSanity',{})]:
            receipt=copy.deepcopy(self.receipt);receipt[key]=value
            with self.assertRaises(RuntimeError):M.bind_resource_receipt(receipt,self.proof,self.identity,self.identity)
    def test_unsealed_fails_before_any_filesystem_or_process_work(self):
        with self.assertRaisesRegex(RuntimeError,'UNSEALED'):M.check_config({'status':'UNSEALED'})

if __name__=='__main__':unittest.main(verbosity=2)
