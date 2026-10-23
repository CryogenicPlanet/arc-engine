// ==== 0x4410ba4  objc::Categories::ArcWebContents_Extensions_::close  (16 bytes)

/* Function Stack Size: 0x10 bytes */

int objc::Categories::ArcWebContents_Extensions_::close(int param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend((ulong)(uint)param_1,"closeWithForegroundCommitTiming:",1);
  return (int)IVar1;
}


