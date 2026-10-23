#!/usr/bin/env python3
"""Private source-native sanity correction tests; no live resource reads."""
from pathlib import Path
import ast
import json
import struct
import subprocess
import sys

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
# Replay all24 frozen original regression groups against the distinct aggregate.
original_test = ROOT / 'arc/browser/webextras_frontend_resource_check_test.py'
code = original_test.read_text().replace("'arc/browser/webextras_frontend_resource_check.py'", "'arc/browser/webextras_frontend_resource_checked.py'")
exec(compile(code, str(original_test), 'exec'), globals())
for fixture_name in ['plain', 'gzip', 'brotli', 'mixed', 'alias', 'rollback-absent']:
    receipt = json.loads((PROOF / fixture_name / 'receipt/receipt.json').read_text())
    assert set(receipt['nativePackSanity']) == {'intermediate', 'built', 'packaged'}
    assert all(r['version'] == 5 and r['nativeLoaderChecksPassed'] and r['writerLookupDeterminismPolicyPassed'] for r in receipt['nativePackSanity'].values())

# Compile only the reviewed balanced extractor, without executing its driver.
extractor_source = (ROOT / 'arc/browser/webextras_devtools_host_gate_test.py').read_bytes()
extractor = next(n for n in ast.parse(extractor_source).body if isinstance(n, ast.FunctionDef) and n.name == 'method')
exec(compile(ast.Module(body=[extractor], type_ignores=[]), '<balanced-extractor>', 'exec'), globals())
cc = (ROOT / 'chromium/src/ui/base/resource/data_pack.cc').read_text()
actual = '\n'.join(method(cc, a) for a in ['int DataPack::Entry::CompareById(', 'int DataPack::Alias::CompareById(', 'DataPack::SanityCheckFileAndRegisterResources(', 'DataPack::LoadImpl(', 'const DataPack::Entry* DataPack::LookupEntryById('])
actual = actual.replace('DataPack::SanityCheckFileAndRegisterResources(', 'base::expected<void, DataPack::FailureReason> DataPack::SanityCheckFileAndRegisterResources(').replace('DataPack::LoadImpl(', 'base::expected<void, DataPack::FailureReason> DataPack::LoadImpl(')
fixture_cpp = r'''
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#define UNSAFE_TODO(x) x
#define LOG(x) std::cerr
#define DLOG(x) std::cerr
#define RETURN_IF_ERROR(x) do{auto result=(x);if(!result.good)return result;}while(false)
namespace base {struct Good{};Good ok(){return{};}template<class E>struct Bad{E error;};template<class E>Bad<E>unexpected(E e){return{e};}template<class T,class E>struct expected{bool good;expected(Good):good(true){}expected(Bad<E>):good(false){}};}
constexpr uint32_t kFileFormatV4=4,kFileFormatV5=5;constexpr size_t kHeaderLengthV4=9,kHeaderLengthV5=12;
class DataPack{public:
 enum TextEncodingType{BINARY,UTF8,UTF16};
 enum class FailureReason{kTooShort,kBoundsExceeded,kOrderingViolation,kAliasTableCorrupt,kIncompleteHeader,kBadPakVersion,kBadEncodingType};
 #pragma pack(push,1)
 struct Entry{static int CompareById(const void*,const void*);uint16_t resource_id;uint32_t file_offset;};
 struct Alias{static int CompareById(const void*,const void*);uint16_t resource_id;uint16_t entry_index;};
 #pragma pack(pop)
 struct DataSource{std::vector<uint8_t>data;const uint8_t*GetData(){return data.data();}size_t GetLength(){return data.size();}};
 const Entry*resource_table_=nullptr;size_t resource_count_=0;const Alias*alias_table_=nullptr;size_t alias_count_=0;TextEncodingType text_encoding_type_=BINARY;std::unique_ptr<DataSource>data_source_;
 base::expected<void,FailureReason>SanityCheckFileAndRegisterResources(size_t,const uint8_t*,size_t);
 base::expected<void,FailureReason>LoadImpl(std::unique_ptr<DataSource>);
 const Entry*LookupEntryById(uint16_t)const;
};
'''+actual+r'''
int main(int argc,char**argv){if(argc!=2)return 2;std::ifstream input(argv[1],std::ios::binary);auto source=std::make_unique<DataPack::DataSource>();source->data=std::vector<uint8_t>(std::istreambuf_iterator<char>(input),{});DataPack pack;bool loaded=pack.LoadImpl(std::move(source)).good;std::cout<<(loaded?1:0)<<"\n";if(loaded){for(uint16_t id:{52490,52853,53015}){const auto*entry=pack.LookupEntryById(id);std::cout<<id<<":"<<(entry?std::to_string(entry->resource_id):"missing")<<"\n";}}}
'''
(PROOF / 'actual-native.cc').write_text(fixture_cpp)
subprocess.run(['xcrun','clang++','-std=c++20','-O2','-Wall','-Wextra','-Werror',str(PROOF/'actual-native.cc'),'-o',str(PROOF/'actual-native')],check=True)


def native_load(path):
    r = subprocess.run([str(PROOF/'actual-native'),str(path)],text=True,capture_output=True,check=True)
    return r.stdout.splitlines()[0] == '1', r.stdout


def bytes_case(name, mutate, error, native_accept=False):
    layout, root = fixture('sanity-'+name)
    for p in [layout.intermediate_pak,layout.built_pak,layout.packaged_pak]:
        p.write_bytes(mutate(p.read_bytes()))
    accepted, stdout = native_load(layout.built_pak)
    assert accepted == native_accept,(name,stdout)
    try:m.check(layout,root/'receipt','present')
    except RuntimeError as e:
        assert error in str(e),(name,str(e))
        assert not (root/'receipt/receipt.json').exists()
        results.append({'name':name,'status':'expected rejection','reason':str(e),'actualNativeLoad':accepted,'nativeOutput':stdout})
    else:raise AssertionError(name+' accepted')


def replace(data, offset, format, value):
    data=bytearray(data);struct.pack_into(format,data,offset,value);return bytes(data)


bytes_case('invalid-encoding-review',lambda d:replace(d,4,'B',255),'bad encoding')
bytes_case('terminal-offset-past-end-review',lambda d:replace(d,12+3*6+2,'<I',len(d)+1),'offset past EOF')
bytes_case('first-offset-past-end',lambda d:replace(d,14,'<I',len(d)+1),'offset past EOF')
bytes_case('decreasing-offset',lambda d:replace(d,12+6+2,'<I',struct.unpack_from('<I',d,14)[0]-1),'offsets not monotone')
bytes_case('table-truncated',lambda d:d[:20],'tables exceed file size')
bytes_case('header-truncated',lambda d:d[:8],'incomplete V5 header')
bytes_case('unsupported-version',lambda d:replace(d,0,'<I',6),'V5 only')
bytes_case('resource-count-overflow-table',lambda d:replace(d,8,'<H',65535),'tables exceed file size')
bytes_case('alias-count-overflow-table',lambda d:replace(d,10,'<H',65535),'tables exceed file size')
bytes_case('duplicate-canonical-id',lambda d:replace(d,18,'<H',52490),'canonical IDs not sorted unique',True)


def unsorted_payloads(data):
    count=3;entries=[struct.unpack_from('<HI',data,12+i*6)for i in range(count+1)]
    payloads=[data[entries[i][1]:entries[i+1][1]]for i in range(count)]
    result=bytearray(data[:12]);offset=12+(count+1)*6
    for i in [1,0,2]:result.extend(struct.pack('<HI',entries[i][0],offset));offset+=len(payloads[i])
    result.extend(struct.pack('<HI',0,offset));return bytes(result)+b''.join(payloads[i]for i in [1,0,2])


bytes_case('unsorted-canonical-correct-payloads',unsorted_payloads,'canonical IDs not sorted unique',True)
assert '52853:missing' in results[-1]['nativeOutput']
# Preserve actual native acceptance rather than inventing stricter checks.
for name,mutation in [('binary-encoding',lambda d:replace(d,4,'B',0)),('utf16-encoding',lambda d:replace(d,4,'B',2)),('ignored-padding',lambda d:d[:5]+b'xyz'+d[8:]),('nonzero-terminal-id',lambda d:replace(d,30,'<H',123)),('trailing-byte',lambda d:d+b'x'),('zero-length-first',lambda d:replace(d,14,'<I',struct.unpack_from('<I',d,20)[0])),('offset-within-tables',lambda d:replace(d,14,'<I',0))]:
    layout,root=fixture('native-accepted-'+name);data=mutation(layout.built_pak.read_bytes());layout.built_pak.write_bytes(data)
    assert native_load(layout.built_pak)[0];m.native_v5_sanity(data)
    results.append({'name':name,'status':'native and sanity accept'})
layout,root=fixture('aliases-extra');original=write({52490:b'a',52853:b'b',53015:b'c',60000:b'a',61000:b'b'},1)
for name,mutation,message,native_ok in [('bad-alias-index',lambda d:replace(d,36+2,'<H',3),'alias index out of bounds',False),('alias-unsorted',lambda d:d[:36]+d[40:44]+d[36:40]+d[44:],'alias IDs not sorted unique',True),('alias-duplicate',lambda d:replace(d,40,'<H',60000),'alias IDs not sorted unique',True),('alias-canonical-overlap',lambda d:replace(d,36,'<H',52490),'canonical and alias IDs overlap',True)]:
    data=mutation(original);p=root/(name+'.pak');p.write_bytes(data);assert native_load(p)[0]==native_ok
    try:m.native_v5_sanity(data)
    except RuntimeError as e:assert message in str(e);results.append({'name':name,'status':'expected rejection','actualNativeLoad':native_ok,'reason':str(e)})
    else:raise AssertionError(name+' accepted')
# Fixed current-build V5 scope: native V4 remains deliberately unsupported.
v4=struct.pack('<IIB',4,0,1)+struct.pack('<HI',0,15);p=PROOF/'empty-v4.pak';p.write_bytes(v4);assert native_load(p)[0]
try:m.native_v5_sanity(v4)
except RuntimeError as e:assert 'V5 only' in str(e);results.append({'name':'native-v4-policy-hold','status':'explicit unsupported policy','actualNativeLoad':True})
else:raise AssertionError('unsupported V4 accepted')
report={'scope':'private synthetic source-native C156 LoadImpl/SanityCheck/Lookup fixture +24 prior exact-resource regressions','proof':str(PROOF),'results':results}
(PROOF/'tests.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'groups':len(results),'status':'PASS','proof':str(PROOF)}))
