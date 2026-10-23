// ==== 0x42f0254  FUN_042f0254  (684 bytes)

void FUN_042f0254(long param_1,long *param_2,undefined8 *param_3)

{
  code *pcVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  ulong uVar5;
  long *plVar6;
  long *plVar7;
  int *piVar8;
  int *piVar9;
  long lVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined1 local_410;
  undefined7 uStack_40f;
  undefined7 local_400;
  char cStack_3f9;
  undefined8 local_3f8;
  undefined7 local_3e8;
  char cStack_3e1;
  undefined1 auStack_3d8 [416];
  char local_238;
  undefined1 local_230;
  undefined7 uStack_22f;
  undefined7 local_220;
  char cStack_219;
  undefined8 local_218;
  undefined7 local_208;
  char cStack_201;
  undefined1 auStack_1f8 [416];
  char local_58;
  
  if (*(long *)(param_1 + 0x28) == 0) {
    local_230 = 0;
    local_58 = '\0';
    (*(code *)param_3[2])(param_3,&local_230);
    if (local_58 == '\0') {
      return;
    }
    FUN_04295208(auStack_1f8);
    if (cStack_201 < '\0') {
      FUN_00005810(local_218,CONCAT17(cStack_201,local_208) & 0x7fffffffffffffff);
    }
    if (-1 < cStack_219) {
      return;
    }
    uVar12 = CONCAT71(uStack_22f,local_230);
    uVar5 = CONCAT17(cStack_219,local_220);
  }
  else {
    if ((*param_2 != 0) && (lVar4 = *(long *)(*param_2 + 0xc0), lVar4 != 0)) {
      plVar6 = *(long **)(lVar4 + 0x8d8);
      if (plVar6 == (long *)0x0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42f04ec);
        (*pcVar1)();
      }
      if (*(undefined1 **)(*plVar6 + 0xd0) == &LAB_04152228) {
        do {
          plVar7 = plVar6;
          plVar6 = (long *)plVar7[0x2f];
        } while ((long *)plVar7[0x2f] != (long *)0x0);
      }
      else {
        do {
          plVar7 = plVar6;
          plVar6 = (long *)plVar7[0x2f];
        } while ((long *)plVar7[0x2f] != (long *)0x0);
      }
      if ((*(byte *)(plVar7 + 0x219) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x42f04f4);
        (*pcVar1)();
      }
      lVar10 = plVar7[0x1de];
      piVar8 = *(int **)("/System/Library/Frameworks/CoreServices.framework/Versions/A/CoreServices"
                        + lVar4 + 0x20);
      if ((piVar8 != (int *)0x0) && (iVar2 = *piVar8, *piVar8 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42f04f8);
        (*pcVar1)();
      }
      uVar12 = *(undefined8 *)
                ("/System/Library/Frameworks/CoreServices.framework/Versions/A/CoreServices" +
                lVar4 + 0x28);
      piVar9 = *(int **)(lVar10 + 0x218);
      if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42f0504);
        (*pcVar1)();
      }
      uVar13 = *(undefined8 *)(lVar10 + 0x220);
      uVar11 = *(undefined8 *)(param_1 + 0x28);
      puVar3 = (undefined4 *)FUN_00008790(0x68);
      *puVar3 = 1;
      *(code **)(puVar3 + 2) = FUN_042f102c;
      *(code **)(puVar3 + 4) = FUN_042f1760;
      *(code **)(puVar3 + 6) = FUN_00b77080;
      *(int **)(puVar3 + 10) = piVar8;
      if ((piVar8 != (int *)0x0) && (iVar2 = *piVar8, *piVar8 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42f0510);
        (*pcVar1)();
      }
      *(undefined8 *)(puVar3 + 0xc) = uVar12;
      *(int **)(puVar3 + 0xe) = piVar9;
      if ((piVar9 != (int *)0x0) && (iVar2 = *piVar9, *piVar9 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x42f051c);
        (*pcVar1)();
      }
      *(undefined8 *)(puVar3 + 0x10) = uVar13;
      uVar12 = *param_3;
      uVar14 = param_3[3];
      uVar13 = param_3[2];
      *(undefined8 *)(puVar3 + 0x14) = param_3[1];
      *(undefined8 *)(puVar3 + 0x12) = uVar12;
      lVar4 = param_3[3];
      *(undefined8 *)(puVar3 + 0x18) = uVar14;
      *(undefined8 *)(puVar3 + 0x16) = uVar13;
      if (*(long *)(lVar4 + 8) != 0) {
        param_3[2] = 0;
        param_3[3] = &DAT_0c4b3500;
      }
      FUN_041ba8a0(uVar11,lVar10);
      if (piVar9 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar9;
        *piVar9 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          FUN_00005810(piVar9,8);
        }
      }
      if (piVar8 == (int *)0x0) {
        return;
      }
      LOAcquire();
      iVar2 = *piVar8;
      *piVar8 = iVar2 + -1;
      LORelease();
      if (iVar2 != 1) {
        return;
      }
      FUN_00005810(piVar8,8);
      return;
    }
    local_410 = 0;
    local_238 = '\0';
    (*(code *)param_3[2])(param_3,&local_410);
    if (local_238 == '\0') {
      return;
    }
    FUN_04295208(auStack_3d8);
    if (cStack_3e1 < '\0') {
      FUN_00005810(local_3f8,CONCAT17(cStack_3e1,local_3e8) & 0x7fffffffffffffff);
    }
    if (-1 < cStack_3f9) {
      return;
    }
    uVar12 = CONCAT71(uStack_40f,local_410);
    uVar5 = CONCAT17(cStack_3f9,local_400);
  }
  FUN_00005810(uVar12,uVar5 & 0x7fffffffffffffff);
  return;
}


