#!/usr/bin/env bash
# Phase B: analyze Arc 1.170.0's arm64 engine in Ghidra, export function bounds, decompile everything.
set -euo pipefail
W=~/work
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$W/STATUS"; }
G=$(ls -d "$W"/tools/ghidra_*/ | head -1)
export JAVA_HOME=$(dirname "$(dirname "$(readlink -f "$(command -v javac)")")")
export MAXMEM=26G GHIDRA_HEADLESS_MAXMEM=26G
mkdir -p "$W/ghidra-projects" "$W/decomp/arccore-1.170.0"
step "ghidra: analyzing ArcCore 1.170.0 arm64 (MAXMEM=$MAXMEM)"
"$G/support/analyzeHeadless" "$W/ghidra-projects" ArcCore-1.170.0 \
  -import "$W/arc/1.170.0/ArcCore-1.170.0.arm64" -max-cpu 16 \
  -scriptPath "$W/scripts/ghidra" \
  -postScript ExportFunctions.java "$W/decomp/arccore-1.170.0-functions.tsv" \
  -postScript DecompileAll.java "$W/decomp/arccore-1.170.0" 60 \
  >"$W/logs/ghidra-arccore.log" 2>&1 && step "PHASE B DONE: $(wc -l <"$W/decomp/arccore-1.170.0-functions.tsv") functions, $(du -sh "$W/decomp/arccore-1.170.0" | cut -f1) decompile" || step "PHASE B FAILED (logs/ghidra-arccore.log)"
