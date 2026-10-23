"""PROPOSAL ONLY: launch-scoped, proven download reference aliases.

Does not normalize any native GUID value, SessionID integer, URL, path, date,
callback sequence, or scalar. Missing/conflicting proof for a compared download
or its non-nil source WebContents is an error, never a guessed correspondence.
"""
import collections
import re

REF = re.compile(r'^<(ArcDownloadItem|ArcWebContents)#\d+>$')
SID_REF = re.compile(r'^<ArcSessionID#\d+>$')
GUID = re.compile(r'^[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}$')
GETTERS = frozenset({('ArcDownloadItem', 'id'), ('ArcDownloadItem', 'webContents'),
                     ('ArcWebContents', 'sessionId'), ('ArcSessionID', 'identifier')})


class IdentityEvidenceError(ValueError):
    pass


DOWNLOAD_BRIDGES = frozenset({'ADK.BrowserContextDownloadDelegateBridge',
                              '_TtC3ADK36BrowserContextDownloadDelegateBridge'})
DOWNLOAD_CALLBACKS = frozenset({
    'browserContext:onDownloadCreated:', 'browserContext:onDownloadUpdated:',
    'browserContext:onDownloadRemoved:',
    'browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:'})
SNAPSHOT = 'browserContext:downloadManagerInitializedWithItems:'
BROWSER_BRIDGES = frozenset({'ADK.ChromiumBrowser', '_TtC3ADK15ChromiumBrowser'})
TYPED_RETURNS = {
    ('ArcDownloadItem', 'webContents'): 'ArcWebContents',
    ('ArcDownloadItem', 'initWithDownloadItem:'): 'ArcDownloadItem',
    ('ArcWebContents', 'initWithWebContents:'): 'ArcWebContents',
    ('ArcBrowserContext', 'createWebContents:'): 'ArcWebContents',
}


def typed_slots(event, field, value):
    """Yield (path, expected class) ONLY for known object-typed positions.

    Strings and object references share a recorder representation. Never infer
    a typed position from a string's spelling. In particular the filename,
    URLs, dictionary values, and arbitrary callback arguments are not slots.
    """
    if event.get('kind') != '-':
        return
    cls, sel = event.get('cls'), event.get('sel')
    if field == 'self' and event.get('dir') == '->engine' and cls in ('ArcDownloadItem', 'ArcWebContents'):
        yield (), cls
    elif field == 'ret' and event.get('ev') == 'ret' and event.get('dir') == '->engine':
        expected = TYPED_RETURNS.get((cls, sel))
        if expected:
            yield (), expected
    elif field == 'args' and event.get('ev') == 'call' and event.get('dir') == 'engine->app' and type(value) is list and len(value) > 1:
        if cls in DOWNLOAD_BRIDGES:
            if sel in DOWNLOAD_CALLBACKS:
                yield (1,), 'ArcDownloadItem'
            elif sel == SNAPSHOT and type(value[1]) is list:
                for index in range(len(value[1])):
                    yield (1, index), 'ArcDownloadItem'
        elif cls in BROWSER_BRIDGES and sel == 'browser:didCreateNewWebContents:isActive:':
            yield (1,), 'ArcWebContents'


def typed_references(event, field):
    value = event.get(field)
    for path, expected in typed_slots(event, field, value):
        ref = value
        for index in path:
            ref = ref[index]
        if type(ref) is str and REF.fullmatch(ref) and ref.startswith('<' + expected + '#'):
            yield ref


class DownloadIdentityAliases:
    def __init__(self, raw_events, compared_events):
        self.values = collections.defaultdict(list)
        self.errors = collections.defaultdict(list)
        self.aliases = {}
        pending = collections.defaultdict(list)

        def key(e):
            return e.get('cls'), e.get('sel'), e.get('self')

        def signature(e):
            return tuple(e.get(k) for k in ('cls', 'sel', 'self', 'dir', 'kind', 'thread', 'depth', 'from'))

        # Collect proof BEFORE caller filtering: the app can observe a wrapper
        # while the native implementation makes its identity getter call.
        for e in raw_events:
            if (e.get('cls'), e.get('sel')) not in GETTERS or e.get('ev') not in ('call', 'ret'):
                continue
            if e.get('dir') != '->engine' or e.get('kind') != '-':
                continue
            stack = pending[e.get('thread')]
            if e['ev'] == 'call':
                stack.append(e)
                if e.get('args') != []:
                    self.errors[key(e)].append('identity getter arguments are not empty')
            elif not stack:
                self.errors[key(e)].append('orphan identity return')
            else:
                call = stack.pop()
                if signature(call) != signature(e):
                    self.errors[key(call)].append('identity call/return receiver or signature mismatch')
                    self.errors[key(e)].append('identity call/return receiver or signature mismatch')
                elif 'ret' not in e:
                    self.errors[key(e)].append('identity return has no explicit ret field')
                else:
                    self.values[key(e)].append(e['ret'])
        for stack in pending.values():
            for call in stack:
                self.errors[key(call)].append('identity call has no matching return')

        # Preserve the old normalizer's first appearance in args/returns.
        # Receivers are proof-checked, but remain outside its comparisons and
        # must not consume first-appearance object numbers.
        compared_refs = list(dict.fromkeys(ref for e in compared_events
            for field in ('args', 'ret') for ref in typed_references(e, field)))
        receiver_refs = list(dict.fromkeys(ref for e in compared_events
            for ref in typed_references(e, 'self')))
        compared_refs = list(dict.fromkeys(compared_refs + receiver_refs))
        downloads = [ref for ref in compared_refs if ref.startswith('<ArcDownloadItem#')]
        by_guid = {}
        source_refs = []
        for ref in downloads:
            value = self.single('ArcDownloadItem', 'id', ref)
            if type(value) is not str or not GUID.fullmatch(value):
                raise IdentityEvidenceError(f'{ref}: invalid canonical download GUID proof')
            self.aliases[ref] = by_guid.setdefault(value, ref)
            k = ('ArcDownloadItem', 'webContents', ref)
            if self.errors[k]:
                raise IdentityEvidenceError(f'{ref}: invalid source getter proof: {self.errors[k]}')
            for source in self.values[k]:
                if source is None:
                    continue  # A download with no source WebContents is valid.
                if type(source) is not str or not source.startswith('<ArcWebContents#') or not REF.fullmatch(source):
                    raise IdentityEvidenceError(f'{ref}: non-object source WebContents proof')
                source_refs.append(source)

        source_refs = list(dict.fromkeys(source_refs))
        source_ids = {self.session_id(ref) for ref in source_refs}
        by_sid = {}
        # Only source identities discovered through actual DownloadItem
        # webContents returns can seed an alias group. This can also identify
        # an already-existing tab wrapper with that same proven SessionID.
        for ref in list(dict.fromkeys(compared_refs + source_refs)):
            if not ref.startswith('<ArcWebContents#'):
                continue
            try:
                sid = self.session_id(ref)
            except IdentityEvidenceError:
                if ref in source_refs:
                    raise
                continue  # Unrelated/unproved WebContents stays strictly separate.
            if sid in source_ids:
                self.aliases[ref] = by_sid.setdefault(sid, ref)

    def single(self, cls, selector, ref):
        k = cls, selector, ref
        values = self.values[k]
        if self.errors[k] or not values:
            raise IdentityEvidenceError(f'{ref} {selector}: missing/invalid proof {self.errors[k]}')
        first = values[0]
        if any(type(v) is not type(first) or v != first for v in values):
            raise IdentityEvidenceError(f'{ref} {selector}: conflicting identity proof')
        return first

    def session_id(self, ref):
        k = 'ArcWebContents', 'sessionId', ref
        if self.errors[k] or not self.values[k]:
            raise IdentityEvidenceError(f'{ref}: missing/invalid sessionId proof')
        identifiers = []
        for sid_ref in self.values[k]:
            if type(sid_ref) is not str or not SID_REF.fullmatch(sid_ref):
                raise IdentityEvidenceError(f'{ref}: invalid/missing SessionID object')
            value = self.single('ArcSessionID', 'identifier', sid_ref)
            if type(value) is not int or not 0 < value <= 2147483647:
                raise IdentityEvidenceError(f'{ref}: invalid native SessionID integer')
            identifiers.append(value)
        if len(set(identifiers)) != 1:
            raise IdentityEvidenceError(f'{ref}: conflicting native SessionID values')
        return identifiers[0]

    def rewrite_field(self, event, field, value):
        # Apply one shared correspondence only in explicit typed slots. Never
        # recurse through arbitrary payloads, including reference-shaped text.
        result = value
        for path, expected in typed_slots(event, field, value):
            ref = value
            for index in path:
                ref = ref[index]
            if type(ref) is not str or not ref.startswith('<' + expected + '#'):
                continue
            replacement = self.aliases.get(ref, ref)
            if replacement == ref:
                continue
            if not path:
                result = replacement
            elif len(path) == 1:
                if result is value:
                    result = list(value)
                result[path[0]] = replacement
            else:
                if result is value:
                    result = list(value)
                    result[path[0]] = list(value[path[0]])
                result[path[0]][path[1]] = replacement
        return result
