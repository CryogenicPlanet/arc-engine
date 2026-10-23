// ==== 0x4738a30  WebContentsPaneHostView::detachWebContentsHostView  (164 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

void WebContentsPaneHostView::detachWebContentsHostView(ID param_1,SEL param_2)

{
  ID IVar1;
  ID self;
  long lVar2;
  
  lVar2 = (long)attached_web_contents_host_view_;
  self = *(ID *)(param_1 + lVar2);
  if (self != 0) {
    if (*(long *)(param_1 + (long)bridge_) != 0) {
      FUN_04738ad4();
    }
    IVar1 = _objc_msgSend(self,"superview");
    if (IVar1 == param_1) {
      _objc_msgSend(self,"removeFromSuperview");
    }
    *(undefined8 *)(param_1 + lVar2) = 0;
    return;
  }
  return;
}


