// Copyright 2026 ArcMod.
#ifndef ARC_RENDERER_AD_BLOCK_FILTER_H_
#define ARC_RENDERER_AD_BLOCK_FILTER_H_

#include <memory>

#include "base/functional/callback.h"
#include "base/memory/scoped_refptr.h"

namespace blink {
class WebDocumentSubresourceFilter;
}
namespace subresource_filter {
class MemoryMappedRuleset;
}
namespace url {
class Origin;
}

namespace arc {
std::unique_ptr<blink::WebDocumentSubresourceFilter> CreateAdBlockFilter(
    scoped_refptr<const subresource_filter::MemoryMappedRuleset> ruleset,
    url::Origin document_origin,
    bool excluded,
    base::RepeatingClosure disallowed_load_callback);
}  // namespace arc

#endif  // ARC_RENDERER_AD_BLOCK_FILTER_H_
