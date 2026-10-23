// ==== 0x43e8b58  ArcBrowserApplication::deleteBrowserContext:error:  (224 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcBrowserApplication::deleteBrowserContext_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  uint uVar1;
  bool bVar2;
  uint *puVar3;
  undefined8 *puVar4;
  uint *local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  puVar4 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
  }
  FUN_042b6394(&local_38,*puVar4,&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  local_58 = local_38;
  puVar3 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    uVar1 = *local_38;
    *local_38 = uVar1 + 1;
    puVar3 = (uint *)(ulong)uVar1;
  }
  bVar2 = FUN_04402548(puVar3,&local_58,param_4);
  if (((ulong)local_58 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  return bVar2;
}


