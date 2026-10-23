// Recovered stock component manifest, resources.pak 14558. Public key identifies
// the built-in extension; this contains no private key or account material.
#ifndef ARC_BROWSER_EXTENSIONS_INTERNAL_MANIFEST_H_
#define ARC_BROWSER_EXTENSIONS_INTERNAL_MANIFEST_H_
#include <string_view>
namespace arc {
inline constexpr std::string_view kInternalExtensionId =
    "ldciclpbkaobfbicjebamaapbfplfjen";
inline constexpr std::string_view kInternalExtensionManifest = R"arcmanifest({
  "key": "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArT/1kaBtWCXe9WdsSBen3/M1/NTTh/zAsLUMjQoylbwgM7J37H1epAByfR+4mRLQbgw2hAdaA+rQId+MqSgZPTAUym84xQR6rpiB+nfJtMu7K14RhGveAaWlZaoZJvbNitXAJOcSeetcl5yfMrEk/S/zFH4NhYs+GYcZMWo9ZCdU64OzxkAJyHXLig6KYDPxK/HY54+HxBrs2r7xZPW1VA0JxotXAdDXqKQVm7OKxtDTnAGqb71mz+PeY+ptSTUH0q0hF/Xioo8py1tG+/Yppc8H22rAZOfqO7YOzI16cnga6h5aNz4JMtAYLbgBSX23UQi1lGCzAcYnbA4uK3+2uQIDAQAB",
  "name": "Arc Internal Extension",
  "version": "0.0.10",
  "manifest_version": 3,
  "background": {
    "service_worker": "background.js"
  },
  "incognito": "split",
  "permissions": [
    "scripting",
    "tabs"
  ],
  "host_permissions": [
    "https://app.slack.com/*",
    "https://chromewebstore.google.com/*",
    "https://www.notion.so/*"
  ]
}
)arcmanifest";
}  // namespace arc
#endif
