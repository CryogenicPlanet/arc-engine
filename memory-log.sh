#!/usr/bin/env bash
# Samples system memory every 30 s while the base build driver runs: used GB (active+wired+compressed), swap used, build phase.
E="$(cd "$(dirname "$0")" && pwd)"
page=$(sysctl -n hw.pagesize)
while pgrep -f "build-base.sh|build-babysit.sh" >/dev/null; do
  vm=$(vm_stat)
  pages() { echo "$vm" | awk -v k="$1" '$0 ~ k {gsub("\\.","",$NF); print $NF}'; }
  used=$(( ($(pages "Pages active") + $(pages "Pages wired down") + $(pages "Pages occupied by compressor")) * page / 1073741824 ))
  swap=$(sysctl -n vm.swapusage | awk '{print $6}')
  phase=$(tail -1 "$E/STATUS" 2>/dev/null | cut -c22-60)
  echo "$(date -u +%FT%TZ) used=${used}GB swap=${swap} phase=${phase}" >>"$E/logs/memory.log"
  sleep 30
done
