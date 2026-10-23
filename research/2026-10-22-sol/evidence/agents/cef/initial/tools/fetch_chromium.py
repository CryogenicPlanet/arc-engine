#!/usr/bin/env python3
"""Fetch a small set of Chromium public primary-source interface evidence."""
import base64, concurrent.futures, hashlib, json, pathlib, urllib.request
ROOT=pathlib.Path(__file__).resolve().parents[1]
REV=json.loads((ROOT/'sources/chromium-tag.json').read_text().split('\n',1)[1])['commit']
PATHS=['chrome/VERSION','components/history/core/browser/history_service.h',
       'components/history/core/browser/history_types.h',
       'components/omnibox/browser/autocomplete_controller.h',
       'components/omnibox/browser/autocomplete_match.h',
       'components/bookmarks/browser/bookmark_model.h',
       'components/search_engines/template_url_service.h',
       'chrome/browser/profiles/profile.h',
       'chrome/browser/profiles/profile_manager.h',
       'chrome/browser/extensions/extension_service.h',
       'chrome/browser/extensions/api/passwords_private/passwords_private_api.h',
       'components/password_manager/core/browser/password_form_manager_for_ui.h',
       'content/public/browser/web_contents.h',
       'content/public/browser/web_contents_delegate.h',
       'chrome/browser/webauthn/chrome_authenticator_request_delegate.h',
       'chrome/browser/media/webrtc/chrome_media_access_handler.h',
       'chrome/common/chrome_content_client.cc',
       'third_party/widevine/cdm/widevine_cdm_common.h']
def fetch(path):
    url=f'https://chromium.googlesource.com/chromium/src/+/{REV}/{path}'
    dest=ROOT/'sources/chromium'/path
    dest.parent.mkdir(parents=True,exist_ok=True)
    try:
        if not dest.exists():
            with urllib.request.urlopen(url+'?format=TEXT',timeout=30) as response:
                data=base64.b64decode(response.read(2_000_000))
            dest.write_bytes(data)
        data=dest.read_bytes()
        return dict(path=path,url=url,revision=REV,bytes=len(data),sha256=hashlib.sha256(data).hexdigest(),status='ok')
    except Exception as e:return dict(path=path,url=url,revision=REV,status=str(e))
with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:manifest=list(pool.map(fetch,PATHS))
(ROOT/'sources/chromium-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
for x in manifest:print(x['status'],x['path'])
