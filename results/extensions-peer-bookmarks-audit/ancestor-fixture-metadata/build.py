from pathlib import Path
import subprocess,sys
p=Path(__file__).resolve().parent
commands=[('compile.log',['xcrun','clang++','-dynamiclib','-fobjc-arc','-std=c++20','-Wall','-Wextra','-Werror','-framework','AppKit',str(p/'probe.mm'),'-o',str(p/'probe.dylib')]),
 ('codesign.log',['codesign','--force','--sign','-',str(p/'probe.dylib')]),
 ('codesign-verify.log',['codesign','--verify','--strict',str(p/'probe.dylib')]),
 ('bun-build.log',['bun','build','--target','bun',str(p/'run.ts'),'--outfile',str(p/'run.bundle.js')]),
 ('bun-scenario-build.log',['bun','build','--target','bun',str(p/'scenario.ts'),'--outfile',str(p/'scenario.bundle.js')]),
 ('python-compile.log',[sys.executable,'-m','py_compile',str(p/'coverage.py')])]
for name,args in commands:
 r=subprocess.run(args,capture_output=True,text=True)
 (p/name).write_text('COMMAND '+repr(args)+'\nACTUAL_EXIT '+str(r.returncode)+'\n'+r.stdout+r.stderr)
 print(name,'ACTUAL_EXIT',r.returncode)
 if r.returncode:sys.exit(r.returncode)
