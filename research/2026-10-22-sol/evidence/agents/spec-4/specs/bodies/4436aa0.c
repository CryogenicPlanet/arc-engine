// ==== 0x4436aa0  ArcBrowserContextEffects::registerColorFilter:forHostPattern:  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserContextEffects::registerColorFilter_forHostPattern_
               (ID param_1,SEL param_2,float *param_3,ID param_4)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 *local_48;
  long local_40;
  long lStack_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    local_48 = (undefined8 *)FUN_00008790(0x50);
    uVar8 = *(undefined8 *)(param_3 + 0x12);
    uVar7 = *(undefined8 *)(param_3 + 0x10);
    uVar4 = *(undefined8 *)param_3;
    uVar6 = *(undefined8 *)(param_3 + 6);
    uVar5 = *(undefined8 *)(param_3 + 4);
    local_48[1] = *(undefined8 *)(param_3 + 2);
    *local_48 = uVar4;
    local_48[3] = uVar6;
    local_48[2] = uVar5;
    uVar4 = *(undefined8 *)(param_3 + 8);
    uVar6 = *(undefined8 *)(param_3 + 0xe);
    uVar5 = *(undefined8 *)(param_3 + 0xc);
    local_48[5] = *(undefined8 *)(param_3 + 10);
    local_48[4] = uVar4;
    local_48[7] = uVar6;
    local_48[6] = uVar5;
    local_48[9] = uVar8;
    local_48[8] = uVar7;
    lStack_38 = 0x14;
    local_40 = 0x14;
    if (param_4 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_4);
    }
    FUN_043329b4(param_1 + 0x10,&local_48,&local_60);
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    if (local_48 != (undefined8 *)0x0) {
      if (local_40 * 4 != 0) {
        lVar2 = local_40 * -4;
        lVar3 = local_40 * 4 + (long)local_48;
        do {
          lVar3 = lVar3 + -4;
          if (lVar3 == 0) goto LAB_04436bb0;
          lVar2 = lVar2 + 4;
        } while (lVar2 != 0);
      }
      local_40 = 0;
      FUN_00005810(local_48,lStack_38 << 2);
    }
    return;
  }
LAB_04436bb0:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4436bb4);
  (*pcVar1)();
}


