#!/usr/bin/env python3
"""Create exact-IMP requests from metadata; never infer bodies from VA spans."""
import csv,json,pathlib
from inventory import ROOT,INPUT,write_tsv

# class, category, selector, priority, gate, specific question
REQUESTS=[
('ArcBrowserApplication','','initialize:','P0','Cocoa bootstrap','Does this own NSApplication/delegates/helper entry points or coexist with an app-owned event loop? Follow actual initialization and error callbacks.'),
('ArcBrowserApplication','','init:','P0','startup ownership','Which app/configuration objects, global browser process state, observers and C++ bridge objects are retained?'),
('ArcBrowserApplication','','run','P0','message pump','Does run enter Chromium main loop, NSApplication run, or an externally pumped event loop?'),
('ArcBrowserApplication','','tryToTerminate:','P0','shutdown','Which beforeunload, browser-close and async completion sequence gates shutdown?'),
('ArcBrowserApplication','','createBrowser:withDelegate:forWindow:','P0','native embedding','How does NSWindow map to Browser, native NSView/Views and ArcBrowser? Can CEF macOS parent-view Alloy hosting preserve this?'),
('ArcBrowserApplication','','createPopupBrowser:withDelegate:','P0','popup creation','Recover opener/context identity, popup dispositions, delegate lifetime and returned window ownership.'),
('ArcBrowserApplication','','createBrowserContext:isHidden:type:completion:error:','P0','profiles','Recover profile path/type/incognito/hidden rules, ProfileManager calls, encryption and sync/async failure semantics.'),
('ArcBrowserApplication','','loadBrowserContext:type:completion:','P0','profile persistence','How are existing Arc profiles loaded/migrated; is context ready before completion and which thread invokes it?'),
('ArcBrowserApplication','','deleteBrowserContext:error:','P1','profile deletion','Recover unload-versus-disk-delete behavior, outstanding web contents, callbacks and policy checks.'),
('ArcBrowserContext','Extensions','createWebContents:','P0','contents factory','Recover synchronous return, Browser association, initial render/hidden state and delegate/NSView lifetime.'),
('ArcBrowser','', 'takeOwnershipOfWebContents:','P0','reparenting','Does moving WebContents between windows preserve Browser/Profile/session ownership? Recover destruction and delegate replacement.'),
('ArcBrowser','','setWindow:','P0','AppKit window','Determine NSWindow/Views integration, window replacement and event handling prerequisites.'),
('ArcWebContents','Extensions','createPaneHostView','P0','custom native UI','Identify exact NSView class, hierarchy, accessibility/responder behavior and required ownership.'),
('ArcWebContents','Extensions','attachToPaneHostView:','P0','custom native UI','Recover reparenting, backing layer/resize/compositor/safe-area behavior and sequencing.'),
('ArcWebContents','Extensions','detachFromPaneHostView:','P0','custom native UI','Determine whether detach suspends rendering, destroys view ownership, or leaves contents alive.'),
('ArcWebContents','','setUIDelegate:','P0','UI callback bridge','Enumerate UI callbacks including password/autofill/passkeys, permission prompts, dialogs, popup creation and cancellation ownership.'),
('ICloudKeychainPresentationDelegate','','presentationAnchorForAuthorizationController:','P0','passkey presentation','Which NSWindow is provided as the AuthenticationServices anchor? Determine relationship to Arc native pane/window embedding; platform code provenance remains unknown.'),
('ICloudKeychainCreateController','','setRequest:','P1','passkey platform bridge','Recover request/ownership handoff and identify callers reaching this system passkey path. This is not proof of Arc-specific modification.'),
('ICloudKeychainGetController','','_requestContextWithRequests:error:','P1','passkey platform bridge','Recover macOS credential assertion presentation/cancellation/error behavior and callers. Also request upstream C++ route for phone QR/security-key flows once identified.'),
('ArcWebContents','','setNavigationDelegate:','P1','navigation callback bridge','Recover provisional/commit/redirect/failure ordering, navigation-token lifetime, and required Chromium observers.'),
('ArcBrowserContext','Extensions','id','P0','category override identity','Compare base id at 0x4084dc0 against category override: is ID a stable profile identity or bridge wrapper?'),
('ArcBrowserContext','','id','P0','category override identity','Compare category id at 0x43f541c; class/category metadata must remain distinct.'),
('ArcBrowserContext','Extensions','createAutocompleteControllerWithProviders:','P0','omnibox service','Recover selected providers, shared history/bookmark/search services, cancellation and result callback shape.'),
('ArcBrowserContextAutocomplete','','autocomplete:cursorPosition:preventInlineAutocomplete:completion:','P0','omnibox behavior','Recover inline/provider/ranking semantics and callback reuse, retention, thread and cancellation.'),
('ArcBrowserContextHistory','Extensions','searchHistory:completion:','P0','global history','Recover history backend query and duplicate/matching policy, result/visit identity and completion ordering.'),
('ArcBrowserContextHistory','Extensions','deleteByOriginatorVisitIDs:originatorCacheGuid:startTime:endTime:completion:','P1','history sync identity','Are Chrome sync visit IDs/cache GUIDs semantically required by the app or merely optional import data?'),
('ArcBrowserContext','Extensions','getBookmarksTree','P1','bookmarks','Identify BookmarkModel access, IDs/UUIDs, lazy liveChildren validity and observer lifetimes.'),
('ArcBrowserContext','Extensions','importPasswords:','P0','password store','Does this call Chrome password store/importer? Which profile keychain/encryption formats and save callbacks are required?'),
('ArcLoginCredentialsSaveRequest','','save','P0','password/autofill callbacks','Recover underlying save request object, captured once-callback and post-save invalidation. Distinguish HTTP auth from form passwords.'),
('ArcLoginCredentialsSaveRequest','','neverAskAgain','P1','password blocklist','Recover profile-level blocklist behavior and whether Chrome storage/schema is directly manipulated.'),
('ArcCreditCardInfoSaveRequest','','save','P1','autofill','Recover Chrome Autofill ownership, local/cloud card save distinction, validation and completion lifetime.'),
('ArcBrowserContextExtensions','','installExtensionFromWebstore:unattended:completion:','P0','extensions install','Recover Chrome install service, Web Store policy, validation, async ownership and profile association.'),
('ArcBrowserContextExtensions','','runJSInExtensionBackgroundPage:js_code:','P0','extension execution','Determine MV3 service-worker/background-page targeting, renderer IPC and callback semantics.'),
('ArcBrowserContextExtensions','','getAllExtensionCommands','P1','extension command UI','Recover command enumeration/shortcuts/global flags and service dependencies.'),
('ArcBrowserContextExtensions','','setPinnedExtensionOrder:','P0','custom extension toolbar','Recover ToolbarActionsModel/pref dependencies and observer notification requirements.'),
('ArcBrowserApplication','','restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:','P0','session restoration','Recover Chrome SessionRestore dependencies, callback sequence and restored WebContents ownership.'),
('ArcBrowserContext','Extensions','getLocalStorage:key:completion:','P1','storage without renderer','Is storage read through DOMStorage backend without a live renderer? Recover origin/partition rules and thread crossing.'),
('ArcBrowserContext','Extensions','getAllCookies:completion:','P1','cookie edge semantics','Recover filter argument, host-only/partitioned cookie information, callback ordering and visitor snapshots.'),
('ArcBrowserContext','Extensions','setPermissionsForOrigin:withPermissionTypes:withSiteSetting:','P1','permission scope','Recover Arc enum mapping, requesting versus top-level origin, policy and persistence.'),
('ArcBrowserContext','Extensions','productConnectionClosed:withReason:','P1','Arc IPC','Recover endpoint/channel/close reason and Mojo versus app-only state. Generic CEF IPC is not evidence of Arc protocol parity.'),
('ArcWebContents','','executeJavascript:reason:completion:','P1','script IPC','Recover isolated/main world, execution reason policy, user activation, result serialization and completion/cancellation ownership.'),
('ArcWebContents','Extensions','mediaSession','P1','media/DRM','Identify MediaSession control surface, notifications, audio focus and its connection to DRM/permissions; follow actual C++ media objects.'),
('ArcWebContents','Extensions','devTools','P1','devtools embedding','Recover frontend Browser/WebContents association, custom inspector view and lifecycle; compare public CEF devtools APIs.'),
('ArcBrowserContextDownloads','Extensions','startDownload:error:','P1','background downloads','Does download require a browser or only a profile? Recover persistence, target path and error behavior.'),
('ArcDownloadItem','','remainingTimeInSeconds','P1','download snapshots','Recover duration calculation, unknown-time sentinel, snapshot lifetime and notification cadence.'),
('ArcDownloadItem','','remove','P1','download history removal','Does remove delete profile history, local file, cancel a running transfer, or notify app observers?'),
('ArcBrowserContextPageContentExtraction','Extensions','getExtractedContentForWebContents:completion:','P1','custom renderer extraction','Recover renderer agents/IPC, accessibility/DOM extraction schema, frame coverage, cancellation and cache semantics.'),
('ArcBrowserApplication','','createAIService:','P2','AI service','Determine external library/service boundary versus renderer engine changes and whether startup depends on successful initialization.'),
('ArcPerformanceController','','setHighEfficiencyModeEnabled:','P2','performance policy','Recover Chrome performance-manager policy changes versus app-side preferences; quantify release-sensitive internal dependencies.'),
]

def run():
    allrows=list(csv.DictReader((ROOT/'evidence/engine-objc-all.tsv').open(),delimiter='\t'))
    lookup={(r['class_name'],r['category'],r['kind'],r['selector']):r for r in allrows}
    imported={r['class_name'] for r in csv.DictReader((ROOT/'inventory.tsv').open(),delimiter='\t')}
    selected=[]
    for cls,cat,sel,priority,gate,question in REQUESTS:
        kind='+' if cls=='ArcPerformanceController' else '-'
        r=dict(lookup[cls,cat,kind,sel]);va=int(r['engine170_imp_va'],16)
        r.update(priority=priority,route_gate=gate,question=question,
            inventory_scope='imported_class' if cls in imported else ('returned/transitive_class_not_in_834' if cls.startswith('Arc') else 'platform_engine_class_not_in_834'),
            engine_image='ArcCore-1.170.0.arm64',address_basis='unslid_MachO_engine_VA',
            logical_1MiB_bucket_start=hex((va>>20)<<20),logical_1MiB_bucket_end_exclusive=hex(((va>>20)+1)<<20),
            shard_request='Export shard containing this exact IMP; bucket is an address grouping, not an asserted exporter filename.',
            followup='Include direct callees/actual C++ bridge implementation, referenced callbacks/delegate protocols, constants and call-site xrefs; do not stop at a forwarding ObjC method.')
        selected.append(r)
    addresses={int(r['engine170_imp_va'],16) for r in selected};gh={}
    for line in (INPUT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
        a,b,size,name=line.rstrip('\n').split('\t',3)
        if int(a,16) in addresses:gh[int(a,16)]=(b,size,name)
    for r in selected:
        a,b,c=gh[int(r['engine170_imp_va'],16)]
        r.update(ghidra_end_bound_va=a,ghidra_body_bytes=b,ghidra_qualified_name=c)
    write_tsv(ROOT/'decompile-requests.tsv',selected)
    grouped=[]
    for bucket in sorted({r['logical_1MiB_bucket_start'] for r in selected},key=lambda a:int(a,16)):
        items=[r for r in selected if r['logical_1MiB_bucket_start']==bucket]
        grouped.append(dict(bucket_start=bucket,bucket_end_exclusive=items[0]['logical_1MiB_bucket_end_exclusive'],
            exact_imp_starts=','.join(sorted({r['engine170_imp_va'] for r in items},key=lambda a:int(a,16))),
            requested_methods=len(items),exporter_filename='unknown; resolve using actual shard index'))
    write_tsv(ROOT/'decompile-shard-groups.tsv',grouped)
    print(f'{len(selected)} exact method requests, {len(grouped)} logical 1-MiB buckets; not a body byte map')

if __name__=='__main__':run()
