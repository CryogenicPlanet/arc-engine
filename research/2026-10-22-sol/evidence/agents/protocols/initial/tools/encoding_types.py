"""Readable declarations from metadata, with no inferred enums/nullability."""
import re
SCALAR={'v':'void','B':'BOOL','c':'signed char','C':'unsigned char','s':'short','S':'unsigned short','i':'int','I':'unsigned int','l':'long','L':'unsigned long','q':'int64_t','Q':'uint64_t','f':'float','d':'double',':':'SEL','#':'Class','*':'char *','?':'unknown'}
def one(s,i=0):
    c=s[i];start=i;i+=1
    if c in 'rnNoORV':
        t,j=one(s,i);return ('const ' if c=='r' else '')+t,j
    if c=='@':
        if i<len(s) and s[i]=='"':
            j=s.index('"',i+1);name=s[i+1:j];return ('id'+name if name.startswith('<') else name+' *'),j+1
        if i<len(s) and s[i]=='?':
            i+=1
            if i<len(s) and s[i]=='<':
                i+=1;sig=[]
                while s[i]!='>':
                    t,i=one(s,i);sig.append(t)
                assert sig[1]=='block'
                return sig[0]+' (^)('+', '.join(sig[2:])+')',i+1
            return 'block',i
        return 'id',i
    if c=='^':
        t,j=one(s,i);return t+' *',j
    if c in '{([':
        close={'{':'}','(':')','[':']'}[c];depth=1;j=i;quote=False
        while depth:
            x=s[j];j+=1
            if x=='"':quote=not quote
            if not quote:
                if x==c:depth+=1
                if x==close:depth-=1
        return s[start:j].split('=')[0].lstrip('{(').rstrip('})'),j
    if c=='b':
        m=re.match(r'\d+',s[i:]);return 'bitfield('+m[0]+')',i+len(m[0])
    return SCALAR[c],i
def sequence(s):
    ts=[];i=0
    while i<len(s):
        t,i=one(s,i);ts.append(t)
        while i<len(s) and s[i] in '0123456789+-':i+=1
    return ts
def declaration(kind,selector,encoding):
    ts=sequence(encoding);assert ts[1:3]==['id','SEL'],(selector,ts)
    args=ts[3:];assert selector.count(':')==len(args),(selector,ts)
    if not args:return f'{kind} ({ts[0]}){selector};'
    return kind+' ('+ts[0]+')'+' '.join(f'{part}:({typ})arg{i}' for i,(part,typ) in enumerate(zip(selector.split(':')[:-1],args)))+';'
