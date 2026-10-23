// ==== 0x42bdb68  FUN_042bdb68  (208 bytes)

void FUN_042bdb68(undefined8 param_1,long param_2,undefined8 param_3,undefined8 param_4)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  
  lVar4 = *(long *)(param_2 + 0x448);
  if (lVar4 == 0) {
    if (((DAT_0e49b470 == 0) && (FUN_027480a8(), DAT_0e49b470 == 0)) &&
       (iVar3 = _ChromeWebAppShortcutCopierMain(), iVar3 == 0)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x42bdc40);
      (*pcVar2)();
    }
    lVar4 = *(long *)(DAT_0e49b470 + 0x460);
    if ((lVar4 != 0) &&
       (uVar1 = *(uint *)(lVar4 + 8), *(uint *)(lVar4 + 8) = uVar1 + 1, 0xfffffffe < uVar1)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x42bdc18);
      (*pcVar2)();
    }
    uVar5 = FUN_00008790(0x48);
    lVar4 = FUN_0434f080(uVar5,lVar4,&DAT_0c9d66c0);
    lVar6 = *(long *)(param_2 + 0x448);
    *(long *)(param_2 + 0x448) = lVar4;
    if (lVar6 != 0) {
      uVar5 = FUN_0434f19c(lVar6);
      FUN_00005810(uVar5,0x48);
      lVar4 = *(long *)(param_2 + 0x448);
    }
  }
  FUN_0434f58c(param_1,lVar4,param_3,param_4);
  return;
}


