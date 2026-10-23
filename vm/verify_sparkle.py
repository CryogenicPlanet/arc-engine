"""Verify an Arc release archive against its Sparkle EdDSA signature (Arc's SUPublicEDKey)."""
import base64
import sys

import nacl.exceptions
import nacl.signing

ARC_PUBLIC_ED_KEY = "PuVxvsbwitqWEvMPriVhNzmiMiVp2AqD8psqG/qcS98="

archive, signature = sys.argv[1], sys.argv[2]
with open(archive, "rb") as f:
    data = f.read()
try:
    nacl.signing.VerifyKey(base64.b64decode(ARC_PUBLIC_ED_KEY)).verify(data, base64.b64decode(signature))
except nacl.exceptions.BadSignatureError:
    sys.exit(f"BAD SIGNATURE: {archive}")
print(f"signature ok: {archive} ({len(data)} bytes)")
