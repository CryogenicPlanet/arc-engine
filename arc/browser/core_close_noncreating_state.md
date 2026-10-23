# Noncreating logical close-state lookup — UNAPPLIED

Patch core_close_noncreating_state.patch SHA256
49db537f8d9cf570134f245fa28a68766c156889d139441fe15d9b12800226d1.
Apply after the current four-part bundle + primitive + adapterbec65249 + zoom37637.
Two private core files only; cross-package use still needs lead-approved contract.

FindCoreCloseStateForNativeWebContents(native) checks ONLY existing userdata and
an existing precise-lifetime strong/weak canonical wrapper. A private objc_direct
method compares the native association and copies its SAME initialized state.
Null native, absent userdata, unpublished/expired wrapper, and stale/reset native
association return empty. No get-or-create, public webContents getter, KVO,
callback, fresh state, or ownership transfer occurs. The required nonnil wrapper
CoreCloseStateForWebContents accessor is unchanged.

This supplies a noncreating API for UI140509, not permission pending activation.
Normal fully initialized/published managed and weak-owned cases have state.
Initialization creates state before setters, but publishes the userdata wrapper
only after init returns; TabHelpers/PermissionManager attach before wrapper,
although that constructor alone does not show a prompt. Global permission hook
has no host/core association gate. Thus EVERY prompt input's availability is
not proved. Consumers must treat missing mandatory state as an integration
blocker; silently skipping accounting or creating another wrapper/state is not
justified. Capture successful state at enqueue, not a new lookup at delivery.

Strict incremental apply/exact reconstruction PASS; actual composed WC TU
compiler exit0/empty diagnostics; fresh independent review no concrete P1/P2.
Evidence with exact staged sources, compiler command, source availability audit
and review: logs/wp/core.noncreating-state/. No consumer/link/build/runtime or
public synchronous Close activation claimed. Production sources remain frozen
for parent run8; this patch is staging only.
