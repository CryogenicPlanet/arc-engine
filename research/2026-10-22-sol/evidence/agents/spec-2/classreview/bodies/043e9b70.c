// ==== 0x43e9b70  ArcBrowserApplication::setBlockLists:  (516 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setBlockLists_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  long lVar6;
  long local_88;
  long local_80;
  ID IStack_78;
  undefined *local_70;
  undefined8 uStack_68;
  undefined1 *local_60;
  undefined *puStack_58;
  long *local_50;
  long local_48;
  long local_40;
  ID IStack_38;
  
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  IVar4 = _objc_msgSend(param_3,"count");
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_48 = FUN_00008790(IVar4 * 0x18);
    local_40 = 0;
    IStack_38 = IVar4;
  }
  local_70 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_68 = 0xd0800000;
  local_60 = &LAB_043d763c;
  puStack_58 = &DAT_0df6b218;
  local_50 = &local_48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_70);
  IStack_78 = IStack_38;
  local_80 = local_40;
  local_88 = local_48;
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  FUN_04332374(&local_88);
  lVar2 = local_88;
  if (local_88 != 0) {
    if (local_80 * 0x18 != 0) {
      lVar6 = local_80 * -0x18;
      puVar1 = (undefined8 *)(local_80 * 0x18 + local_88);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_043e9e88;
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_80 = (lVar2 - local_88 >> 3) * -0x5555555555555555;
    FUN_00005810(local_88,IStack_78 * 0x18);
  }
  lVar2 = local_48;
  if (local_48 != 0) {
    if (local_40 * 0x18 != 0) {
      lVar6 = local_40 * -0x18;
      puVar1 = (undefined8 *)(local_40 * 0x18 + local_48);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) {
LAB_043e9e88:
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x43e9e8c);
          (*pcVar3)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_40 = (lVar2 - local_48 >> 3) * -0x5555555555555555;
    FUN_00005810(local_48,IStack_38 * 0x18);
  }
  return;
}


