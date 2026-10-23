#!/bin/bash
# Regenerate patches/*.patch from the Chromium tree using patches/MANIFEST (file -> owning patch).
# Fails if a changed Chromium file isn't assigned in the manifest. Run under arc/tools/locked.sh.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$E/chromium/src"
cd "$SRC"
changed=$(git status --porcelain | grep -v '^?? arc$' | awk '{print $2}' | grep -v '^arc$' || true)
assigned=$(grep -v '^#' "$E/patches/MANIFEST" | awk '{print $2}')
missing=$(comm -23 <(echo "$changed" | sort -u) <(echo "$assigned" | sort -u) | grep -v '^$' || true)
if [ -n "$missing" ]; then
  echo "unassigned changed files (add them to patches/MANIFEST):"; echo "$missing"; exit 1
fi
for patch in $(grep -v '^#' "$E/patches/MANIFEST" | awk '{print $1}' | sort -u); do
  files=$(grep -v '^#' "$E/patches/MANIFEST" | awk -v p="$patch" '$1==p {print $2}')
  for f in $files; do git ls-files --error-unmatch "$f" >/dev/null 2>&1 || git add -N "$f"; done
  git diff --no-ext-diff -- $files > "$E/patches/$patch"
  echo "$patch: $(grep -c '^+++ ' "$E/patches/$patch") files"
done
