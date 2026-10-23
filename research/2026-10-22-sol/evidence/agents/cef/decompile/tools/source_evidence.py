#!/usr/bin/env python3
"""Hash and excerpt exact pinned primary-source anchors used in this review."""
import csv, hashlib, json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]; OUT=ROOT/'decompile/evidence'
CHROMIUM=Path('/home/exedev/work/inputs/chromium-156')
C='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
F='426920c33ef22551e0b8f48cf343d623ac25642d'
spec=[
 ('chromium','chrome/browser/profiles/profile_manager.cc','void ProfileManager::CreateMultiProfileAsync',1037,1106),
 ('chromium','chrome/browser/extensions/webstore_install_with_prompt.cc','WebstoreInstallWithPrompt::WebstoreInstallWithPrompt',17,33),
 ('chromium','chrome/browser/extensions/webstore_standalone_installer.cc','void WebstoreStandaloneInstaller::BeginInstall()',54,98),
 ('chromium','components/omnibox/browser/autocomplete_controller.cc','void AutocompleteController::Start(',631,760),
 ('chromium','chrome/browser/ui/toolbar/toolbar_actions_model.cc','bool ToolbarActionsModel::IsActionPinned(',476,590),
 ('chromium','chrome/browser/ui/toolbar/toolbar_actions_model.cc','void ToolbarActionsModel::SetActionVisibility(',688,712),
 ('chromium','chrome/browser/ui/passwords/bubble_controllers/save_update_bubble_controller.cc','void SaveUpdateBubbleController::OnSaveClicked()',140,151),
 ('chromium','chrome/browser/ui/passwords/manage_passwords_ui_controller.cc','void ManagePasswordsUIController::SavePassword(',961,1012),
 ('chromium','content/browser/renderer_host/render_widget_host_view_mac.mm','void RenderWidgetHostViewMac::TakeFallbackContentFrom(',1229,1247),
 ('cef','include/internal/cef_types_mac.h','Alloy style will always be used',100,156),
 ('cef','include/cef_request_context.h','static CefRefPtr<CefRequestContext> CreateContext(',108,157),
 ('cef','include/cef_auth_callback.h','class CefAuthCallback',42,70),
 ('cef','CHROMIUM_BUILD_COMPATIBILITY.txt','156.0.8078.48',1,20),
 ('chromium','chrome/browser/extensions/webstore_standalone_installer.h','virtual std::unique_ptr<InstallPromptData> CreateInstallPrompt()',100,143),
 ('chromium','chrome/browser/extensions/webstore_standalone_installer.cc','void WebstoreStandaloneInstaller::ProceedWithInstallPrompt()',136,148),
]
rows=[]
for project,rel,anchor,start,end in spec:
 path=(CHROMIUM if project=='chromium' else ROOT/'initial/sources/cef')/rel
 b=path.read_bytes(); lines=b.decode().splitlines(); actual=[i for i,s in enumerate(lines,1) if anchor in s]
 assert actual,(str(path),anchor)
 if start==end==1:start=max(1,actual[0]-8);end=min(len(lines),actual[0]+35)
 excerpt=OUT/f'source-{len(rows):02d}.txt'
 excerpt.write_text('\n'.join(f'{i}: {lines[i-1]}' for i in range(start,min(end,len(lines))+1))+'\n')
 rev=C if project=='chromium' else F
 url=(f'https://chromium.googlesource.com/chromium/src/+/{rev}/{rel}' if project=='chromium'
      else f'https://github.com/chromiumembedded/cef/blob/{rev}/{rel}')
 rows.append(dict(project=project,revision=rev,source_path=str(path),relative_path=rel,
                  sha256=hashlib.sha256(b).hexdigest(),anchor=anchor,anchor_lines=','.join(map(str,actual)),
                  excerpt_start=start,excerpt_end=end,local_excerpt=str(excerpt.relative_to(ROOT/'decompile')),url=url))
with (OUT/'primary-sources.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
print(json.dumps(dict(source_anchors=len(rows),revisions=dict(chromium=C,cef=F))))
