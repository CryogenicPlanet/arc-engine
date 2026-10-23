#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/spec-4
mkdir -p specs/sources/net/base/registry_controlled_domains specs/sources/boringssl
curl -fsSL --max-time 30 https://raw.githubusercontent.com/chromium/chromium/d0c02bfad08ec92a6ffcf989e53cd995e6381a4c/net/base/registry_controlled_domains/registry_controlled_domain.h -o specs/sources/net/base/registry_controlled_domains/registry_controlled_domain.h
curl -fsSL --max-time 30 https://raw.githubusercontent.com/google/boringssl/2ea3200c3a3ec176613ab7a0b9a4ffc6db846120/include/openssl/curve25519.h -o specs/sources/boringssl/curve25519.h
