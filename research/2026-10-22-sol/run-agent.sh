#!/usr/bin/env bash
set -uo pipefail
name=$1
stage=${2:-initial}
case "$name" in abi|cef|arc-layer|matcher|review) ;; *) exit 64 ;; esac
case "$stage" in initial|decompile|synthesis) ;; *) exit 64 ;; esac
root=/home/exedev/work
job="$name-$stage"
dir="$root/agents/$name"
mkdir -p "$dir" "$root/logs" "$root/out"
# A second monitor must not launch the same logical job twice.
exec 9>"$root/$name.lock"
flock -n 9 || exit 0
test ! -e "$root/$job.done" || exit 0
if [ "$stage" = decompile ]; then
  test -e "$root/$name-initial.done" && test -e "$root/inputs/decompile/READY" || exit 75
elif [ "$stage" = synthesis ]; then
  for role in abi cef arc-layer matcher; do
    test -e "$root/$role-decompile.done" || exit 75
  done
fi
echo $$ > "$root/$job.pid"
stamp() { printf '%s JOB %s %s\n' "$(date -u +%FT%TZ)" "$job" "$*" >> "$root/STATUS"; }
stamp START
attempt="$root/$job.attempt"
touch "$attempt"
nice -n 10 timeout --signal=TERM --kill-after=30s 90m codex exec --ignore-user-config --skip-git-repo-check \
  --dangerously-bypass-approvals-and-sandbox --disable multi_agent \
  -C "$dir" -m gpt-6.1-sol -c model_reasoning_effort=high \
  -c model_provider=exe-llm \
  -c 'model_providers.exe-llm.name="exe-llm"' \
  -c 'model_providers.exe-llm.base_url="https://llm.int.exe.xyz/v1"' \
  -c 'model_providers.exe-llm.wire_api="responses"' \
  -c model_providers.exe-llm.requires_openai_auth=false \
  --json -o "$root/out/$job.md" - < "$root/prompts/$job.md" \
  > "$root/logs/$job.jsonl" 2>&1
rc=$?
if [ "$rc" -eq 0 ] && [ -s "$root/out/$job.md" ] && [ -s "$dir/$stage/report.md" ] \
  && [ "$dir/$stage/report.md" -nt "$attempt" ]; then
  touch "$root/$job.done"
  stamp DONE
else
  stamp "FAILED exit=$rc (inspect logs/$job.jsonl; do not retry blindly)"
  exit 1
fi
