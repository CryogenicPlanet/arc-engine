from pathlib import Path
import urllib.request,json,hashlib
rev='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
paths=['services/media_session/public/cpp/media_position.h','services/media_session/public/cpp/media_position.cc','chrome/browser/ui/toolbar/toolbar_actions_model.h','chrome/browser/ui/toolbar/toolbar_actions_model.cc','components/autofill/core/browser/ui/addresses/autofill_address_profile_save_ui_handler.h','components/autofill/core/browser/ui/addresses/autofill_address_profile_save_ui_handler.cc','components/security_state/core/security_state.h','components/security_state/core/security_state.cc','components/sessions/core/serialized_navigation_entry.h']
rows=[]
for p in paths:
 url=f'https://raw.githubusercontent.com/chromium/chromium/{rev}/{p}'
 try:
  data=urllib.request.urlopen(url,timeout=25).read();dest=Path('specs/sources/chromium')/p;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(data);rows.append(dict(commit=rev,path=p,url=url,sha256=hashlib.sha256(data).hexdigest(),status='ok'))
 except Exception as e:rows.append(dict(commit=rev,path=p,url=url,status=str(e)))
 print(p,rows[-1]['status'])
Path('specs/sources/manifest.json').write_text(json.dumps(rows,indent=2))
