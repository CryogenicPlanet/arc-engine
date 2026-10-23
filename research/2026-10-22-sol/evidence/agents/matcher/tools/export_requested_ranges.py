# Ghidra Jython research helper for the PARENT to transfer/run if appropriate.
# Not executed here; no Ghidra installation or remote connection is required.
# @category ArcMod.Research
# Arguments: decompile-requests.tsv output.jsonl
# Emits real AddressSet ranges. Does not decompile or change the program.
import csv
import json

args = getScriptArgs()
if len(args) != 2:
    raise ValueError('Expected request TSV and output JSONL paths')
manager = currentProgram.getFunctionManager()
seen = set()
with open(args[0], 'rb') as inp, open(args[1], 'wb') as out:
    for row in csv.DictReader(inp, delimiter='\t'):
        value = int(row['arc_start'], 16)
        if value in seen:
            continue
        seen.add(value)
        entry = toAddr(value)
        function = manager.getFunctionAt(entry)
        record = dict(requested_entry=hex(value),program_name=str(currentProgram.getName()),
                      executable_sha256=str(currentProgram.getExecutableSHA256()))
        if function is None:
            record['error'] = 'no function at exact requested entry'
        else:
            body = function.getBody()
            ranges = []
            iterator = body.getAddressRanges()
            while iterator.hasNext():
                address_range = iterator.next()
                ranges.append(dict(space=str(address_range.getMinAddress().getAddressSpace().getName()),
                    start=str(address_range.getMinAddress()),end_inclusive=str(address_range.getMaxAddress()),
                    size=int(address_range.getLength())))
            record.update(name=str(function.getName(True)),is_thunk=bool(function.isThunk()),
                          min_address=str(body.getMinAddress()),max_address_inclusive=str(body.getMaxAddress()),
                          actual_body_byte_count=int(body.getNumAddresses()),body_ranges=ranges)
        out.write(json.dumps(record,sort_keys=True)+'\n')
