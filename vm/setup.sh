#!/usr/bin/env bash
# Phase A on the analysis VM: tools, verified Arc releases, Chromium 156 file list, Ghidra.
set -euo pipefail
W=~/work
mkdir -p "$W"/{arc,chromium,tools,logs}
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$W/STATUS"; }
export PATH="/usr/lib/llvm-18/bin:$PATH"

step "apt: java 21, llvm, pynacl"
sudo DEBIAN_FRONTEND=noninteractive apt-get update -q >"$W/logs/apt.log" 2>&1
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y -q openjdk-21-jdk-headless llvm python3-nacl jq ripgrep zip >>"$W/logs/apt.log" 2>&1
step "apt done"

arc() {
  local version=$1 build=$2 sig=$3 dir="$W/arc/$1"
  mkdir -p "$dir" && cd "$dir"
  curl -sSf -o "Arc-$version-$build.zip" "https://releases.arc.net/release/Arc-$version-$build.zip"
  python3 "$W/scripts/verify_sparkle.py" "Arc-$version-$build.zip" "$sig" | tee -a "$W/STATUS"
  unzip -q -o "Arc-$version-$build.zip" -x '__MACOSX/*'
  llvm-lipo Arc.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore -thin arm64 -output "ArcCore-$version.arm64"
  llvm-lipo Arc.app/Contents/MacOS/Arc -thin arm64 -output "Arc-$version.arm64"
  step "arc $version: $(sha256sum "ArcCore-$version.arm64" | cut -c1-64) ArcCore arm64"
}
chromium_files() {
  cd "$W/chromium" && rm -rf src && git init -q src && cd src
  git remote add origin https://chromium.googlesource.com/chromium/src
  git fetch -q --depth 1 --filter=blob:none origin refs/tags/156.0.8078.48
  git ls-tree -r --name-only FETCH_HEAD >"$W/chromium/upstream-156.0.8078.48-files.txt"
  git show FETCH_HEAD:DEPS >"$W/chromium/DEPS-156.0.8078.48"
  step "chromium 156.0.8078.48 file list: $(wc -l <"$W/chromium/upstream-156.0.8078.48-files.txt") files"
}
ghidra() {
  cd "$W/tools"
  local url
  url=$(curl -sSf https://api.github.com/repos/NationalSecurityAgency/ghidra/releases/tags/Ghidra_12.1.4_build | jq -r '.assets[] | select(.name | endswith(".zip")) | .browser_download_url' | head -1)
  curl -sSfL -o ghidra.zip "$url" && unzip -q -o ghidra.zip && rm ghidra.zip
  step "ghidra: $(ls -d ghidra_*)"
}

( arc 1.170.0 89123 "1z4CJHtKVaq04u0L+egYMt2XT05CU+WTcDU7ZPYkwRksQFAViT77S+izFc07r2hjkDmXB4B4hbIjkhpza71YBA==" ) >"$W/logs/arc-1.170.0.log" 2>&1 &
( arc 1.168.0 88413 "8SvW3bZ1jExyCKf8hWk+XGUq6JaRG1dMNzZyLsAmkVsNwOA/Gext95RRTWoFcYRlEjXT39pl0AOpbAmk1aVMAw==" ) >"$W/logs/arc-1.168.0.log" 2>&1 &
( chromium_files ) >"$W/logs/chromium-files.log" 2>&1 &
( ghidra ) >"$W/logs/ghidra-download.log" 2>&1 &
wait && step "PHASE A DONE" || step "PHASE A FAILED (see logs)"
