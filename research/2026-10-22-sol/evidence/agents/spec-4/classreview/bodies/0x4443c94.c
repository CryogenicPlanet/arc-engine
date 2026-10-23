// ==== 0x4443c94  ArcWebContents::zoomFactor  (88 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcWebContents::zoomFactor(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  undefined8 uVar3;
  double dVar4;
  float fVar5;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    fVar5 = 1.0;
    if ((*(long *)(*(long *)(param_1 + 0x118) + 0x130) != 0) &&
       (lVar2 = FUN_00882ba0(), fVar5 = 1.0, lVar2 != 0)) {
      uVar3 = FUN_04597a58();
      dVar4 = (double)_pow(0x3ff3333333333333,uVar3);
      fVar5 = (float)dVar4;
    }
    return fVar5;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443cec);
  (*pcVar1)();
}


