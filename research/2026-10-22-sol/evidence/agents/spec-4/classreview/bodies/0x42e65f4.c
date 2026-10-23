// ==== 0x42e65f4  FUN_042e65f4  (412 bytes)

void FUN_042e65f4(long *param_1,long param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  bool bVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  long lVar10;
  long lVar11;
  undefined8 **local_150 [27];
  undefined8 *local_78;
  undefined8 local_70;
  undefined8 local_68;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  if (*(long *)(param_2 + 0x68) != 0) {
    local_78 = (undefined8 *)0x0;
    local_70 = 0;
    local_68 = 0;
    uVar5 = FUN_014baf1c();
    FUN_0417c3f4(&local_78,uVar5,7);
    if (local_78 != &local_70) {
      lVar11 = 0;
      lVar10 = 0;
      puVar9 = local_78;
      do {
        iVar4 = FUN_06b38690(puVar9[7]);
        if (((iVar4 != 0) && (lVar6 = puVar9[7], *(int *)(*(long *)(lVar6 + 0x228) + 0x88) != 2)) &&
           ((lVar7 = *(long *)(param_2 + 0x68), *(char *)(lVar7 + 0x48) == '\0' ||
            ((iVar4 = FUN_0417c57c(lVar6), iVar4 == 0 ||
             (iVar4 = FUN_025490f0(*(undefined8 *)(lVar6 + 0x228),lVar7), iVar4 != 0)))))) {
          FUN_0433513c(local_150,*(undefined8 *)(param_2 + 0x68),puVar9[7]);
          if (lVar11 == param_1[2]) {
            lVar6 = FUN_042ec358(param_1,local_150);
          }
          else {
            lVar10 = lVar10 + lVar11 * 0xd8;
            if (lVar10 == 0) goto LAB_042e6758;
            lVar6 = FUN_04334f1c(lVar10,local_150);
            lVar6 = lVar6 + 0xd8;
          }
          lVar10 = *param_1;
          lVar11 = (lVar6 - lVar10 >> 3) * -0x7b425ed097b425ed;
          param_1[1] = lVar11;
          if (lVar6 - lVar10 == 0) {
LAB_042e6758:
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(1,0x42e675c);
            (*pcVar2)();
          }
          FUN_04334d2c(local_150);
        }
        puVar1 = (undefined8 *)puVar9[1];
        puVar8 = puVar9;
        if ((undefined8 *)puVar9[1] == (undefined8 *)0x0) {
          do {
            puVar9 = (undefined8 *)puVar8[2];
            bVar3 = puVar8 != (undefined8 *)*puVar9;
            puVar8 = puVar9;
          } while (bVar3);
        }
        else {
          do {
            puVar9 = puVar1;
            puVar1 = (undefined8 *)*puVar9;
          } while ((undefined8 *)*puVar9 != (undefined8 *)0x0);
        }
      } while (puVar9 != &local_70);
    }
    local_150[0] = &local_78;
    FUN_01ab1238(local_150,local_70);
  }
  return;
}


