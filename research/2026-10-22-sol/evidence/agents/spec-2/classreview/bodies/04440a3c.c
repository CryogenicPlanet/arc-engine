// ==== 0x4440a3c  ArcMenuModel::menuWillShow  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMenuModel::menuWillShow(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
                    /* WARNING: Could not recover jumptable at 0x04440a54. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(long **)(*(long *)(param_1 + 0x10) + 0x28) + 0x100))();
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4440a5c);
  (*pcVar1)();
}


