// ==== 0x4402950  ArcDevToolsConnectionDialog::accept  (28 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcDevToolsConnectionDialog::accept(int param_1,sockaddr *param_2,socklen_t *param_3)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = (ulong)(uint)param_1;
  if ((*(byte *)(*(long *)(uVar2 + 0x20) + 0x10) & 1) != 0) {
    return param_1;
  }
                    /* WARNING: Could not recover jumptable at 0x04402968. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = (**(code **)(uVar2 + 0x18))(uVar2 + 8);
  return iVar1;
}


