// ==== 0x43ed544  ArcBookmarkImportTargets::initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode:  (148 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBookmarkImportTargets::initWithBookmarkBarBookmarksTargetNode_withOtherBookmarksTargetNode_
             (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  ID IVar1;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBookmarkImportTargets;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 != 0) {
    *(ID *)(IVar1 + 8) = param_3;
    *(ID *)(IVar1 + 0x10) = param_4;
  }
  return IVar1;
}


