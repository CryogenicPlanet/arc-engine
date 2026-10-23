#!/usr/bin/env bash
# Phase B (restart): robust Ghidra run on ArcCore 1.170.0 arm64.
#  1. ensure swap, 2. import + analyze + SAVE (checkpoint), 3. export bounds + decompile from the saved project.
# Retries analysis once with heavy analyzers off; decompile can be rerun on its own. STATUS ends in PHASE B DONE/FAILED.
set -uo pipefail
W=~/work
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$W/STATUS"; }
G=$(ls -d "$W"/tools/ghidra_*/ | head -1)
export JAVA_HOME=$(dirname "$(dirname "$(readlink -f "$(command -v javac)")")")
export GHIDRA_HEADLESS_MAXMEM=20G
P="$W/ghidra-projects" NAME=ArcCore-1.170.0 BIN="$W/arc/1.170.0/ArcCore-1.170.0.arm64"
OUT="$W/decomp/arccore-1.170.0" FUNCS="$W/decomp/arccore-1.170.0-functions.tsv"

if ! swapon --show | grep -q /swapfile; then
  sudo fallocate -l 32G /swapfile && sudo chmod 600 /swapfile && sudo mkswap /swapfile >/dev/null && sudo swapon /swapfile
  step "swap: $(swapon --show --noheadings | awk '{print $3}') enabled"
fi

analyzed() { grep -q "ANALYSIS SAVED" "$W/STATUS"; }
analyze() { # $1 = attempt label, remaining args = extra headless args
  local label=$1; shift
  rm -rf "$P/$NAME.rep" "$P/$NAME.gpr" "$P/$NAME.lock" "$P/$NAME.lock~"
  step "ghidra analyze ($label, heap $GHIDRA_HEADLESS_MAXMEM)"
  "$G/support/analyzeHeadless" "$P" "$NAME" -import "$BIN" -max-cpu 16 -scriptPath "$W/scripts/ghidra" "$@" \
    >"$W/logs/ghidra-analyze-$label.log" 2>&1 && grep -q "REPORT: Save succeeded\|Save succeeded" "$W/logs/ghidra-analyze-$label.log"
}
if ! analyzed; then
  if analyze full || analyze lite -preScript DisableHeavyAnalyzers.java; then
    step "ANALYSIS SAVED ($(du -sh "$P" | cut -f1) project)"
  else
    step "PHASE B FAILED: analysis (logs/ghidra-analyze-*.log; dmesg for OOM)"; exit 1
  fi
fi

mkdir -p "$OUT"
step "ghidra export + decompile from saved project (-max-cpu 10)"
"$G/support/analyzeHeadless" "$P" "$NAME" -process "$(basename "$BIN")" -noanalysis -readOnly -max-cpu 10 \
  -scriptPath "$W/scripts/ghidra" -postScript ExportFunctions.java "$FUNCS" -postScript DecompileAll.java "$OUT" 60 \
  >"$W/logs/ghidra-decompile.log" 2>&1 \
  && [ -s "$FUNCS" ] && [ "$(ls "$OUT" | wc -l)" -gt 0 ] \
  && step "PHASE B DONE: $(wc -l <"$FUNCS") functions, $(ls "$OUT" | wc -l) shards, $(du -sh "$OUT" | cut -f1)" \
  || step "PHASE B FAILED: decompile (logs/ghidra-decompile.log); analysis is saved, rerun this script to retry"
