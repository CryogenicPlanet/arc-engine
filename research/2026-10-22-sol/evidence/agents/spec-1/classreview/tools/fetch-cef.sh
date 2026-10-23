#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/spec-1
mkdir -p classreview/sources
curl --fail --silent --show-error --max-time 30 https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/include/cef_browser.h -o classreview/sources/cef_browser.h
curl --fail --silent --show-error --max-time 30 https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/include/internal/cef_types_mac.h -o classreview/sources/cef_types_mac.h
