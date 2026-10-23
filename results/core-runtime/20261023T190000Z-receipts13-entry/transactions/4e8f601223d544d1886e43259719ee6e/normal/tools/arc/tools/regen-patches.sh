#!/bin/bash
# Regenerate patches/*.patch from the Chromium tree using patches/MANIFEST (file -> owning patch).
# Fails if a changed Chromium file isn't assigned in the manifest. Run under arc/tools/locked.sh.
#
# Safety (after the 17:45 UTC incident, where a fixture's private GIT_INDEX_FILE made every patch a
# whole-file "new file" diff): inherited git environment is ignored, the real chromium/src index is used,
# a patch that marks a file tracked in HEAD as new is refused, and patches are replaced atomically.
set -euo pipefail
unset GIT_INDEX_FILE GIT_DIR GIT_WORK_TREE GIT_OBJECT_DIRECTORY GIT_COMMON_DIR GIT_NAMESPACE GIT_PREFIX
E="$(cd "$(dirname "$0")/../.." && pwd)"
SRC="$E/chromium/src"
[ "$(git -C "$SRC" rev-parse --show-toplevel)" = "$SRC" ] || { echo "not the real chromium/src checkout"; exit 1; }
cd "$SRC"
changed=$(git status --porcelain | grep -v '^?? arc$' | awk '{print $2}' | grep -v '^arc$' || true)
assigned=$(grep -v '^#' "$E/patches/MANIFEST" | awk '{print $2}')
missing=$(comm -23 <(echo "$changed" | sort -u) <(echo "$assigned" | sort -u) | grep -v '^$' || true)
if [ -n "$missing" ]; then
  echo "unassigned changed files (add them to patches/MANIFEST):"; echo "$missing"; exit 1
fi
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
for patch in $(grep -v '^#' "$E/patches/MANIFEST" | awk '{print $1}' | sort -u); do
  files=$(grep -v '^#' "$E/patches/MANIFEST" | awk -v p="$patch" '$1==p {print $2}')
  for f in $files; do git ls-files --error-unmatch "$f" >/dev/null 2>&1 || git add -N "$f"; done
  git diff --no-ext-diff -- $files > "$tmp/$patch"
  # A file that exists in HEAD must never appear as a new file.
  for f in $(grep -B1 '^+++ b/' "$tmp/$patch" | grep -A1 '^--- /dev/null' | grep '^+++ b/' | sed 's#+++ b/##'); do
    if git cat-file -e "HEAD:$f" 2>/dev/null; then
      echo "REFUSED: $patch would mark tracked file $f as new (wrong index?); no patch written"; exit 1
    fi
  done
done
for p in "$tmp"/*.patch; do
  mv "$p" "$E/patches/$(basename "$p")"
  echo "$(basename "$p"): $(grep -c '^+++ ' "$E/patches/$(basename "$p")") files"
done
