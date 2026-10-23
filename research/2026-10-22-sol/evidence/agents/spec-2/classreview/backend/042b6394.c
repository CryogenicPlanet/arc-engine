// ==== 0x42b6394  FUN_042b6394  (804 bytes)

void FUN_042b6394(undefined8 *param_1,long param_2,undefined8 *param_3)

{
  undefined8 *puVar1;
  char cVar2;
  undefined8 ***pppuVar3;
  long lVar4;
  long lVar5;
  ulong uVar6;
  undefined8 uVar7;
  long lVar8;
  undefined4 *puVar9;
  byte bVar10;
  ulong uVar11;
  code *pcVar12;
  undefined8 uVar13;
  long *plVar14;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined8 **local_70;
  ulong local_68;
  undefined8 local_60;
  long local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  local_70 = (undefined8 ***)0x0;
  local_68 = 0;
  local_60 = 0;
  cVar2 = *(char *)((long)param_3 + 0x17);
  puVar1 = (undefined8 *)*param_3;
  if (-1 < (long)cVar2) {
    puVar1 = param_3;
  }
  lVar5 = param_3[1];
  if (-1 < cVar2) {
    lVar5 = (long)cVar2;
  }
  FUN_01c39a30(&local_70,puVar1,lVar5);
  lVar5 = FUN_01a29860(DAT_0e481618);
  uVar11 = (ulong)local_60._7_1_;
  uVar6 = local_68;
  if (-1 < (long)local_60) {
    uVar6 = uVar11;
  }
  if (uVar6 == 0) {
LAB_042b6560:
    uVar7 = FUN_04b37104(0xd,"Default profile cannot be deleted",0x21,0x2c5,
                         "../../arc/browser/browser_application_impl.cc");
    *param_1 = uVar7;
    goto LAB_042b6504;
  }
  bVar10 = local_60._7_1_;
  if (*(int *)(DAT_0e4ac750 + 0x10) == 0) {
    local_58 = 0;
    uStack_50 = 0;
    local_48 = 0;
    FUN_01b92818(&local_88,&local_58,"Default",7);
    uVar6 = FUN_00df8f58(&local_70,&local_88);
    if ((long)local_78 < 0) {
      FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
    }
    if ((uVar6 & 1) != 0) goto LAB_042b6560;
    uVar11 = local_60 >> 0x38;
    bVar10 = (byte)(local_60 >> 0x38);
  }
  local_88 = 0;
  uStack_80 = 0;
  local_78 = 0;
  uVar6 = local_68;
  pppuVar3 = (undefined8 ***)local_70;
  if (-1 < (char)bVar10) {
    uVar6 = uVar11;
    pppuVar3 = &local_70;
  }
  FUN_01b92818(&local_88,lVar5 + 0x50,pppuVar3,uVar6);
  uVar7 = FUN_01a298f8(lVar5);
  lVar8 = FUN_01a299e0(uVar7,&local_88);
  if (lVar8 == 0) {
    uVar7 = FUN_04b37104(0x15,"profile does not exist!",0x17,0x2cd,
                         "../../arc/browser/browser_application_impl.cc");
  }
  else {
    if ((*(byte *)(DAT_0e4ac750 + 0x24) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar12 = (code *)SoftwareBreakpoint(0,0x42b66b4);
      (*pcVar12)();
    }
    if (((*(int *)(DAT_0e4ac750 + 0x20) == 0) &&
        (lVar8 = FUN_042c1ad4(param_2 + 0x100,&local_70), lVar8 != 0)) &&
       (FUN_04080740(&local_58,param_2 + 0x100,lVar8), lVar8 = local_58, local_58 != 0)) {
      if ((char)local_48 != '\0') {
        if (local_58 == -0x10) {
                    /* WARNING: Does not return */
          pcVar12 = (code *)SoftwareBreakpoint(1,0x42b66bc);
          (*pcVar12)();
        }
        plVar14 = *(long **)(local_58 + 0x30);
        if (plVar14 != (long *)0x0) {
          LOAcquire();
          lVar4 = plVar14[1];
          plVar14[1] = lVar4 + -1;
          LORelease();
          if (lVar4 == 0) {
            (**(code **)(*plVar14 + 0x10))(plVar14);
            if (plVar14[2] != 0) {
              LOAcquire();
              lVar4 = plVar14[2];
              plVar14[2] = lVar4 + -1;
              LORelease();
              if (lVar4 != 0) goto LAB_042b65d8;
            }
            pcVar12 = *(code **)(*plVar14 + 0x20);
            if (((pcVar12 == FUN_00f01bc0) || (pcVar12 == FUN_00f01bc0)) ||
               (pcVar12 == FUN_013efb40)) {
              FUN_00005810(plVar14,0x20);
            }
            else {
              (*pcVar12)(plVar14);
            }
          }
        }
LAB_042b65d8:
        if (*(char *)(lVar8 + 0x27) < '\0') {
          FUN_00005810(*(undefined8 *)(lVar8 + 0x10),*(ulong *)(lVar8 + 0x20) & 0x7fffffffffffffff);
        }
      }
      FUN_00005810(lVar8,0x38);
    }
    uVar13 = *(undefined8 *)(lVar5 + 0xa0);
    puVar9 = (undefined4 *)FUN_00008790(0x28);
    uVar7 = 1;
    *puVar9 = 1;
    *(code **)(puVar9 + 2) = ArcWebContentsConfiguration::_cxx_destruct;
    *(code **)(puVar9 + 4) = FUN_00b6d3c0;
    *(code **)(puVar9 + 6) = FUN_00b77080;
    FUN_07bde3bc(uVar13,&local_88,puVar9,1);
  }
  *param_1 = uVar7;
  if ((long)local_78 < 0) {
    FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
  }
LAB_042b6504:
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  return;
}


