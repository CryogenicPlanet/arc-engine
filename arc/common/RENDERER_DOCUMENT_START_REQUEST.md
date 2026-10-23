# Minimal document-start hook for accepted Boost lifecycle

**UNAPPLIED; lead-owned native change.** Apply the Chromium-root artifact
`renderer_document_start_hook_proposal.patch`, SHA256
`7732bc58ec57ef2194b4270fca22895a42e12c47e1c59ab583413ef9d7bac767`,
through the maintained Chromium patch workflow. No numbered slot is reserved.

It inserts four lines in
`chrome/renderer/chrome_content_renderer_client.cc:1495` before extension
document-start calls: in host-app mode, call the already-linked
`arc::RunEffectsAtDocumentStart(render_frame)` and return if its weak frame agent
did not survive injected script. Existing configuration delivery still only
stores new configuration; existing documents are unchanged.

Exact current native file SHA256:
`9db0d5a861abf63603033393d099b96dc1496ee3a6a8a17b6a9fe45b1bb78a15`.
Exact proposed file SHA256:
`abbcf4a16eb60c2eb8f7f7c1485a72f1f44706fcfcc6e76b8c32294d4cc36a7c`.
The required header is already included, and the helper exists in current owned
source. This requires no new shared declaration, Mojo generation, GN dependency,
other file or 49-file integration-kit prerequisite.

The accepted native `renderer-boost-lifecycle` case passes58 assertions and all
four public CSS/isolated-JS register/unregister operations. First OURS raw50fc
fails the next-document state: black/default and no injected DOM marker instead
of purple/marker1. Unregister and its next-document state are not reached.
Repeated document-identity polling makes the total assertion count variable.
Native registration deliberately retains existing document state; reapplying on
configuration receipt would violate that observed behavior.

The current native file has no call into the helper at document start. This is a
verified integration gap consistent with the observed failure; the patch has not
yet been linked or run, and no sole-cause or fix claim is made. One native-TU
actual-flag VFS check passes with empty diagnostics; source/header sentinels and
exact patch replay pass. Evidence:
`logs/wp/renderer-boost-document-start-hook/`; independent clean review:
`logs/wp/renderer-boost-document-start-review/REVIEW.md`, SHA256
`d9e414bb14bc8323b491909bf6d44766834e15b630a1ca09fce4fa979a1e9268`.

This hunk is already present in the larger nativef67 proposal. If adopted alone,
that full native artifact must be rebased to remove the duplicate before later
adoption; never apply both blindly. The narrow proposal does not depend on the
core WebContents baseline, which has separately advanced to progress475.
After lead adoption, run the unchanged Boost lifecycle case and full boundary
diff with fresh source/package guards. Runtime and complete acceptance remain
pending. COMMON ownership keeps actual Chromium and `patches/` writes with lead.

The future Boost-only runner is prepared at
`logs/wp/renderer-boost-document-start-validation/`, unsealed/unrun until the exact
native target above exists. Its private copied profile isolator uses a no-follow
Darwin directory-FD ownership attribute, preserving the unchanged fixture's empty
storage listing and all original restore/process/lock checks. Twelve private-temp
canaries pass; independent adaptation review339736ca is clean. Shared isolator,
fixture assertions and recorded native observations are unchanged. The original
marker-file incompatibility and failed Python API probe are preserved. This
preparation has touched no real disposable profile or launched an application.

A reviewed future remainder is now published as
`renderer_native_after_document_start_proposal.patch`, SHA256
`2893b8f079ff40ad931518b17d1f4954acbfb37a8609b8209c206dde4c37a709`.
After exact7732 adoption, this replaces nativef67 and pairs with unchanged Arca044
(or the separately qualified conditional Arc companion). It removes only the
already-adopted four-line hunk, preserves every other native byte, and reaches the
same full target. Twelve private Git operations/reversals and independent review
e985f579 pass. Current future-baseline guard correctly fails solely because the
hook is not yet adopted. Never combine the remainder with full nativef67.
