#!/usr/bin/env python3
"""Apply a small, explicit class/category/selector overlay; preserve initial evidence."""
import collections,csv,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'decompile'
data=[
 dict(id='profiles',cls='ArcBrowserApplication',cat='',sel='createBrowserContext:isHidden:type:completion:error:',va='0x43e8368',
      classification='confirmed_chrome_internal_dependency',chain='0x43e8368>0x42b3d74>0x7bdc470; callback=0x42b3f34',
      symbols='ProfileManager::CreateMultiProfileAsync; ProfileManager::CreateProfileAsync',sources='primary-sources.tsv#0',
      app='0x1004af488',evidence='APP body 0x1004af380 calls arcBrowserApplication getter, copies block, supplies hidden=0/type=0, checks BOOL/NSError; engine name/null callback validation and async profile path',
      remaining='ObjC completion label 0x43eb7a0; type=1 behavior; executor, cancellation, profile migration/encryption'),
 dict(id='autocomplete_factory',cls='ArcBrowserContext',cat='Extensions',sel='createAutocompleteControllerWithProviders:',va='0x43f5f60',
      classification='confirmed_chrome_internal_dependency',chain='0x43f5f60>0x42c2f5c; 0x43ecc2c>0x42c35e4>0x20cd850',
      symbols='AutocompleteController::Start',sources='primary-sources.tsv#3',app='0x1004dbe84',
      evidence='Category factory constructs controller from low two provider bits; context controller dispatch reaches source-backed stock omnibox Start',
      remaining='Provider-client identity and bit semantics; callback replacement/results; no short-selector BL found'),
 dict(id='autocomplete_browser',cls='ArcBrowser',cat='',sel='autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:',va='0x43e1f80',
      classification='confirmed_chrome_internal_dependency',chain='0x43e1f80>0x42fbd80>0x20cd850',symbols='AutocompleteController::Start',
      sources='primary-sources.tsv#3',app='0x100527850',
      evidence='APP 0x100527668 loads ChromiumBrowser::browser, passes cursor/inline/startFresh/keyword/NSError/completion; backend checks browser lifetime and keyword, builds input, starts controller',
      remaining='Provider customization/ranking and async result ordering; callback label 0x43e5fd4'),
 dict(id='webstore',cls='ArcBrowserContextExtensions',cat='',sel='installExtensionFromWebstore:unattended:completion:',va='0x43fd304',
      classification='confirmed_chrome_internal_dependency',chain='0x43fd304>0x42e6790>0x786cd50; 0x42e6790>0x784a050; callback=0x42e712c',
      symbols='extensions::WebstoreInstallWithPrompt constructor; WebstoreStandaloneInstaller::BeginInstall',sources='primary-sources.tsv#1,#2',
      app='0x100401ee8,0x1004d867c,0x1004d87a4,0x1004db1b0',
      evidence='APP 0x100401e54 obtains extensions, calls with unattended=1 and copied block. Backend selects window-aware or null-window construction; unattended branch substitutes vtables and clears +0x168 byte; completion callbacks fan out',
      remaining='Unattended prompt vtable targets; exact result-code mapping; ObjC block label 0x43fec84; no runtime install proof'),
 dict(id='extension_pin_order',cls='ArcBrowserContextExtensions',cat='',sel='setPinnedExtensionOrder:',va='0x43fdd3c',
      classification='confirmed_chrome_internal_dependency',chain='0x43fdd3c>0x42eb57c>0x7cd94c0,0x7cd9ea0,0x7cd9f40',
      symbols='ToolbarActionsModel::SetActionVisibility; ToolbarActionsModel::IsActionPinned; ToolbarActionsModel::MovePinnedAction',sources='primary-sources.tsv#4,#5',
      app='0x1004d94a8',evidence='APP obtains extensions then sets array; backend deduplicates requested IDs, changes pin membership and order through source-backed toolbar operations',
      remaining='Policy/incognito/disabled-extension edge cases and delegate notification order'),
 dict(id='password_save',cls='ArcLoginCredentialsSaveRequest',cat='',sel='save',va='0x443f038',
      classification='confirmed_chrome_internal_dependency',chain='0x443f038>0x7d64988>0x78b2e84',
      symbols='SaveUpdateBubbleController::OnSaveClicked; ManagePasswordsUIController::SavePassword',sources='primary-sources.tsv#6,#7',app='',
      evidence='Bubble target checks pending save/update states, sets dismissal=1, cleans statistics then calls SavePassword with pending username/password; delegate contains passwords_account_storage_used plus compromise-helper pattern',
      remaining='No direct APP BL to save stub found; current app-receiver use and full weak lifetime/executor not certified'),
 dict(id='native_pane',cls='ArcWebContents',cat='Extensions',sel='attachToPaneHostView:',va='0x4410cd0',
      classification='arc_native_bridge_with_content_internal_dependency',chain='0x4410cd0>0x4739114; 0x4737eb4>0x473cad8>0x6846c88; 0x4737eb4>0x473cf0c',
      symbols='RenderWidgetHostViewMac::TakeFallbackContentFrom; WebContentsHostView; WebContentsPaneHostView; ViewsHostable',sources='primary-sources.tsv#8,#9',
      app='0x100542704',evidence='APP createPaneHostView selector dispatch; engine exact host-class/ViewsHostable checks, four-anchor NSView wrapping, weak observer and content compositor fallback transfer',
      remaining='APP direct attach BL absent; branch/indirect attach paths, frame ownership, resizing and compositor behavior require runtime checks'),
 dict(id='permission_base',cls='ArcPermissionRequest',cat='',sel='additionalData',va='0x2d0fb40',
      classification='arc_specific_behavior',chain='0x2d0fb40',symbols='shared offset-8 ObjC getter; Ghidra alias WebNNMLFeatureProvider::featureNames',sources='',
      app='0x1004cf140,0x1004d16c8,0x1004d2224,0x10057a7e4,0x10057a888',
      evidence='Exact base IMP returns object ivar at +8; alias name cannot identify receiver class or dictionary semantics',
      remaining='Class-qualified ivar identity and effective category dispatch; shared selector APP receiver unproven'),
 dict(id='permission_category',cls='ArcPermissionRequest',cat='Extensions',sel='additionalData',va='0x440a85c',
      classification='arc_specific_behavior',chain='0x440a85c',symbols='ArcPermissionRequest Extensions dictionary conversion',sources='',
      app='0x1004cf140,0x1004d16c8,0x1004d2224,0x10057a7e4,0x10057a888',
      evidence='Creates NSMutableDictionary, iterates optional underlying map, converts integer keys to NSNumber and value strings, returns copy; separate from base getter',
      remaining='Key enum/schema and effective runtime method selection; CEF permission callback alone does not establish dictionary contract'),
]
# Bind app evidence to the actual binary-validated calls, avoiding hand-copied VA drift.
calls=list(csv.DictReader((OUT/'evidence/app-calls.tsv').open(),delimiter='\t'))
for r in data:
 selector='createPaneHostView' if r['id']=='native_pane' else r['sel']
 r['app']=','.join(x['app_call_va'] for x in calls if x['selector']==selector)
 r['kind']='-';r['confidence']='high_source_backed_identity_not_semantic_equivalence' if r['sources'] else 'direct_pseudocode_behavior_only'
with (OUT/'route-findings.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(data[0]),delimiter='\t');w.writeheader();w.writerows(data)
overlay={(r['cls'],r['cat'],r['kind'],r['sel']):r for r in data}
rows=list(csv.DictReader((ROOT/'initial/cef-mapping.tsv').open(),delimiter='\t'))
changes=[]
for r in rows:
 key=(r['class_name'],r['objc_category'],r['kind'],r['selector']);new=overlay.get(key)
 r['initial_category']=r['category'];r['stage2_finding_id']='';r['stage2_scope']='unchanged_initial_provisional_mapping'
 if new:
  r['category']=new['classification'];r['stage2_finding_id']=new['id'];r['stage2_scope']='bounded_engine_body_review'
  changes.append(dict(class_name=key[0],objc_category=key[1],kind=key[2],selector=key[3],engine_va=r['engine170_imp_va'],
                      initial_category=r['initial_category'],updated_category=r['category'],finding_id=new['id']))
  assert r['engine170_imp_va']==new['va']
with (OUT/'cef-mapping.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
with (OUT/'mapping-changes.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(changes[0]),delimiter='\t');w.writeheader();w.writerows(changes)
counts=dict(collections.Counter(r['category'] for r in rows))
assert len(rows)==834 and len(changes)==5
(OUT/'evidence/mapping-validation.json').write_text(json.dumps(dict(imported_declared_rows=len(rows),changed_rows=len(changes),categories=counts,expanded_route_findings=len(data)),indent=2)+'\n')
print(json.dumps(counts))
