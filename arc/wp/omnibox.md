# Work package "omnibox": autocomplete and search
Read `COMMON.md` first. Classes: ArcBrowserContextAutocomplete, ArcAutocompleteMatch,
ArcAutocompleteMatchAdditionalData, ArcAutocompleteResult, ArcPasteAndGoClassification. On ArcBrowser (category):
autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:,
deleteAutocompleteMatch:. On ArcBrowserContext (category): createAutocompleteControllerWithProviders:.
Search engines (ArcSearchEngine, lead's arc_values.mm) come from TemplateURLService via the preferences package.
Real engine path: Chrome's AutocompleteController with a provider set (report "Context autocomplete" and
"Browser autocomplete queried by app"): blank input, keyword mode, error and controller recreation behavior.
Files: `arc/api/arc_omnibox_*.mm`, `arc/wp/omnibox.gni`.
Acceptance: diff clean in the "history and search" scenario (command bar typing).
