// ==== 0x4332040  FUN_04332040  (264 bytes)

undefined4 * FUN_04332040(undefined4 *param_1,undefined4 *param_2)

{
  undefined8 *puVar1;
  ulong uVar2;
  char cVar3;
  byte bVar4;
  undefined8 *puVar5;
  code *pcVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  
  *param_1 = *param_2;
  puVar5 = (undefined8 *)(param_1 + 2);
  puVar1 = (undefined8 *)(param_2 + 2);
  cVar3 = *(char *)(param_1 + 8);
  if (cVar3 == *(char *)(param_2 + 8)) {
    if (cVar3 != '\0' && param_1 != param_2) {
      bVar4 = *(byte *)((long)param_2 + 0x1f);
      if (*(char *)((long)param_1 + 0x1f) < '\0') {
        uVar2 = *(ulong *)(param_2 + 4);
        puVar5 = *(undefined8 **)(param_2 + 2);
        if (-1 < (char)bVar4) {
          uVar2 = (ulong)bVar4;
          puVar5 = puVar1;
        }
        FUN_00359ad8(param_1 + 2,puVar5,uVar2);
      }
      else if ((char)bVar4 < '\0') {
        FUN_00359998(param_1 + 2,*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4));
      }
      else {
        uVar8 = *(undefined8 *)(param_2 + 4);
        uVar7 = *puVar1;
        *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 6);
        *(undefined8 *)(param_1 + 4) = uVar8;
        *puVar5 = uVar7;
      }
    }
  }
  else if (cVar3 == '\0') {
    if (puVar5 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x4332108);
      (*pcVar6)();
    }
    if (*(char *)((long)param_2 + 0x1f) < '\0') {
      FUN_00060a60(param_1 + 2,*(undefined8 *)(param_2 + 2),*(undefined8 *)(param_2 + 4));
    }
    else {
      uVar8 = *(undefined8 *)(param_2 + 4);
      uVar7 = *puVar1;
      *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_2 + 6);
      *(undefined8 *)(param_1 + 4) = uVar8;
      *puVar5 = uVar7;
    }
    *(undefined1 *)(param_1 + 8) = 1;
  }
  else {
    if (*(char *)((long)param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 2),*(ulong *)(param_1 + 6) & 0x7fffffffffffffff);
    }
    *(undefined1 *)(param_1 + 8) = 0;
  }
  return param_1;
}


