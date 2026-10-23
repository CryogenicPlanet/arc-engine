# Independent bounded review

PASS for the conditional nil-ObjC-owner bridge policy.

The actual B70 bridge weak-load-retains its selected delegate at `4446b2c`, holds that reference in `x21` through the owner load at `4446b38` and unconditional `objc_msgSend` at `4446b54`, then releases that selected reference at `4446b70`. The owner is explicit argument `x2`; it is not the message receiver. There is no owner-presence branch in the inspected complete bridge body. Installation offsets, selector, vtable slot and actual chained import names corroborate that dataflow. The bounded known invoker checks operate on native manager/registration identities without reading the bridge's weak ObjC owner.

Current source lines126–133 precisely retain the selected adapter, load the owner and delegate, add `contents &&` to delivery eligibility, and decrement pending afterward. Removing only that owner condition is consistent with the original conditional bridge policy while preserving the reviewed lifetime/order. This review does not inspect or approve the separately prepared private candidate.

All packet input/output hashes and all11 original range hashes were independently verified against the actual pinned binary. Reviewed README SHA256: `40577fad701bd23b9be1b2948591741b821a6361a8dea3e593093c36f9e08dc4`. Reviewed manifest SHA256: `f327ccb127dea967598c03d167e5533598bc1adca80e550ab891a05b4d27933d`. Actual original universal binary SHA256: `b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`. Full reviewed pins are in `fresh-review.json`.

Live-registration/surviving-delegate/nil-owner reachability in an intact canonical lifetime remains unproved. This is not a Root ownership, generic selector/registration, absent-registration retention, all-call-graph, runtime or activation conclusion. The earlier post-PM native-binding guard is distinct. No app/probe/compiler/query/build/Git/index/queue/lock/Chirp/credential or production source action was performed. Only these two new review files were written; existing packet files were not changed.
