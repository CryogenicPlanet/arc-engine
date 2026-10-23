import os,shlex,subprocess,sys
from pathlib import Path
root=Path('/Users/cryogenicplanet/general/arc-engine');out=root/'chromium/src/out/Base';env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']}
r=subprocess.run(['siso','query','commands','-C','.','obj/arc/arc_core/arc_sessions_restore.o'],cwd=out,env=env,capture_output=True)
a=next(shlex.split(line) for line in reversed(r.stdout.decode(errors='replace').splitlines()) if ' -c ' in line and 'arc_sessions_restore.mm' in line)
a=[sys.argv[1] if x.endswith('/arc_sessions_restore.mm') else x for x in a];clean=[];i=0
while i<len(a):
 if a[i] in ('-o','-MF'):i+=2;continue
 if a[i]=='-MMD':i+=1;continue
 clean.append(a[i]);i+=1
r=subprocess.run(clean+['-ivfsoverlay','/Users/cryogenicplanet/general/arc-engine/results/sessions/proposals/close-accounting-20261023T1420Z/overlay.json','-fsyntax-only','-Wno-unused-command-line-argument'],cwd=out,env=env,capture_output=True)
print('compiler_exit',r.returncode);print(r.stderr.decode(errors='replace')[-9000:]);sys.exit(r.returncode)
