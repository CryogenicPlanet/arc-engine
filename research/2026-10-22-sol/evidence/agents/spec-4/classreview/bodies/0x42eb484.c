// ==== 0x42eb484  FUN_042eb484  (248 bytes)

void FUN_042eb484(long *param_1,long param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  
  if ((*(long *)(param_2 + 0x68) == 0) || (lVar3 = *(long *)(param_2 + 0x108), lVar3 == 0)) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
  }
  else {
    puVar5 = *(undefined8 **)(lVar3 + 0xc0);
    lVar3 = *(long *)(lVar3 + 200);
    param_1[1] = 0;
    param_1[2] = 0;
    lVar4 = lVar3 * 0x18;
    *param_1 = 0;
    if (lVar4 != 0) {
      uVar6 = (lVar3 * 0x18 >> 3) * -0x5555555555555555;
      if (0xaaaaaaaaaaaaaaa < uVar6) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      puVar2 = (undefined8 *)FUN_00008790(lVar4);
      *param_1 = (long)puVar2;
      param_1[2] = uVar6;
      puVar7 = puVar2;
      do {
        if (puVar7 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x42eb560);
          (*pcVar1)();
        }
        if (*(char *)((long)puVar5 + 0x17) < '\0') {
          FUN_00060a60(puVar7,*puVar5,puVar5[1]);
        }
        else {
          uVar9 = puVar5[1];
          uVar8 = *puVar5;
          puVar7[2] = puVar5[2];
          puVar7[1] = uVar9;
          *puVar7 = uVar8;
        }
        puVar5 = puVar5 + 3;
        puVar7 = puVar7 + 3;
        lVar4 = lVar4 + -0x18;
      } while (lVar4 != 0);
      param_1[1] = ((long)puVar7 - (long)puVar2 >> 3) * -0x5555555555555555;
    }
  }
  return;
}


