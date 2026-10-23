# Engine-swap acceptance run 2026-10-23T05:02:21.992Z

| check | swap155 | swap156 | same? |
| --- | --- | --- | --- |
| chromium | 155.0.8059.40 | 156.0.8078.48 | **differs** |
| wasm | true | true | yes |
| worker | true | true | yes |
| localStorage | true | true | yes |
| indexedDB | true | true | yes |
| cookie | true | true | yes |
| fetch | true | true | yes |
| serviceWorker | true | true | yes |
| crypto | true | true | yes |
| webgl2 | ANGLE (Apple, ANGLE Metal Renderer: Apple M4 Max, Unspecified Version) | ANGLE (Apple, ANGLE Metal Renderer: Apple M4 Max, Unspecified Version) | yes |
| webgpu | adapter ok | adapter ok | yes |
| codecs | avc1.42E01E:y mp4a.40.2:y vp9:y av01.0.05M.08:y hvc1.1.6.L93.B0:y | avc1.42E01E:y mp4a.40.2:y vp9:y av01.0.05M.08:y hvc1.1.6.L93.B0:y | yes |
| mediaRecorderVP9 | 64x64 playing | 64x64 playing | yes |
| widevine | com.widevine.alpha | com.widevine.alpha | yes |
| webrtcDataChannel | ERR timeout | ERR timeout | yes |
| notificationPermission | default | default | yes |
| clipboardApi | true | true | yes |
| pdfViewer | application/pdf / embed=none | application/pdf / embed=none | yes |
| download | saved to ~/Downloads | saved to ~/Downloads | yes |

## Follow-ups (same run)

- **pdfViewer:** both engines render the PDF in Chromium's viewer (see `swap155-pdf.png` and `swap156-pdf.png`).
  The `embed=none` probe is wrong: newer Chromium's PDF viewer doesn't expose `<embed>` in the top
  document. **Pass on both.**
- **webrtcDataChannel:** times out identically on both. Likely environmental: Chromium uses `.local`
  mDNS host candidates, and these fresh ad hoc-signed agent apps lack macOS Local Network permission.
  Not a swap regression. Confirm in a signed-in, permission-granted profile.
- **Extensions** (`ext-test.ts`, sessions launched with `--enable-unsafe-extension-debugging`): an MV3
  unpacked extension loaded through CDP `Extensions.loadUnpacked`, and its content script ran on both.
  **Pass on both.**
- **Stability:** no error lines in either run log and no crash reports.

Not yet covered, because these need native UI or a person: print dialog, permission prompts
(camera, mic, location, notifications), JavaScript dialogs, Little Arc, the shortcut matrix, and a
copy of a real profile (imported extensions, sign-ins) on the new engine.
