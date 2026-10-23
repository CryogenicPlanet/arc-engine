// ==== 0x4443b58  ArcWebContents::deviceScaleFactor  (116 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcWebContents::deviceScaleFactor(ID param_1,SEL param_2)

{
  code *pcVar1;
  float *pfVar2;
  long lVar3;
  float fVar4;
  undefined1 auStack_48 [40];
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar3 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    fVar4 = 1.0;
    if (((lVar3 != 0) && (fVar4 = 1.0, *(long *)(lVar3 + 0x8d8) != 0)) &&
       (lVar3 = FUN_010f9b60(), lVar3 != 0)) {
      FUN_01f5fc10(auStack_48,lVar3 + 0x110);
      pfVar2 = (float *)FUN_02fd2190(auStack_48);
      fVar4 = *pfVar2;
      FUN_01e930f8(auStack_48);
    }
    return fVar4;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4443bcc);
  (*pcVar1)();
}


