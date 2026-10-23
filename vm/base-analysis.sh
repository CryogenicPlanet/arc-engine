#!/usr/bin/env bash
# arcmod-base VM: Ghidra analysis of our stock Chromium 156 build (Chromium Framework, arm64, unstripped)
# for function matching against ArcCore. Waits for the upload, verifies its sha256, analyzes with heavy
# analyzers off, saves the project, then exports function bounds. STATUS ends in "JOB base DONE|FAILED".
set -uo pipefail
W=~/work
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$W/STATUS"; }
EXPECT_SHA=39647a537f42460da18576c0a5856171dfec2250ed505698dbbdd99a40b75645
BIN="$W/upload/ChromiumFramework-156.0.8078.48.arm64"
P="$W/ghidra-projects" NAME=ChromiumBase-156
mkdir -p "$W"/{upload,tools,logs,ghidra-projects,decomp}
if ! command -v javac >/dev/null; then
  sudo DEBIAN_FRONTEND=noninteractive apt-get update -q >"$W/logs/apt.log" 2>&1
  sudo DEBIAN_FRONTEND=noninteractive apt-get install -y -q openjdk-21-jdk-headless llvm jq >>"$W/logs/apt.log" 2>&1 && step "apt done" || { step "JOB base FAILED: apt"; exit 1; }
fi
if ! swapon --show | grep -q /swapfile; then
  sudo fallocate -l 32G /swapfile && sudo chmod 600 /swapfile && sudo mkswap /swapfile >/dev/null && sudo swapon /swapfile && step "swap 32G enabled"
fi
if ! ls -d "$W"/tools/ghidra_*/ >/dev/null 2>&1; then
  url=$(curl -sSf https://api.github.com/repos/NationalSecurityAgency/ghidra/releases/tags/Ghidra_12.1.4_build | jq -r '.assets[] | select(.name | endswith(".zip")) | .browser_download_url' | head -1)
  (cd "$W/tools" && curl -sSfL -o ghidra.zip "$url" && unzip -q -o ghidra.zip && rm ghidra.zip) && step "ghidra installed" || { step "JOB base FAILED: ghidra download"; exit 1; }
fi
step "waiting for upload"
until [ -f "$BIN.done" ]; do sleep 20; done
got=$(sha256sum "$BIN" | cut -c1-64)
[ "$got" = "$EXPECT_SHA" ] && step "upload verified ($(du -h "$BIN" | cut -f1))" || { step "JOB base FAILED: sha256 mismatch ($got)"; exit 1; }
G=$(ls -d "$W"/tools/ghidra_*/ | head -1)
export JAVA_HOME=$(dirname "$(dirname "$(readlink -f "$(command -v javac)")")")
export GHIDRA_HEADLESS_MAXMEM=40G
if ! grep -q "base ANALYSIS SAVED" "$W/STATUS"; then
  step "ghidra analyze ChromiumBase-156 (heap 40G, heavy analyzers off)"
  rm -rf "$P/$NAME.rep" "$P/$NAME.gpr" "$P/$NAME.lock" "$P/$NAME.lock~"
  "$G/support/analyzeHeadless" "$P" "$NAME" -import "$BIN" -max-cpu 32 -scriptPath "$W/scripts/ghidra"     -preScript DisableHeavyAnalyzers.java >"$W/logs/ghidra-base-analyze.log" 2>&1     && grep -q "Save succeeded" "$W/logs/ghidra-base-analyze.log"     && step "base ANALYSIS SAVED ($(du -sh "$P" | cut -f1))" || { step "JOB base FAILED: analysis (logs/ghidra-base-analyze.log; dmesg)"; exit 1; }
fi
"$G/support/analyzeHeadless" "$P" "$NAME" -process "$(basename "$BIN")" -noanalysis -readOnly   -scriptPath "$W/scripts/ghidra" -postScript ExportFunctions.java "$W/decomp/chromium-base-156-functions.tsv"   >"$W/logs/ghidra-base-export.log" 2>&1 && [ -s "$W/decomp/chromium-base-156-functions.tsv" ]   && step "JOB base DONE: $(wc -l <"$W/decomp/chromium-base-156-functions.tsv") functions exported"   || step "JOB base FAILED: export (logs/ghidra-base-export.log); analysis is saved, rerun to retry"
