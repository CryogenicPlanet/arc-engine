#!/usr/bin/env python3
import csv,hashlib,json,subprocess
from pathlib import Path
O=Path(__file__).resolve().parents[1]; W=O.parent.parent/'spec-1/specs'; I=Path('/home/exedev/work/inputs/chromium-156')
CH='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'; CE='426920c33ef22551e0b8f48cf343d623ac25642d'
paths={'chromium':['services/media_session/public/cpp/media_position.h','services/media_session/public/cpp/media_position.cc','components/security_state/core/security_state.h','components/site_engagement/content/site_engagement_service.h','components/sessions/core/serialized_navigation_entry.h','chrome/browser/ui/toolbar/toolbar_actions_model.h','chrome/browser/ui/toolbar/toolbar_actions_model.cc','content/public/browser/web_contents.h','content/browser/web_contents/web_contents_view_mac.mm'],'cef':['include/internal/cef_types_mac.h','include/cef_browser.h']}
rows=[]
for repo,files in paths.items():
 pin=CH if repo=='chromium' else CE
 for p in files:
  url=f'https://raw.githubusercontent.com/{"chromium/chromium" if repo=="chromium" else "chromiumembedded/cef"}/{pin}/{p}'
  local=I/p if repo=='chromium' else None
  out=O/'sources'/repo/p;out.parent.mkdir(parents=True,exist_ok=True)
  if local and local.exists():data=local.read_bytes();out.write_bytes(data);origin=str(local)
  else:
   subprocess.run(['curl','--fail','--silent','--show-error','--location','--max-time','45',url,'--output',str(out)],check=True);data=out.read_bytes();origin='curl exact revision'
  writer=W/'sources'/repo/p
  rows.append(dict(repo=repo,revision=pin,path=p,url=url,origin=origin,sha256=hashlib.sha256(data).hexdigest(),writer_copy_equal=writer.read_bytes()==data if writer.exists() else 'not separately copied by writer',evidence=str(out)))
with (O/'evidence/source-checks.tsv').open('w') as f:
 w=csv.DictWriter(f,list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
print(json.dumps(rows,indent=2))
