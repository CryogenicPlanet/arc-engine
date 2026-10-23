// ==== 0x440c34c  ArcCpuInfo::init  (544 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCpuInfo::init(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  int iVar2;
  ID IVar3;
  undefined8 uVar4;
  long lVar5;
  undefined1 *local_50;
  long lStack_48;
  undefined7 local_40;
  char cStack_39;
  undefined1 local_31;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCpuInfo;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    local_31 = 0;
    FUN_04341e44(&local_50,&local_31);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    lVar5 = lStack_48;
    if (-1 < cStack_39) {
      lVar5 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lVar5);
    *(undefined8 *)(IVar3 + 0x20) = uVar4;
    if (cStack_39 < '\0') {
      FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    FUN_04341ef0(&local_50,&local_31);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    lVar5 = lStack_48;
    if (-1 < cStack_39) {
      lVar5 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lVar5);
    *(undefined8 *)(IVar3 + 0x28) = uVar4;
    uVar4 = 0;
    if (cStack_39 < '\0') {
      uVar4 = FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    FUN_04572478(&local_50,uVar4);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    if (-1 < cStack_39) {
      lStack_48 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lStack_48);
    *(undefined8 *)(IVar3 + 0x30) = uVar4;
    uVar4 = 0;
    if (cStack_39 < '\0') {
      uVar4 = FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    lVar5 = FUN_083a7e78(uVar4);
    *(long *)(IVar3 + 0x38) = (long)*(int *)(lVar5 + 4);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x40) = (long)*(int *)(lVar5 + 8);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x48) = (long)*(int *)(lVar5 + 0xc);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x50) = (long)*(int *)(lVar5 + 0x10);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x58) = (long)*(int *)(lVar5 + 0x14);
    lVar5 = FUN_083a7e78();
    iVar2 = *(int *)(lVar5 + 0x18);
    *(undefined1 *)(IVar3 + 8) = 0;
    *(long *)(IVar3 + 0x60) = (long)iVar2;
    *(undefined8 *)(IVar3 + 0x68) = 0;
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 9) = *(undefined1 *)(lVar5 + 0x20);
    if (((DAT_0e4a2f30 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e4a2f30), iVar2 != 0)) {
      DAT_0e4a2f28 = 0;
      thunk_FUN_0c44b760(&DAT_0e4a2f30);
    }
    *(undefined1 *)(IVar3 + 10) = DAT_0e4a2f28;
    *(undefined8 *)(IVar3 + 0xb) = 0;
    *(undefined4 *)(IVar3 + 0x12) = 0;
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x16) = *(undefined1 *)(lVar5 + 0x1c);
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x17) = *(undefined1 *)(lVar5 + 0x1d);
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x18) = *(undefined1 *)(lVar5 + 0x1e);
  }
  return IVar3;
}


