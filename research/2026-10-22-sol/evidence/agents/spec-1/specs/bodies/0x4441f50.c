// ==== 0x4441f50  ArcWebContents::shareThisTab  (168 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::shareThisTab(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined4 local_40 [2];
  undefined8 local_38;
  undefined8 uStack_30;
  undefined4 local_28;
  undefined8 local_24;
  undefined4 local_1c;
  undefined1 local_18;
  undefined4 local_14;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4441ff0);
    (*pcVar1)();
  }
  lVar2 = *(long *)(*(long *)(param_1 + 0x118) + 0x600);
  if (((lVar2 != 0) && (lVar3 = *(long *)(*(long *)(param_1 + 0x118) + 0x130), lVar3 != 0)) &&
     (*(long *)(lVar2 + 0x148) != 0)) {
    lVar3 = *(long *)(lVar3 + 0x8d8);
    if ((*(long *)(lVar3 + 0x128) != 0) && (*(char *)(*(long *)(lVar3 + 0x128) + 4) == '\0')) {
      lVar4 = *(long *)(*(long *)(lVar3 + 0x130) + 0x40);
      if (lVar4 != 0) {
        local_24 = CONCAT44(*(undefined4 *)(lVar3 + 0x758),*(undefined4 *)(lVar4 + 0x198));
        local_40[0] = 3;
        local_38 = 0;
        uStack_30 = 0;
        local_28 = 0;
        local_1c = 0;
        local_18 = 0;
        local_14 = 0;
        FUN_00dffa40(lVar2 + 0x148,local_40,*(undefined1 *)(lVar2 + 0x161));
        return;
      }
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x4442000);
      (*pcVar1)();
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x4441ff4);
    (*pcVar1)();
  }
  return;
}


