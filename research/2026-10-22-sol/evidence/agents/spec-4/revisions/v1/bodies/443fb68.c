// ==== 0x443fb68  ArcMenuItemModel::label  (136 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcMenuItemModel::label(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  long *plVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x48) & 1) != 0) {
    plVar2 = *(long **)(*(long *)(param_1 + 0x38) + 0x10);
    (**(code **)(*plVar2 + 0x38))
              (&local_38,plVar2,*(undefined8 *)(*(long *)(param_1 + 0x38) + 0x18));
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar4 = FUN_01d9fb90(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) << 1);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x443fbf0);
  (*pcVar3)();
}


