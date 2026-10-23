// ==== 0x43386a0  FUN_043386a0  (88 bytes)

undefined8 * FUN_043386a0(undefined8 *param_1)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  *param_1 = &PTR_FUN_0da7eed0;
  puVar2 = (undefined8 *)FUN_00008790(0x20);
  puVar2[1] = 0;
  puVar2[2] = 0;
  *puVar2 = &PTR_viewWillStartLiveResize_0da7ef60;
  if (puVar2 + 3 != (undefined8 *)0x0) {
    puVar2[3] = 0;
    param_1[1] = puVar2 + 3;
    param_1[2] = puVar2;
    return param_1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43386f8);
  (*pcVar1)();
}


