// ==== 0x442a794  FUN_0442a794  (968 bytes)

ulong FUN_0442a794(long *param_1,long param_2)

{
  uint uVar1;
  code *pcVar2;
  ulong uVar3;
  uint *puVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  long local_70;
  long lStack_68;
  ulong local_60;
  undefined8 local_58;
  long local_50;
  long lStack_48;
  ulong local_40;
  long local_38;
  
  puVar4 = *(uint **)(param_2 + 0x18);
  uVar1 = puVar4[3];
  if ((int)uVar1 < 0x594dd0c8) {
    if (uVar1 != 0x3190b3f3) {
      if (uVar1 != 0x4f2747fa) {
        if (uVar1 != 0x41dae62e) {
          return 0;
        }
        if (puVar4[1] < 2) {
          local_50 = (long)puVar4 + (ulong)*puVar4;
        }
        else {
          lVar7 = *(long *)(puVar4 + 8);
          local_50 = 0;
          if (lVar7 != 0) {
            local_50 = (long)(puVar4 + 8) + lVar7;
          }
        }
        lStack_68 = 0;
        local_70 = 0;
        local_58 = 0;
        local_60 = 0;
        lVar7 = *(long *)(local_50 + 8);
        local_38 = 0;
        if (lVar7 != 0) {
          local_38 = local_50 + 8 + lVar7;
        }
        lStack_48 = param_2;
        uVar3 = FUN_04d8fe18(&local_38,&local_70,&lStack_48);
        if ((uVar3 & 1) == 0) {
          FUN_084a997c(param_2,0x11,"arc.mojom.RenderFrameAgent",2,0);
        }
        else {
          (**(code **)(*param_1 + 0x20))(param_1,&local_70);
        }
        if ((char)local_58 == '\0') {
          return uVar3;
        }
        if (local_70 == 0) {
          return uVar3;
        }
        if (lStack_68 * 4 != 0) {
          lVar7 = lStack_68 * -4;
          lVar5 = lStack_68 * 4 + local_70;
          do {
            lVar5 = lVar5 + -4;
            if (lVar5 == 0) goto LAB_0442ab10;
            lVar7 = lVar7 + 4;
          } while (lVar7 != 0);
        }
        lStack_68 = 0;
        FUN_00005810(local_70,local_60 << 2);
        return uVar3;
      }
      if (puVar4[1] < 2) {
        lVar7 = (long)puVar4 + (ulong)*puVar4;
      }
      else {
        lVar5 = *(long *)(puVar4 + 8);
        lVar7 = 0;
        if (lVar5 != 0) {
          lVar7 = (long)(puVar4 + 8) + lVar5;
        }
      }
      pcVar2 = *(code **)(*param_1 + 0x30);
      goto LAB_0442a8fc;
    }
    if (puVar4[1] < 2) {
      lVar7 = (long)puVar4 + (ulong)*puVar4;
    }
    else {
      lVar5 = *(long *)(puVar4 + 8);
      lVar7 = 0;
      if (lVar5 != 0) {
        lVar7 = (long)(puVar4 + 8) + lVar5;
      }
    }
    local_70 = 0;
    lStack_68 = 0;
    local_60 = 0;
    local_50 = 0;
    lStack_48 = 0;
    local_40 = 0;
    lVar6 = *(long *)(lVar7 + 8);
    lVar5 = lVar7 + 8 + lVar6;
    if (lVar6 != 0 && lVar5 != 0) {
      if (lVar5 == -8 && *(int *)(lVar5 + 4) != 0) goto LAB_0442ab10;
      FUN_0043b8b0(&local_70);
    }
    lVar5 = *(long *)(lVar7 + 0x10);
    lVar7 = lVar7 + 0x10 + lVar5;
    if (lVar5 != 0 && lVar7 != 0) {
      if (lVar7 == -8 && *(int *)(lVar7 + 4) != 0) goto LAB_0442ab10;
      FUN_0043b8b0(&local_50);
    }
    (**(code **)(*param_1 + 0x10))(param_1,&local_70,&local_50);
    if ((long)local_40 < 0) {
      FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
    }
  }
  else {
    if (uVar1 == 0x594dd0c8) {
      if (puVar4[1] < 2) {
        lVar7 = (long)puVar4 + (ulong)*puVar4;
      }
      else {
        lVar5 = *(long *)(puVar4 + 8);
        lVar7 = 0;
        if (lVar5 != 0) {
          lVar7 = (long)(puVar4 + 8) + lVar5;
        }
      }
      if ((*(byte *)(lVar7 + 8) & 1) == 0) {
        uVar3 = 0;
      }
      else {
        uVar3 = (ulong)*(uint *)(lVar7 + 0xc) | 0x100000000;
      }
      (**(code **)(*param_1 + 0x28))(param_1,uVar3);
      return 1;
    }
    if (uVar1 == 0x664c8cd1) {
      if (puVar4[1] < 2) {
        lVar7 = (long)puVar4 + (ulong)*puVar4;
      }
      else {
        lVar5 = *(long *)(puVar4 + 8);
        lVar7 = 0;
        if (lVar5 != 0) {
          lVar7 = (long)(puVar4 + 8) + lVar5;
        }
      }
      pcVar2 = *(code **)(*param_1 + 0x38);
LAB_0442a8fc:
      (*pcVar2)(param_1,*(byte *)(lVar7 + 8) & 1);
      return 1;
    }
    if (uVar1 != 0x7a0c874f) {
      return 0;
    }
    if (puVar4[1] < 2) {
      lVar7 = (long)puVar4 + (ulong)*puVar4;
    }
    else {
      lVar5 = *(long *)(puVar4 + 8);
      lVar7 = 0;
      if (lVar5 != 0) {
        lVar7 = (long)(puVar4 + 8) + lVar5;
      }
    }
    local_70 = 0;
    lStack_68 = 0;
    local_60 = 0;
    lVar5 = *(long *)(lVar7 + 8);
    lVar7 = lVar7 + 8 + lVar5;
    if (lVar5 != 0 && lVar7 != 0) {
      if (lVar7 == -8 && *(int *)(lVar7 + 4) != 0) {
LAB_0442ab10:
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x442ab14);
        (*pcVar2)();
      }
      FUN_0043b8b0(&local_70);
    }
    (**(code **)(*param_1 + 0x18))(param_1,&local_70);
  }
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  return 1;
}


