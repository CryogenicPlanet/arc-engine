// ==== 0x4402988  ArcDevToolsConnectionDialog::disable  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcDevToolsConnectionDialog::disable(ID param_1,SEL param_2)

{
  if ((*(byte *)(*(long *)(param_1 + 0x60) + 0x10) & 1) != 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x044029a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_1 + 0x58))(param_1 + 0x48);
  return;
}


