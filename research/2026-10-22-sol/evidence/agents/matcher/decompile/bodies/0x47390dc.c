// ==== 0x47390dc  FUN_047390dc  (56 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

ID FUN_047390dc(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = _objc_alloc(&objc::class_t::WebContentsPaneHostView);
  IVar1 = WebContentsPaneHostView::initWithFrame_
                    (IVar1,"initWithFrame:",*(CGRect *)PTR__NSZeroRect_0d8254f0);
  return IVar1;
}


