// ==== 0x85eac30  FUN_085eac30  (172 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

ID FUN_085eac30(long *param_1)

{
  ID self;
  long lVar1;
  
  if (*param_1 == 0) {
    self = 0;
  }
  else {
    self = _objc_alloc_init(&_OBJC_CLASS___NSImage);
    lVar1 = FUN_02a0308c(param_1);
    if (lVar1 == 0) {
      self = 0;
    }
    else {
      NSImage::addRepresentation_(self,"addRepresentation:",lVar1);
      NSImage::setSize_(self,"setSize:",(double)(int)param_1[5],
                        (double)*(int *)((long)param_1 + 0x2c));
    }
  }
  return self;
}


