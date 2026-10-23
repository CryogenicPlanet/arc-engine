#!/usr/bin/env bash
# Event continuation on research VM; never accesses exe.dev management or SSH.
set -uo pipefail
root=/home/exedev/work
exec 9>"$root/auto-synthesis.lock"
flock -n 9 || exit 0
echo $$ > "$root/auto-synthesis.pid"
jobs=(abi-decompile cef-decompile arc-layer-decompile matcher-decompile quality-initial protocols-initial spec-1-classreview spec-2-classreview spec-3-classreview spec-4-classreview)
deadline=$((SECONDS + 14400))
while :; do
  ready=1
  for job in "${jobs[@]}"; do
    test -e "$root/$job.done" || ready=0
    if grep -qE "JOB $job FAILED" "$root/STATUS"; then
      echo "Predecessor failed: $job; preserving state for lead review"
      exit 1
    fi
  done
  if [ "$ready" -eq 1 ]; then break; fi
  if [ "$SECONDS" -ge "$deadline" ]; then
    printf '%s JOB auto-synthesis FAILED (prerequisites not complete after 4h)\n' "$(date -u +%FT%TZ)" >> "$root/STATUS"
    exit 1
  fi
  sleep 30
done
bash "$root/run-agent.sh" review synthesis
