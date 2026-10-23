// ==== 0x1004d9468  FUN_1004d9468  (92 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_1004d9468(void)

{
  ID self;
  NSArray *pNVar1;
  ID unaff_x20;
  
  self = objc_stub::extensions(unaff_x20);
  pNVar1 = (extension_Foundation)::Swift::Array<undefined>::_bridgeToObjectiveC();
  objc_stub::setPinnedExtensionOrder_(self,pNVar1);
  return 0;
}


