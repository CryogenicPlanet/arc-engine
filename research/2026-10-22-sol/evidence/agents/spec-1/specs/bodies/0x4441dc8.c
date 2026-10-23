// ==== 0x4441dc8  ArcWebContents::resetZoomAndScaleFactors  (104 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::resetZoomAndScaleFactors(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  undefined8 local_28;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4441e30);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x118);
  if (*(long *)(lVar3 + 0x130) == 0) {
    return;
  }
  lVar2 = FUN_00882ba0();
  if (lVar2 != 0) {
    local_28 = 0;
    FUN_09fc1b54(0,lVar2,&local_28);
  }
  FUN_067982c0(0x3f800000,*(undefined8 *)(lVar3 + 0x130));
  return;
}


