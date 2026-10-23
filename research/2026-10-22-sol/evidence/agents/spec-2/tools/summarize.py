import json,re,sys
ms=json.load(open('specs/evidence/methods-bodies.json'))
for m in ms:
 if len(sys.argv)>1 and m['class_name'] not in sys.argv[1:]:continue
 b=m['body'];code=b[b.find('\n{'):]
 calls=list(dict.fromkeys(re.findall(r'FUN_[0-9a-f]+|_objc_msgSend\([^;]+',code)))
 fields=list(dict.fromkeys(re.findall(r'param_1 \+ (0x[0-9a-f]+|[0-9]+)',code)))
 print(m['class_name'],m['category'] or 'base',m['selector'],m['imp'],'C='+str(len(b)),'fields='+','.join(fields),'calls='+','.join(calls))
 print('  '+' | '.join(x.strip() for x in code.splitlines() if 'return ' in x or '"../../' in x or '_objc_' in x or '"arc' in x)[:1200])
