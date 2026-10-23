// ==== 0x43dd5d4  ArcHtmlToMarkdownResult::initWithBody:title:references:selectorsApplied:selectorsContentPct:metaTags:  (364 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x40 bytes */

ID ArcHtmlToMarkdownResult::
   initWithBody_title_references_selectorsApplied_selectorsContentPct_metaTags_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,double param_7,
             ID param_8)

{
  ID IVar1;
  ID IVar2;
  undefined8 uVar3;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcHtmlToMarkdownResult;
  local_70 = param_1;
  IVar1 = NSObject::init((ID)&local_70,"init");
  if (IVar1 != 0) {
    IVar2 = _objc_msgSend(param_3,"copy");
    uVar3 = *(undefined8 *)(IVar1 + 8);
    *(ID *)(IVar1 + 8) = IVar2;
    IVar2 = _objc_msgSend(param_4,"copy",uVar3);
    uVar3 = *(undefined8 *)(IVar1 + 0x10);
    *(ID *)(IVar1 + 0x10) = IVar2;
    IVar2 = _objc_msgSend(param_5,"copy",uVar3);
    uVar3 = *(undefined8 *)(IVar1 + 0x18);
    *(ID *)(IVar1 + 0x18) = IVar2;
    IVar2 = _objc_msgSend(param_6,"copy",uVar3);
    uVar3 = *(undefined8 *)(IVar1 + 0x20);
    *(ID *)(IVar1 + 0x20) = IVar2;
    *(double *)(IVar1 + 0x28) = param_7;
    IVar2 = _objc_msgSend(param_8,"copy",uVar3);
    *(ID *)(IVar1 + 0x30) = IVar2;
  }
  return IVar1;
}


