// ==== 0x1c391bc  FUN_01c391bc  (96 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */

cfstringStruct * FUN_01c391bc(undefined8 param_1,long param_2)

{
  code *pcVar1;
  cfstringStruct *pcVar2;
  
  if (param_2 == 0) {
    pcVar2 = &cf___;
    _CFRetain(&cf___);
  }
  else {
    if (param_2 < 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x1c39204);
      (*pcVar1)();
    }
    pcVar2 = (cfstringStruct *)
             _CFStringCreateWithBytes
                       (*(undefined8 *)PTR__kCFAllocatorDefault_0d824840,param_1,param_2,0x8000100,0
                       );
  }
  return pcVar2;
}


