# ArcCore 1.170.0 vs Chromium 156.0.8078.48: compiled-in source paths

- Paths found in the engine: 7823 (only files that log or assert are visible)
- Browser Company `arc/` tree: 111
- Present in stock Chromium src (unmodified or modified, unknown): 6074
- From Chromium DEPS repos (v8, skia, ...; checked separately): 1638
- Generated sources: 0
- **Inside Chromium's own directories but absent from stock Chromium (added by the Browser Company): 0**

## Added inside Chromium directories


## `arc/` tree by directory

-   46 `arc/browser/ui`
-    8 `arc/browser/web_contents`
-    6 `arc/browser/importer`
-    5 `arc/browser/browser_context`
-    4 `arc/utility/importer`
-    3 `arc/browser/extensions`
-    3 `arc/browser/public`
-    2 `arc/common/utils`
-    2 `arc/components/crash`
-    2 `arc/third_party/platform_media`
-    1 `arc/base/posted_from_logger.cc`
-    1 `arc/blink/core`
-    1 `arc/browser/ai_service_impl.cc`
-    1 `arc/browser/arc_browser_runner.cc`
-    1 `arc/browser/arc_content_browser_client.cc`
-    1 `arc/browser/browser_application_impl.cc`
-    1 `arc/browser/browser_impl.cc`
-    1 `arc/browser/content_conversion_service_impl.cc`
-    1 `arc/browser/download_item_impl.cc`
-    1 `arc/browser/history_manager.cc`
-    1 `arc/browser/internal`
-    1 `arc/browser/local_state_impl.cc`
-    1 `arc/browser/metrics`
-    1 `arc/browser/network_activity_monitor_impl.cc`
-    1 `arc/browser/network_process_connection_waiter.cc`
-    1 `arc/browser/objcpp`
-    1 `arc/browser/performance`
-    1 `arc/browser/tracing_controller_impl.cc`
-    1 `arc/browser/translate_helper.cc`
-    1 `arc/browser/url_loader_helper.cc`
-    1 `arc/common/arc_paths_mac.mm`
-    1 `arc/components/ad_blocker`
-    1 `arc/components/feature_config`
-    1 `arc/content/browser`
-    1 `arc/content/public`
-    1 `arc/renderer/ad_blocker_agent.cc`
-    1 `arc/renderer/render_frame_agent_impl.cc`
-    1 `arc/services/ai`
-    1 `arc/services/content_conversion`
-    1 `arc/services/data_decoder`

## DEPS repos referenced

-  432 `third_party/webrtc`
-  236 `third_party/dawn`
-  172 `third_party/boringssl/src`
-  170 `third_party/skia`
-  117 `net/third_party/quiche/src`
-   96 `third_party/perfetto`
-   87 `third_party/angle`
-   57 `third_party/ffmpeg`
-   56 `third_party/ink/src`
-   54 `v8`
-   41 `third_party/litert/src`
-   34 `third_party/federated_compute/src`
-   31 `third_party/openscreen/src`
-   15 `third_party/re2/src`
-   12 `third_party/pdfium`
-    7 `third_party/tflite/src`
-    5 `third_party/anonymous_tokens/src`
-    5 `third_party/libc++abi/src`
-    4 `third_party/ruy/src`
-    3 `third_party/rust-toolchain`
-    2 `third_party/libc++/src`
-    1 `third_party/cld_3/src`
-    1 `third_party/highway/src`
