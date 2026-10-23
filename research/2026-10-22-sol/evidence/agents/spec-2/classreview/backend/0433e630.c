// ==== 0x433e630  FUN_0433e630  (600 bytes)

/* WARNING: Removing unreachable block (ram,0x0433e6c0) */
/* WARNING: Removing unreachable block (ram,0x0433e6e0) */

void FUN_0433e630(long *param_1,long param_2)

{
  code *pcVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  char *pcVar5;
  ulong uVar6;
  long lVar7;
  long local_70;
  long lStack_68;
  long local_60;
  ulong local_58;
  long lStack_50;
  long local_48;
  
  param_1[1] = 0;
  param_1[2] = 0;
  *param_1 = 0;
  uVar6 = *(ulong *)(param_2 + 0x278);
  if (uVar6 != 0) {
    if (0x555555555555555 < uVar6) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    local_70 = FUN_00008790(uVar6 * 0x30);
    local_60 = 0;
    lStack_68 = local_70;
    local_58 = uVar6;
    FUN_0433ef90(&local_70,param_1,param_1 + 1,param_1 + 2);
    lVar3 = local_60 * 0x30;
    if (lVar3 != 0) {
      lVar4 = 0;
      lVar7 = lStack_68;
      do {
        if (lVar7 + lVar3 == 0x30) goto LAB_0433e870;
        lVar7 = lVar7 + -0x30;
        lVar4 = lVar4 + 0x30;
      } while (lVar3 - lVar4 != 0);
    }
    if (local_70 != 0) {
      FUN_00005810(local_70,local_58 * 0x30);
    }
    lVar3 = *(long *)(param_2 + 0x278) * 0x60;
    if (lVar3 != 0) {
      pcVar5 = (char *)(*(long *)(param_2 + 0x270) + 0x17);
      do {
        if (*pcVar5 < '\0') {
          FUN_00054420(&local_70,*(undefined8 *)(pcVar5 + -0x17),*(undefined8 *)(pcVar5 + -0xf));
        }
        else {
          lStack_68 = *(long *)(pcVar5 + -0xf);
          local_70 = *(long *)(pcVar5 + -0x17);
          local_60 = *(long *)(pcVar5 + -7);
        }
        if (pcVar5[0x18] < '\0') {
          FUN_00054420(&local_58,*(undefined8 *)(pcVar5 + 1),*(undefined8 *)(pcVar5 + 9));
        }
        else {
          lStack_50 = *(long *)(pcVar5 + 9);
          local_58 = *(ulong *)(pcVar5 + 1);
          local_48 = *(long *)(pcVar5 + 0x11);
        }
        if (param_1[1] == param_1[2]) {
          plVar2 = (long *)FUN_0433f0f8(param_1,&local_70);
        }
        else {
          plVar2 = (long *)(*param_1 + param_1[1] * 0x30);
          if (plVar2 == (long *)0x0) {
LAB_0433e870:
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(1,0x433e874);
            (*pcVar1)();
          }
          plVar2[2] = local_60;
          plVar2[1] = lStack_68;
          *plVar2 = local_70;
          lStack_68 = 0;
          local_60 = 0;
          local_70 = 0;
          plVar2[5] = local_48;
          plVar2[4] = lStack_50;
          plVar2[3] = local_58;
          lStack_50 = 0;
          local_48 = 0;
          local_58 = 0;
          plVar2 = plVar2 + 6;
        }
        param_1[1] = ((long)plVar2 - *param_1 >> 4) * -0x5555555555555555;
        if ((long)plVar2 - *param_1 == 0) goto LAB_0433e870;
        if (local_48 < 0) {
          FUN_00005810(local_58,local_48 << 1);
        }
        if (local_60 < 0) {
          FUN_00005810(local_70,local_60 << 1);
        }
        pcVar5 = pcVar5 + 0x60;
        lVar3 = lVar3 + -0x60;
      } while (lVar3 != 0);
    }
  }
  return;
}


