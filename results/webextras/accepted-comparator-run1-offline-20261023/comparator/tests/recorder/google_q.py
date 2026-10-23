"""Opt-in comparison-value proposal. Pure stdlib; never writes raw transcripts."""
import re
from urllib.parse import quote, unquote_to_bytes

QUERY='autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:'
IDENTITY=('thread','depth','from','dir','cls','kind','sel','self')
GETTERS={'destinationUrlSpec','destinationUrl'}


def _google_value(value,selector,query_url,normalized_query):
    if not isinstance(value,str) or not value.isascii(): return value
    prefix='url:' if selector=='destinationUrl' else ''
    start=prefix+'https://www.google.com/search?'
    if not value.startswith(start): return value
    query,fragment_mark,fragment=value[len(start):].partition('#')
    parts=query.split('&');q_index=None
    for index,part in enumerate(parts):
        key,equal,encoded=part.partition('=')
        # Encoded keys, aliases, empty keys/parts and flag-only parameters are
        # outside this narrow proposal. Every non-q byte remains untouched.
        if not equal or not re.fullmatch(r'[A-Za-z0-9._~-]+',key): return value
        if key=='q':
            if q_index is not None: return value
            q_index=index
            try: decoded=unquote_to_bytes(encoded).decode('utf-8',errors='strict')
            except UnicodeError: return value
            if decoded!=query_url or quote(decoded,safe='')!=encoded: return value
    if q_index is None: return value
    parts[q_index]='q='+quote(normalized_query,safe='')
    return start+'&'.join(parts)+fragment_mark+fragment


def prepare_google_q_normalizer(events,*,enabled=False):
    """Return a ret-value accessor for THESE event objects; default is identity.

    Opt-in requires exactly one fully paired v2 app/main browser query whose
    input is an explicit canonical HTTP 127.0.0.1 fixture URL. Every main record
    participates in strict depth/LIFO/full-identity pairing. A live -run root
    may remain open; no other open frame is accepted. Invalid opt-in evidence
    raises ValueError. Only proven paired getter RET objects can be rewritten.
    """
    if enabled is not True:
        return lambda event:event.get('ret')
    events=tuple(events);stack=[];pairs=[];queries=[];seqs=[];main_seqs=[]
    def need(ok,message):
        if not ok: raise ValueError(message)
    for event in events:
        if event.get('ev')=='hooked':
            need(event.get('seq')==0,'invalid hook marker');continue
        need(event.get('ev') in ('call','ret') and all(k in event for k in IDENTITY) and
             all(isinstance(event[k],str) for k in ('thread','from','dir','cls','kind','sel')) and
             event['kind'] in ('+','-') and (isinstance(event['self'],str) if event['kind']=='-' else event['self'] is None) and
             type(event.get('seq')) is int and event['seq']>0 and type(event.get('depth')) is int,
             'missing v2 event metadata')
        seqs.append(event['seq'])
        if event['thread']!='main': continue
        main_seqs.append(event['seq'])
        if event['ev']=='call':
            need(event['depth']==len(stack),'main depth gap');stack.append(event)
            if all(event.get(k)==v for k,v in {'from':'app','dir':'->engine','cls':'ArcBrowser','kind':'-','sel':QUERY}.items()):
                queries.append(event)
        else:
            need(stack and all(stack[-1][k]==event[k] for k in IDENTITY),'main return mismatch')
            pairs.append((stack.pop(),event))
    need(seqs and sorted(seqs)==list(range(1,len(seqs)+1)) and main_seqs==sorted(main_seqs),'sequence gap/duplicate/main order')
    need(not stack or (len(stack)==1 and all(stack[0].get(k)==v for k,v in
         {'ev':'call','depth':0,'from':'app','dir':'->engine','cls':'ArcBrowserApplication','kind':'-','sel':'run','args':[]}.items())),
         'unexpected unclosed main invocation')
    need(len(queries)==1,'requires exactly one app/main browser query')
    query=queries[0];returns=[ret for call,ret in pairs if call is query]
    args=query.get('args')
    need(len(returns)==1 and returns[0].get('ret') is True and isinstance(args,list) and len(args)==7,
         'query not fully paired with true return/seven arguments')
    url=args[0]
    match=re.fullmatch(r'(http://127\.0\.0\.1:)([1-9][0-9]{3,4})(/[!-~]*)',url) if isinstance(url,str) else None
    need(match is not None and 1024<=int(match[2])<=65535 and type(args[1]) is int and args[1]==len(url) and
         args[2] is False and args[3] is True and args[4:]==['','<ptr>','<block>'],
         'query is not canonical fresh loopback fixture/end cursor')
    normalized=match[1]+'<port>'+match[3]
    eligible={id(ret):ret for call,ret in pairs if call['seq']>query['seq'] and call.get('args')==[] and
              all(ret.get(k)==v for k,v in {'from':'app','dir':'->engine','cls':'ArcAutocompleteMatch','kind':'-'}.items()) and
              ret.get('sel') in GETTERS}
    def normalized_return(event):
        if eligible.get(id(event)) is not event: return event.get('ret')
        return _google_value(event.get('ret'),event['sel'],url,normalized)
    return normalized_return
