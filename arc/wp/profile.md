# Work package "profile": per-profile services

Read `COMMON.md` first. Profiles exist already (`arc/api/arc_browser_context.mm`, owned by the lead): use
`ArcProfileFrom(context)`, `-[ArcBrowserContext profile]`, `-serviceNamed:` (returns the one cached instance of a
service class; once you write the primary @implementation of e.g. ArcBrowserContextPreferences it is used
automatically). If a service needs a back reference to its ArcBrowserContext or Profile, add a designated
initializer and ask the lead (Requests) to create it with that initializer in -serviceNamed:, or add a category
method on ArcBrowserContext in your file that returns your configured instance.

Classes: ArcBrowserContextPreferences, ArcBrowserContextHistory, ArcHistoryItem, ArcHistoryDeletion,
ArcHistoryQueryOptions, ArcAnnotatedVisit, ArcVisitCategory, ArcMostVisitedURL, ArcBrowserContextDownloads,
ArcDownloadItem, ArcCookie, ArcBookmarkTreeNode, ArcSiteSettingInfo, ArcSiteEngagementDetails,
ArcDisplayNameOrOrigin, ArcLoginCredentials, ArcLoginCredentialsSaveRequest, ArcAddressEntrySaveRequest,
ArcCreditCardInfoSaveRequest, ArcBrowserContextAttributes is done (lead). On ArcBrowserContext (category in your
files): cookies, local storage, bookmarks, favicons, content settings/permissions, import/importPasswords,
metrics, getAdBlockDebugInfo, chatClosed/productConnectionClosed, setUserVisibleName:, loadUrl/cancelUrlLoad
(coordinate: the app package owns ArcURLLoad*; call into it). On ArcBrowser (category): cookie methods
(getAllCookies:, setCookie:withCookie:, removeCookie:withName:, saveCookiesToDisk). Delegates you drive:
ArcBrowserContextPreferencesObserver, ArcBrowserContextHistoryDelegate, ArcBrowserContextDownloadsDelegate
(initial snapshot, deltas, filename selection with timeout), ArcBrowserContextDelegate (profile lifecycle parts).

Files you own: `arc/api/arc_profile_*.mm` (create as needed), `arc/wp/profile.gni`.

Acceptance: `arc/tools/diff-run.sh` (launch-slice) shows no missing/return/callback differences for your classes,
then the profile scenarios (downloads, history, cookies, site settings) once the "scenarios" package records them.
