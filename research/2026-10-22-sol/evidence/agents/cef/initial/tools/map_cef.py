#!/usr/bin/env python3
"""Generate conservative, contract-level CEF mappings from reviewed API rules.

Direct means one public CEF operation covers the stated operation, with normal
ObjC type conversion still needed. No row certifies Arc behavior or ABI parity.
Unresolved means no sufficient positive mapping/dependency evidence, not absence.
"""
import collections,csv,json,pathlib,re
from inventory import ROOT,write_tsv
CEF=json.loads((ROOT/'sources/cef-target.json').read_text())['sha']
CHR=json.loads((ROOT/'sources/chromium-tag.json').read_text().split('\n',1)[1])['commit']
BASE=f'https://github.com/chromiumembedded/cef/blob/{CEF}/'
CHBASE=f'https://chromium.googlesource.com/chromium/src/+/{CHR}/'
DIRECT='direct_cef_public'; PARTIAL='partial_cef_plus_bridge'; ARC='arc_specific_behavior'; UNKNOWN='unresolved'; INTERNAL='chrome_internal_dependency'
RULES={}

def rule(cls,selectors,api,path,rationale,category=PARTIAL,runtime='both',thread='per cited API; Arc callback thread/ownership unverified'):
    """Explicit selector keys. API must exist in fetched revision."""
    for selector in selectors.split('|'):
        assert (cls,selector) not in RULES,(cls,selector)
        text=(ROOT/'sources/cef'/path).read_text()
        # The last API token is checked in addition to a manual source review.
        for component in api.split(';'):
            token=component.strip().split('::')[-1].split('(')[0]
            assert re.search(r'\b'+re.escape(token)+r'\b',text), (api,path)
        RULES[(cls,selector)]=dict(category=category,cef_api=api,evidence_url=BASE+path,
            evidence_revision=CEF,evidence_kind='pinned_public_CEF_API',confidence='medium',
            rationale=rationale,runtime=runtime,thread_ownership=thread)

# Read-only download values have exact public getters. ArcDownloadItem must be
# an app-owned snapshot: CEF explicitly forbids retaining callback item objects.
for selector,api in {'completedBytes':'GetReceivedBytes','currentSpeed':'GetCurrentSpeed',
 'totalBytes':'GetTotalBytes','startTime':'GetStartTime','targetPath':'GetFullPath',
 'id':'GetId','isPaused':'IsPaused'}.items():
    rule('ArcDownloadItem',selector,'CefDownloadItem::'+api,'include/cef_download_item.h',
         'Same documented download value; copy into an Arc-owned snapshot during CEF download callbacks. Type conversion and behavioral validation remain.',DIRECT,
         thread='Read inside CefDownloadHandler callback on UI thread; do not retain CefDownloadItem after callback.')
for sel,api in [('pause','Pause'),('resume','Resume'),('cancel:','Cancel')]:
    rule('ArcDownloadItem',sel,'CefDownloadItemCallback::'+api,'include/cef_download_handler.h',
         'Public action exists through the callback, not through CefDownloadItem. Retain appropriate callback separately; Arc cancel argument semantics and completion ordering need decompile.')
rule('ArcDownloadItem','sourceURL','CefDownloadItem::GetURL; CefDownloadItem::GetOriginalUrl','include/cef_download_item.h',
     'CEF exposes current and original URL; Arc sourceURL choice is unknown.')
rule('ArcDownloadItem','isDone|currentState','CefDownloadItem::IsInProgress; CefDownloadItem::IsComplete; CefDownloadItem::IsCanceled; CefDownloadItem::IsInterrupted','include/cef_download_item.h',
     'Translate CEF state predicates into Arc state enum; terminal-state and interruption policy need behavior evidence.')
rule('ArcDownloadItem','webContents','CefDownloadHandler::OnDownloadUpdated','include/cef_download_handler.h',
     'Associate the callback browser with the Arc web-contents wrapper; CEF does not return ArcWebContents.')

for sel,api in [('title','GetTitle'),('url','GetURL'),('isValid','IsValid')]:
    rule('ArcBackForwardListItem',sel,'CefNavigationEntry::'+api,'include/cef_navigation_entry.h',
         'Public navigation-entry getter covers this value. Arc validity duration and URL normalization remain to validate.',DIRECT)

# Fields that are actually present in the public cookie structure.
for field,cef in {'name':'name','value':'value','domain':'domain','path':'path','secure':'secure',
                  'httpOnly':'httponly','sameSite':'same_site','expirationDate':'expires',
                  'session':'has_expires'}.items():
    setter='set'+field[0].upper()+field[1:]+':'
    rule('ArcCookie',field+'|'+setter,'cef_cookie_t; '+cef,'include/internal/cef_types.h',
         'Copy the public cookie field; session inverts has_expires, time/enum/string conversion and expiry policy require Arc validation.')
rule('ArcCookie','init','cef_cookie_t','include/internal/cef_types.h',
     'Allocate an Arc ObjC value wrapper around copied cookie fields; defaults and storage ownership need implementation.')
rule('ArcBrowserContext','deleteAllCookies:','CefCookieManager::DeleteCookies','include/cef_cookie.h',
     'Empty URL and cookie-name filters delete all cookies. Bridge async callback and context; Arc argument meaning and completion thread remain unverified.')
rule('ArcBrowserContext','getAllCookies:|getAllCookies:completion:','CefCookieManager::VisitAllCookies','include/cef_cookie.h',
     'Visitor enumerates cookies ordered by path then creation date. Aggregate ArcCookie snapshots and bridge completion; Arc filter overload remains unknown.')
rule('ArcBrowserContext','getContentSettingForOrigin:withContentSettingType:','CefRequestContext::GetContentSetting','include/cef_request_context.h',
     'Public getter takes requesting and top-level URLs; Arc supplies one origin. Origin scoping and content-type enum translation must be recovered.',thread='CEF browser UI thread; Arc synchronous call-thread contract unknown.')
rule('ArcBrowserContext','setContentSettingsForOrigin:withContentSettingTypes:withSiteSetting:|setPermissionsForOrigin:withPermissionTypes:withSiteSetting:',
     'CefRequestContext::SetContentSetting','include/cef_request_context.h',
     'Public setter covers individual content settings. Bridge batch, permission-to-content-type enums and two-URL scoping; policy/persistence parity is unverified.',thread='CEF browser UI thread; enum and policy validation required.')
rule('ArcBrowserContext','isEqual:','CefRequestContext::IsSame','include/cef_request_context.h',
     'Public context identity predicate exists. Must establish whether Arc equality means same profile, storage sharing, or wrapper identity.')

for cls in ('ArcBrowserApplication','ArcBrowserContext'):
    rule(cls,'loadUrl:withDelegate:','CefURLRequest::Create','include/cef_urlrequest.h',
         'Context-specific network requests are supported. Build CefRequest and translate delegate streaming, errors, redirects, timeouts and cancellation; no Arc delegate parity proof.',thread='Request object and callbacks stay on the creating CEF thread; crossing to Arc/Swift thread requires explicit dispatch.')
    rule(cls,'cancelUrlLoad:','CefURLRequest::Cancel','include/cef_urlrequest.h',
         'Keep token-to-request identity and cancel on creating thread; Arc cancellation/completion race semantics need decompile.')
rule('ArcBrowserContext','createWebContents:','CefBrowserHost::CreateBrowser','include/cef_browser.h',
     'Browser factory can supply a context and settings, but ArcWebContents identity, native view attachment and synchronous return versus async creation require a bridge.',runtime='Alloy for macOS parent NSView; Chrome style requires alternate hosting')
rule('ArcBrowserContext','downloads','CefDownloadHandler::OnDownloadUpdated','include/cef_download_handler.h',
     'CEF supports callbacks per browser. Context-wide persisted download collection and ArcBrowserContextDownloads wrapper are extra state; not a profile download-service mapping.')
for sel in ('createBrowserContext:isHidden:type:completion:error:','getBrowserContext:type:','loadBrowserContext:type:completion:'):
    rule('ArcBrowserApplication',sel,'CefRequestContext::CreateContext','include/cef_request_context.h',
         'CefRequestContext supports isolated/shared storage contexts. Arc named profile types, hidden state, load failures, lifecycle and existing profile migration are extra semantics.')
for sel in ('createBrowser:withDelegate:forWindow:','createPopupBrowser:withDelegate:'):
    rule('ArcBrowserApplication',sel,'CefBrowserHost::CreateBrowser','include/cef_browser.h',
         'CEF factory creates a browser with request context and client handlers. NSWindow/NSView hosting, popup disposition, ArcBrowser grouping and callbacks require a bridge.',runtime='Alloy for macOS parent NSView; Chrome style alternative unproven')
for sel,api in [('initialize:','CefInitialize'),('run','CefRunMessageLoop'),('quit','CefQuitMessageLoop')]:
    rule('ArcBrowserApplication',sel,api,'include/cef_app.h',
         'Corresponding process lifecycle primitive exists. Preserve app-owned Cocoa event loop, helper bootstrap, initialization sequencing and shutdown; Arc lifecycle parity is unverified.',thread='CEF main application thread; macOS cannot use multi_threaded_message_loop.')
rule('ArcBrowserApplication','tryToTerminate:','CefBrowserHost::TryCloseBrowser; CefBrowserHost::CloseBrowser','include/cef_browser.h',
     'CEF close methods permit beforeunload handling per browser. Arc app-level termination aggregation, callback order and final CefShutdown remain a bridge task.')
rule('ArcBrowserApplication','version','cef_version_info','include/cef_version_info.h',
     'Chromium/CEF numeric version information is public; Arc engine version string format and compatibility signaling are app-specific.')
rule('ArcBrowserApplication','devToolsWebSocketDebugURL','CefSettings; remote_debugging_port','include/internal/cef_types.h',
     'Public remote debugging port enables discovery, but exact browser-wide WebSocket URL discovery/lifetime and Arc formatting need additional work.')
rule('ArcBrowserApplication','enableAccessibilityForScreenReader|ensureBasicAccessibilityEnabled|disableAccessibility','CefBrowserHost::SetAccessibilityState','include/cef_browser.h',
     'CEF offers per-browser accessibility state. Arc application-wide mode, observer fan-out and screen-reader activation semantics require a bridge.')
rule('ArcBrowserApplication','tracingController','CefBeginTracing; CefEndTracing','include/cef_trace.h',
     'Public begin/end tracing can back a controller; options, privacy filtering, duration, completion and file lifecycle require a bridge.')
rule('ArcCrashKeysReporter','setArcCrashKeysObserver:','CefSetCrashKeyValue','include/cef_crash_util.h',
     'CEF accepts crash keys; an Arc observer protocol is not supplied. Need key enumeration, updates and observation ownership. Does not cover crash reporting backend equivalence.')

for sel,api in {'url':'SetURL','method':'SetMethod','headers':'SetHeaderMap'}.items():
    rule('ArcURLLoadConfiguration',sel+'|set'+sel[0].upper()+sel[1:]+':','CefRequest::'+api,'include/cef_request.h',
         'Public request property exists. Arc configuration remains a mutable ObjC value and is copied/frozen when dispatched; header normalization needs validation.')
for field in ('payload','payloadData','payloadFileURL','payloadContentType'):
    rule('ArcURLLoadConfiguration',field+'|set'+field[0].upper()+field[1:]+':','CefRequest::SetPostData','include/cef_request.h',
         'Translate Arc payload bytes/file/content type into CefPostData and headers; conflicting inputs and encoding/error policy need behavior evidence.')
rule('ArcURLLoadConfiguration','cachePolicy|setCachePolicy:|followRedirects|setFollowRedirects:','CefRequest::SetFlags','include/cef_request.h',
     'Request flags provide cache/redirect controls. Arc policy enum, exact redirect policy and defaults are unknown.')
rule('ArcURLLoadConfiguration','streamResponseData|setStreamResponseData:','CefURLRequestClient::OnDownloadData','include/cef_urlrequest.h',
     'CEF streams response chunks. Bridge buffered-versus-streaming policy and delivery/cancellation ordering.')

for sel in ('name','role','value','description','children','childTreeId','nodeId','isClickable','isEditable','isFocusable'):
    rule('ArcAccessibilityTreeNode',sel,'CefAccessibilityHandler::OnAccessibilityTreeChange','include/cef_accessibility_handler.h',
         'Accessibility updates can populate an app-owned Arc tree model. Exact dictionary keys, role enum, incremental-tree lifetime and cross-tree IDs need verification; no direct per-node API claim.',runtime='Alloy callback path; Chrome-style equivalence unverified')
for sel in ('type','originUrl','additionalData'):
    rule('ArcPermissionRequest',sel,'CefPermissionHandler::OnShowPermissionPrompt; CefPermissionHandler::OnRequestMediaAccessPermission','include/cef_permission_handler.h',
         'CEF permission/media callbacks expose origin and permission flags. Translate Arc request model and extra data; request ownership and callback scope are not yet known.')
for sel in ('contentSettingsType','permissionType','originUrl','siteSetting','setContentSettingsType:','setOriginUrl:','setSiteSetting:'):
    rule('ArcSiteSettingInfo',sel,'CefRequestContext::GetContentSetting; CefRequestContext::SetContentSetting','include/cef_request_context.h',
         'Represent content-setting query/set values in an Arc DTO. Permission enums, one-versus-two-origin scoping and default/inherited sources require translation.')
for sel in ('r','g','b','a','initWithR:g:b:a:'):
    rule('ArcColor',sel,'cef_color_t','include/internal/cef_types.h',
         'CEF uses a 32-bit ARGB color. Convert Arc component representation; precision, alpha convention and constructor ranges need validation.')
rule('ArcKeyEventData','eventType|domCode|key|windowsVirtualKeyCode|initWithEventType:domCode:key:windowsVirtualKeyCode:',
     'cef_key_event_t','include/internal/cef_types.h',
     'CEF key events expose type/key-code/character information but not Arc DOM-code/key semantics directly. macOS native keycode conversion and event routing require a bridge.')

rule('ArcImageRep','isNull','CefImage::IsEmpty','include/cef_image.h',
     'Public emptiness predicate covers an image without representations; Arc null-versus-empty distinction remains to verify.',DIRECT)
rule('ArcImageRep','createFromPixelDataBGRA8888:height:scale:rowBytes:pixelData:','CefImage::AddBitmap','include/cef_image.h',
     'CEF accepts scale, pixel dimensions, color/alpha type and bytes. Repack Arc row stride if necessary; alpha convention, copy lifetime and allocation failures need a bridge.')
rule('ArcImageRep','copyPixelDataBGRA8888','CefImage::GetAsBitmap','include/cef_image.h',
     'Request BGRA bitmap and copy public CefBinaryValue; select representation, stride and ownership to match Arc.')
rule('ArcImageRep','encodeToJPEGWithQuality:','CefImage::GetAsJPEG','include/cef_image.h',
     'JPEG encoding exists; translate quality scale, image representation selection, alpha handling and return ownership.')
rule('ArcImageRep','encodeToPNGWithMethod:','CefImage::GetAsPNG','include/cef_image.h',
     'PNG encoding exists; Arc method option is unknown and CEF exposes only transparency choice and scale.')
rule('ArcImageRep','pixelWidth|pixelHeight','CefImage::GetRepresentationInfo','include/cef_image.h',
     'Public representation info returns pixel dimensions; Arc scale-selection rule must be preserved.')
rule('ArcImageRep','decodeFromData:scale:completion:|decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion:|dangerouslyDecodeFromData:scale:',
     'CefImage::AddPNG; CefImage::AddJPEG','include/cef_image.h',
     'Public PNG/JPEG decode is a subset. Arbitrary codec support, size/resource limits, desired-size resizing and asynchronous callbacks need an app implementation; no full codec equivalence claim.')

rule('ArcNavigation','url','CefNavigationEntry::GetURL','include/cef_navigation_entry.h',
     'CEF navigation entries expose URL, but ArcNavigation is a live navigation token; bind entry/event timing and redirect identity.')
rule('ArcNavigation','responseStatusCode','CefNavigationEntry::GetHttpStatusCode','include/cef_navigation_entry.h',
     'Public completed-entry status exists; Arc provisional navigation timing and failure status require event tracking.')
rule('ArcNavigation','responseHeaders','CefResponse::GetHeaderMap','include/cef_response.h',
     'CEF response headers are public through request callbacks; map the correct main-frame navigation response and copy values into Arc token.')
for sel in ('filePath','setFilePath:','tracingDuration','setTracingDuration:'):
    rule('ArcTracingConfiguration',sel,'CefEndTracing','include/cef_trace.h',
         'Trace output path can be passed to CefEndTracing; duration requires app scheduling, and Arc configuration is a separate DTO.')
for sel in ('argc','argv','profilePath','applicationID','init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:'):
    rule('ArcBrowserApplicationConfiguration',sel,'CefInitialize','include/cef_app.h',
         'CEF initialization accepts main arguments/settings. Arc initializer contains profile/session/app policy outside this API; convert and retain config separately.')
for sel in ('initiallyMuted','setInitiallyMuted:'):
    rule('ArcWebContentsConfiguration',sel,'CefBrowserHost::SetAudioMuted','include/cef_browser.h',
         'CEF per-browser audio mute operation exists; order it before Arc initial-render readiness to avoid startup audio races.')
for sel in ('initiallyHidden','setInitiallyHidden:','forcedViewSize','setForcedViewSize:'):
    rule('ArcWebContentsConfiguration',sel,'CefBrowserHost::WasHidden; CefBrowserHost::WasResized','include/cef_browser.h',
         'CEF provides visibility/resize notifications for windowless browsers; windowed NSView visibility/geometry requires AppKit bridge.',runtime='Alloy windowless notifications; native NSView bridge separate')
for sel in ('transparentBackground','setTransparentBackground:'):
    rule('ArcWebContentsConfiguration',sel,'cef_browser_settings_t; background_color','include/internal/cef_types.h',
         'CEF settings can request transparent background subject to rendering mode; Arc compositing and alpha behavior require validation.')
for sel in ('disallowContextMenus','setDisallowContextMenus:'):
    rule('ArcWebContentsConfiguration',sel,'CefContextMenuHandler::OnBeforeContextMenu','include/cef_context_menu_handler.h',
         'Clear or replace the public context-menu model; map Arc configuration and event ordering.')
for sel in ('disallowDownloads','setDisallowDownloads:'):
    rule('ArcWebContentsConfiguration',sel,'CefDownloadHandler::CanDownload','include/cef_download_handler.h',
         'CEF CanDownload gates user-initiated downloads but explicitly excludes StartDownload. Arc policy must cover both entry paths.')

# Nearby Chromium interfaces are investigation candidates, not proven call
# dependencies: the bounded arrived-pseudocode inspection does not identify
# the actual Chrome service implementations behind the unnamed forwarding calls.
FAMILIES=[
 (('ArcAutocompleteMatch','ArcAutocompleteMatchAdditionalData'),'components/omnibox/browser/autocomplete_match.h','AutocompleteMatch','Full omnibox result/provider/keyword/relevance contract; navigation history is not an autocomplete API.'),
 (('ArcHistoryItem','ArcHistoryQueryOptions','ArcMostVisitedURL'),'components/history/core/browser/history_service.h','history::HistoryService','Global history database/query/count/top-sites semantics are not covered by CefNavigationEntry (per-browser navigation history).'),
 (('ArcBookmarkTreeNode',),'components/bookmarks/browser/bookmark_model.h','bookmarks::BookmarkModel','Persistent bookmark IDs, UUIDs, hierarchy and observers require a profile-level bookmark model; candidate internal service only.'),
 (('ArcSearchEngine',),'components/search_engines/template_url_service.h','TemplateURLService','Search-provider keyword/template/activation contract needs Chrome search-engine semantics; CefRequest setters do not supply it.'),
 (('ArcExtensionInfo','ArcExtensionCommand','ArcExtensionToolbarAction'),'chrome/browser/extensions/extension_service.h','extensions::ExtensionService','Chrome extension state/action/command model requires investigation. Chrome-style extension execution is not public install/list/pinning API coverage.'),
 (('ArcLoginCredentialsSaveRequest',),'components/password_manager/core/browser/password_form_manager_for_ui.h','password_manager::PasswordFormManagerForUI','Arc form-save callbacks need dedicated password ownership semantics. CefAuthCallback handles HTTP authentication, not form-password saving.'),
 (('ArcSessionID','ArcSessionTab','ArcTabGroupInfo','ArcTabGroupResult'),'content/public/browser/web_contents.h','content::WebContents','Session IDs, persisted tab state and Chrome tab groups are not established by browser factory APIs.'),
 (('ArcBrowserContextAttributes',),'chrome/browser/profiles/profile_manager.h','ProfileManager','Profile catalog, directory identity, names and colors exceed the request-context storage concept.'),
]

def classify(row):
    cls,sel=row['class_name'],row['selector']
    base=dict(category=UNKNOWN,cef_api='',evidence_url=BASE+'include/',evidence_revision=CEF,
              evidence_kind='review_scope_not_absence_proof',confidence='low',runtime='unknown',
              rationale='No sufficient exact public CEF mapping or actual Chrome-internal call evidence in the available metadata; engine semantics required.',
              thread_ownership='Unknown: ObjC type encoding does not establish callback thread, retained/borrowed ownership, or async ordering.',
              candidate_internal_api='',candidate_internal_url='',candidate_internal_revision='')
    for classes,path,api,why in FAMILIES:
        if cls in classes:
            base.update(candidate_internal_api=api,candidate_internal_url=CHBASE+path,
                        candidate_internal_revision=CHR,rationale=why+' Actual dependency remains unproven.')
    if cls=='ArcBrowserContext' and any(k.lower() in sel.lower() for k in ('Bookmark','import','favicon','Autocomplete','history','extensions','preferences')):
        base['rationale']='Profile-level service/returned wrapper semantics need decompile. Browser display/navigation APIs alone do not establish this contract; no CEF-absence claim.'
    if cls=='ArcBrowserApplication' and any(k.lower() in sel.lower() for k in ('Encryption','Enterprise','Feature','Flag','histogram','GPULog','localState','Import','restoreBrowser','deleteBrowserContext','availableBrowserContext')):
        base['rationale']='Application/profile service semantics and policy must be recovered. Configuration/Chrome-command APIs are not sufficient evidence of this operation.'
    if (cls.startswith(('ArcAI','ArcCommandBarRanking','ArcNomicEmbedText')) or cls in ('ArcHSLAMatrix','ArcNotificationItem','ArcNotificationButton')):
        base.update(category=ARC,confidence='high_contract_low_semantics',evidence_kind='engine_ObjC_feature_contract',
            rationale='Arc-specific feature/configuration or value-object contract is explicit in the engine class/selector metadata. Preserve this app-facing model in a bridge/app implementation; backend algorithm and code provenance are unknown.')
    if sel.startswith('.cxx_') or 'arc::' in row['type_encoding'] or sel.startswith('initWith') or sel.startswith('mutable'):
        base.update(category=ARC,confidence='high_ABI_low_semantics',evidence_kind='engine_ObjC_native_wrapper_ABI',
            rationale='Arc native-wrapper ABI/lifetime entry point, supported by selector and type encoding. Requires Arc-shaped state and ownership in either route; this does not identify a proprietary backend algorithm.')
    if cls=='ArcBrowserContext' and sel in ('chatClosed:withReason:','productConnectionClosed:withReason:','getAdBlockDebugInfo:completion:'):
        base.update(category=ARC,confidence='medium_contract_low_semantics',evidence_kind='engine_ObjC_feature_contract',
                    rationale='App-specific product/ad-block diagnostic contract; exact side effects, IPC and lifetime remain unknown.')
    if (cls,sel) in RULES:base.update(RULES[(cls,sel)])
    return base

def run():
    rows=list(csv.DictReader((ROOT/'inventory.tsv').open(),delimiter='\t'))
    inspected={}
    arrived=ROOT/'evidence/arrived-requested-functions.tsv'
    if arrived.exists():
        inspected={r['engine170_imp_va']:r for r in csv.DictReader(arrived.open(),delimiter='\t')}
    abi_path=ROOT.parent.parent/'abi/initial/contract.tsv'
    abi={}
    if abi_path.exists():
        for r in csv.DictReader(abi_path.open(),delimiter='\t'):
            abi[(r['class_name'],r['category'],r['kind'],r['selector'])]=r
        assert len(abi)==len(rows)
    for row in rows:
        key=row['class_name'],row['category'],row['kind'],row['selector']
        if abi:
            other=abi[key]
            assert int(row['engine170_imp_va'],16)==int(other['imp'],16),key
            assert row['type_encoding']==other['type_encoding'],key
            row['abi_contract_compatibility']=other['compatibility']
        else:row['abi_contract_compatibility']='not_read'
        row['objc_category']=row.pop('category')
        row.update(classify(row))
        row['public_evidence_role']='positive_mapping_API' if row['category'] in (DIRECT,PARTIAL) else 'review_scope_only_not_support_for_Arc_semantics'
        row['mapping_scope']='contract operation coverage; Arc behavior and ObjC/C++ ABI parity unverified'
        evidence=inspected.get(row['engine170_imp_va'],{})
        row['engine_pseudocode_status']=evidence.get('status','not_extracted_for_this_row')
        row['engine_pseudocode_source']=evidence.get('source','')
        row['engine_pseudocode_line']=evidence.get('source_line','')
        row['engine_pseudocode_sha256']=evidence.get('source_sha256','')
        row['engine_forwarding_targets']=evidence.get('direct_named_FUN_targets','')
        row['engine_evidence']='/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64@'+row['engine170_imp_va']
        row['engine_evidence_revision']='ArcCore-1.170.0 sha256:69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd'
    write_tsv(ROOT/'cef-mapping.tsv',rows)
    categories=[DIRECT,PARTIAL,INTERNAL,ARC,UNKNOWN]
    counts=collections.Counter(r['category'] for r in rows)
    out=[dict(category=c,count=counts[c]) for c in categories]
    write_tsv(ROOT/'category-counts.tsv',out)
    byclass=[]
    for cls in sorted({r['class_name'] for r in rows}):
        count=collections.Counter(r['category'] for r in rows if r['class_name']==cls)
        byclass.append(dict(class_name=cls,total=sum(count.values()),**{c:count[c] for c in categories}))
    write_tsv(ROOT/'category-counts-by-class.tsv',byclass)
    summary=dict(cef_revision=CEF,cef_chromium='156.0.8078.48',target_chromium='156.0.8078.48',
                 counts={c:counts[c] for c in categories},rows=len(rows),classes=len(byclass),
                 independently_matches_abi_contract=bool(abi),positive_rules=len(RULES),
                 caveat='Counts describe declared contract operation coverage, not called methods, code size, work estimates, or proven behavioral equivalence.')
    (ROOT/'evidence/mapping-validation.json').write_text(json.dumps(summary,indent=2)+'\n')
    print(json.dumps(summary,indent=2))

if __name__=='__main__':run()
