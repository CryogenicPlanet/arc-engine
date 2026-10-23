// ==== 0x437854c  FUN_0437854c  (864 bytes)

void FUN_0437854c(long param_1,int param_2)

{
  byte bVar1;
  bool bVar2;
  code *pcVar3;
  int iVar4;
  char *pcVar5;
  long *plVar6;
  undefined4 *puVar7;
  undefined8 *puVar8;
  int *piVar9;
  undefined8 uVar10;
  undefined6 local_50;
  undefined2 uStack_4a;
  undefined2 uStack_48;
  uint uStack_46;
  char acStack_42 [18];
  
  if (*(long *)(param_1 + 0x130) == 0) {
    return;
  }
  if (((*(long *)(param_1 + 0x160) != 0) && (*(char *)(*(long *)(param_1 + 0x160) + 4) == '\0')) &&
     (*(long *)(param_1 + 0x168) != 0)) {
    return;
  }
  *(bool *)(param_1 + 0x159) = param_2 == 0;
  *(undefined1 *)(param_1 + 0x15a) = 0;
  if (((*(byte *)(param_1 + 0x358) & 1) != 0) || (bVar2 = false, *(long *)(param_1 + 0x360) != 0)) {
    *(int *)(param_1 + 0x5d4) = *(int *)(param_1 + 0x5d4) + 1;
    acStack_42[9] = 0x13;
    if ("ui_delegate_wrapper" < acStack_42 + 5 && &local_50 < "i_delegate_wrapper")
    goto LAB_043788a0;
    uStack_48 = 0x7461;
    uStack_46 = 0x72775f65;
    builtin_strncpy(acStack_42,"apper",6);
    local_50 = 0x6c65645f6975;
    uStack_4a = 0x6765;
    FUN_043788bc(&local_50);
    bVar2 = true;
  }
  if (((*(byte *)(param_1 + 0x3b8) & 1) != 0) || (*(long *)(param_1 + 0x3c0) != 0)) {
    *(int *)(param_1 + 0x5d8) = *(int *)(param_1 + 0x5d8) + 1;
    pcVar5 = (char *)FUN_00008790(0x20);
    local_50 = SUB86(pcVar5,0);
    uStack_4a = (undefined2)((ulong)pcVar5 >> 0x30);
    acStack_42[5] = 0;
    acStack_42[6] = '\0';
    acStack_42[7] = '\0';
    acStack_42[8] = 0;
    acStack_42[9] = 0x80;
    uStack_48 = 0x1c;
    uStack_46 = 0;
    acStack_42[0] = '\0';
    acStack_42[1] = '\0';
    acStack_42[2] = ' ';
    acStack_42[3] = '\0';
    acStack_42[4] = '\0';
    if ((pcVar5 + 0x1c < pcVar5) ||
       ("navigation_delegate_wrapper_" < pcVar5 + 0x1c && pcVar5 < "avigation_delegate_wrapper_"))
    goto LAB_043788a0;
    builtin_strncpy(pcVar5,"navigation_delegate_wrapper_",0x1d);
    FUN_043788bc(&local_50,*(undefined4 *)(param_1 + 0x5d8));
    FUN_00005810(pcVar5,0x20);
    bVar2 = true;
  }
  if (*(long *)(param_1 + 0x470) != 0) {
    *(int *)(param_1 + 0x5dc) = *(int *)(param_1 + 0x5dc) + 1;
    acStack_42[9] = 10;
    if ("observers_" < &uStack_46 && &local_50 < "bservers_") goto LAB_043788a0;
    uStack_48 = 0x5f73;
    local_50 = 0x76726573626f;
    uStack_4a = 0x7265;
    uStack_46 = uStack_46 & 0xffffff00;
    FUN_043788bc(&local_50);
    bVar2 = true;
  }
  if (*(long *)(*(long *)(param_1 + 0x2d8) + 0xb8) == 0) {
    if (((*(byte *)(*(long *)(param_1 + 0x2c8) + 0x50) & 1) == 0) &&
       (*(long *)(*(long *)(param_1 + 0x2c8) + 0x58) == 0)) {
      if (!bVar2) {
        *(undefined4 *)(param_1 + 0x5e4) = 0;
        *(undefined8 *)(param_1 + 0x5dc) = 0;
        *(undefined8 *)(param_1 + 0x5d4) = 0;
        FUN_0679dfc4(*(undefined8 *)(param_1 + 0x130));
        return;
      }
      goto LAB_04378788;
    }
  }
  else {
    *(int *)(param_1 + 0x5e0) = *(int *)(param_1 + 0x5e0) + 1;
    acStack_42[9] = 0x16;
    if ("navigation_controller_" < acStack_42 + 8 && &local_50 < "avigation_controller_")
    goto LAB_043788a0;
    uStack_48 = 0x6e6f;
    uStack_46 = 0x6e6f635f;
    local_50 = 0x61676976616e;
    uStack_4a = 0x6974;
    builtin_strncpy(acStack_42,"troller_",9);
    FUN_043788bc(&local_50);
    if (((*(byte *)(*(long *)(param_1 + 0x2c8) + 0x50) & 1) == 0) &&
       (*(long *)(*(long *)(param_1 + 0x2c8) + 0x58) == 0)) goto LAB_04378788;
  }
  *(int *)(param_1 + 0x5e4) = *(int *)(param_1 + 0x5e4) + 1;
  acStack_42[9] = 0xe;
  if ("media_session_" < acStack_42 && &local_50 < "edia_session_") {
LAB_043788a0:
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43788a4);
    (*pcVar3)();
  }
  local_50 = 0x5f616964656d;
  uStack_4a = 0x6573;
  uStack_48 = 0x7373;
  uStack_46 = 0x5f6e6f69;
  acStack_42[0] = '\0';
  FUN_043788bc(&local_50);
LAB_04378788:
  plVar6 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
  if (((undefined8 *)*plVar6 == (undefined8 *)0x0) ||
     (puVar8 = *(undefined8 **)*plVar6, puVar8 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x43788a8);
    (*pcVar3)();
  }
  FUN_00029e40(&local_50,"Close","../../arc/browser/web_contents/web_contents_impl.cc",0x7ec);
  piVar9 = *(int **)(param_1 + 0x610);
  if ((piVar9 != (int *)0x0) && (iVar4 = *piVar9, *piVar9 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x43788b4);
    (*pcVar3)();
  }
  uVar10 = *(undefined8 *)(param_1 + 0x618);
  bVar1 = *(byte *)(param_1 + 0x159);
  puVar7 = (undefined4 *)FUN_00008790(0x48);
  *puVar7 = 1;
  *(undefined1 **)(puVar7 + 2) = &LAB_041bc8a8;
  *(code **)(puVar7 + 4) = FUN_040b2900;
  *(undefined1 **)(puVar7 + 6) = &LAB_04169bc0;
  *(code **)(puVar7 + 8) = FUN_0437854c;
  *(undefined8 *)(puVar7 + 10) = 0;
  *(int **)(puVar7 + 0xc) = piVar9;
  *(undefined8 *)(puVar7 + 0xe) = uVar10;
  puVar7[0x10] = bVar1 ^ 1;
  (**(code **)*puVar8)(puVar8,&local_50,puVar7,0);
  return;
}


