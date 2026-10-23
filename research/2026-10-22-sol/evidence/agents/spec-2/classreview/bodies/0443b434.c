// ==== 0x443b434  ArcConfirmInfobar::accept  (40 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcConfirmInfobar::accept(int param_1,sockaddr *param_2,socklen_t *param_3)

{
  code *pcVar1;
  int iVar2;
  long *plVar3;
  
  if ((*(byte *)((ulong)(uint)param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443b45c);
    (*pcVar1)();
  }
  plVar3 = (long *)**(long **)((ulong)(uint)param_1 + 0x10);
  if (plVar3 != (long *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0443b450. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar2 = (**(code **)(*plVar3 + 0x108))();
    return iVar2;
  }
  return 0;
}


