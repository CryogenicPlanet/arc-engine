// ==== 0x43a809c  FUN_043a809c  (832 bytes)

void FUN_043a809c(long param_1,undefined8 param_2)

{
  code *pcVar1;
  int iVar2;
  int *piVar3;
  long lVar4;
  int *piVar5;
  undefined8 uVar6;
  int *local_68;
  char *local_60;
  undefined6 uStack_58;
  undefined2 uStack_52;
  undefined6 uStack_50;
  char cStack_4a;
  char cStack_49;
  
  lVar4 = *(long *)(param_1 + 0x68);
  piVar5 = *(int **)(lVar4 + 0x60);
  if ((piVar5 != (int *)0x0) && (iVar2 = *piVar5, *piVar5 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x43a83c4);
    (*pcVar1)();
  }
  uVar6 = *(undefined8 *)(lVar4 + 0x68);
  piVar3 = (int *)FUN_00008790(0x40);
  *(undefined1 **)(piVar3 + 2) = &LAB_043a84ec;
  *(code **)(piVar3 + 4) = FUN_040742c0;
  *(undefined1 **)(piVar3 + 6) = &LAB_040c0f80;
  *(undefined1 **)(piVar3 + 8) = &LAB_043a8a50;
  piVar3[10] = 0;
  piVar3[0xb] = 0;
  *(int **)(piVar3 + 0xc) = piVar5;
  *piVar3 = 1;
  *(undefined8 *)(piVar3 + 0xe) = uVar6;
  cStack_49 = '\x16';
  if (&cStack_4a <= "arc.mojom.EffectsAgent" || "arc.mojom.EffectsAgent" < &local_60) {
    uStack_58 = 0x656666452e6d;
    local_60 = (char *)0x6f6a6f6d2e637261;
    uStack_52 = 0x7463;
    uStack_50 = 0x746e65674173;
    cStack_4a = '\0';
    local_68 = (int *)FUN_00008790(0x30);
    *local_68 = 1;
    *(undefined1 **)(local_68 + 2) = &LAB_01fdf600;
    *(code **)(local_68 + 4) = FUN_0407a980;
    *(code **)(local_68 + 6) = FUN_00b77080;
    *(undefined1 **)(local_68 + 8) = &LAB_043a871c;
    *(int **)(local_68 + 10) = piVar3;
    if (piVar3 != (int *)0x0) {
      iVar2 = *piVar3;
      *piVar3 = iVar2 + 1;
      if (iVar2 < 1) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x43a83dc);
        (*pcVar1)();
      }
      if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x43a83e8);
        (*pcVar1)();
      }
    }
    FUN_018aade4(param_2,&local_60,&local_68);
    if (local_68 != (int *)0x0) {
      LOAcquire();
      iVar2 = *local_68;
      *local_68 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(local_68 + 4))();
      }
    }
    if (cStack_49 < '\0') {
      FUN_00005810(local_60,CONCAT17(cStack_49,CONCAT16(cStack_4a,uStack_50)) & 0x7fffffffffffffff);
    }
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        (**(code **)(piVar3 + 4))(piVar3);
      }
    }
    lVar4 = *(long *)(param_1 + 0x60);
    piVar5 = *(int **)(lVar4 + 0x180);
    if ((piVar5 != (int *)0x0) && (iVar2 = *piVar5, *piVar5 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x43a83d0);
      (*pcVar1)();
    }
    uVar6 = *(undefined8 *)(lVar4 + 0x188);
    piVar3 = (int *)FUN_00008790(0x40);
    *piVar3 = 1;
    *(undefined1 **)(piVar3 + 2) = &LAB_043a84ec;
    *(code **)(piVar3 + 4) = FUN_040742c0;
    *(undefined1 **)(piVar3 + 6) = &LAB_040c0f80;
    *(undefined1 **)(piVar3 + 8) = &LAB_043a482c;
    piVar3[10] = 0;
    piVar3[0xb] = 0;
    *(int **)(piVar3 + 0xc) = piVar5;
    *(undefined8 *)(piVar3 + 0xe) = uVar6;
    local_60 = (char *)FUN_00008790(0x20);
    uStack_50 = 0x20;
    cStack_4a = '\0';
    cStack_49 = -0x80;
    uStack_58 = 0x18;
    uStack_52 = 0;
    if ((local_60 <= local_60 + 0x18) &&
       (local_60 + 0x18 <= "arc.mojom.AdBlockerAgent" || "arc.mojom.AdBlockerAgent" < local_60)) {
      builtin_strncpy(local_60,"arc.mojom.AdBlockerAgent",0x19);
      piVar5 = (int *)FUN_00008790(0x30);
      *piVar5 = 1;
      *(undefined1 **)(piVar5 + 2) = &LAB_01fdf600;
      *(code **)(piVar5 + 4) = FUN_0407a980;
      *(code **)(piVar5 + 6) = FUN_00b77080;
      *(undefined1 **)(piVar5 + 8) = &LAB_043a871c;
      *(int **)(piVar5 + 10) = piVar3;
      if (piVar3 != (int *)0x0) {
        iVar2 = *piVar3;
        *piVar3 = iVar2 + 1;
        if (iVar2 < 1) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x43a83f4);
          (*pcVar1)();
        }
        if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x43a8400);
          (*pcVar1)();
        }
      }
      local_68 = piVar5;
      FUN_018aade4(param_2,&local_60,&local_68);
      if (local_68 != (int *)0x0) {
        LOAcquire();
        iVar2 = *local_68;
        *local_68 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          (**(code **)(local_68 + 4))();
        }
      }
      if (cStack_49 < '\0') {
        FUN_00005810(local_60,CONCAT17(cStack_49,CONCAT16(cStack_4a,uStack_50)) & 0x7fffffffffffffff
                    );
      }
      if (piVar3 != (int *)0x0) {
        LOAcquire();
        iVar2 = *piVar3;
        *piVar3 = iVar2 + -1;
        LORelease();
        if (iVar2 == 1) {
          (**(code **)(piVar3 + 4))(piVar3);
        }
      }
      return;
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43a83cc);
  (*pcVar1)();
}


