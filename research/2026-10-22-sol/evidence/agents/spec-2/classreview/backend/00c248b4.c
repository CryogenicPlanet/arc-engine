// ==== 0xc248b4  FUN_00c248b4  (328 bytes)

void FUN_00c248b4(undefined8 *param_1,ulong param_2)

{
  ulong uVar1;
  undefined8 *puVar2;
  code *pcVar3;
  undefined8 *puVar4;
  long lVar5;
  ulong uVar6;
  size_t sVar7;
  undefined8 local_58 [2];
  byte local_48;
  byte bStack_47;
  byte bStack_46;
  byte bStack_45;
  byte bStack_44;
  byte bStack_43;
  byte bStack_42;
  byte abStack_41 [9];
  long local_38;
  
  lVar5 = 0;
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_58[0] = 0;
  local_58[1] = 0;
  abStack_41[1] = 0;
  local_48 = 0;
  bStack_47 = 0;
  bStack_46 = 0;
  bStack_45 = 0;
  bStack_44 = 0;
  bStack_43 = 0;
  bStack_42 = 0;
  abStack_41[0] = 0;
  uVar6 = -param_2;
  if (-1 < (long)param_2) {
    uVar6 = param_2;
  }
  do {
    abStack_41[lVar5 + 1] = (char)uVar6 + (char)(uVar6 / 10) * -10 | 0x30;
    if (uVar6 < 10) {
      if ((long)param_2 < 0) {
        uVar6 = lVar5 + 0x17;
        if (0x18 < uVar6) break;
        abStack_41[lVar5] = 0x2d;
      }
      else {
        uVar6 = lVar5 + 0x18;
      }
      sVar7 = 0x19 - uVar6;
      if (uVar6 < 3) {
        puVar4 = (undefined8 *)FUN_00008790(0x20);
        *param_1 = puVar4;
        param_1[1] = sVar7;
        param_1[2] = &DAT_8000000000000020;
      }
      else {
        *(char *)((long)param_1 + 0x17) = (char)sVar7;
        puVar4 = param_1;
      }
      if ((puVar4 <= (undefined8 *)((long)puVar4 + sVar7)) &&
         (puVar2 = (undefined8 *)((long)local_58 + uVar6),
         (undefined8 *)((long)puVar4 + sVar7) <= puVar2 || puVar2 < puVar4)) {
        _memcpy(puVar4,puVar2,sVar7);
        *(undefined1 *)((long)puVar4 + sVar7) = 0;
        if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
          return;
        }
                    /* WARNING: Subroutine does not return */
        ___stack_chk_fail();
      }
      break;
    }
    uVar1 = lVar5 + 0x17;
    lVar5 = lVar5 + -1;
    uVar6 = uVar6 / 10;
  } while (uVar1 < 0x19);
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0xc249fc);
  (*pcVar3)();
}


