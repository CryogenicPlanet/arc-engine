// ==== 0x4405130  ArcImage::.cxx_construct  (88 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcImage::_cxx_construct(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  *(undefined ***)(param_1 + 8) = &PTR_FUN_0da7eed0;
  puVar2 = (undefined8 *)FUN_00008790(0x20);
  puVar2[1] = 0;
  puVar2[2] = 0;
  *puVar2 = &PTR_viewWillStartLiveResize_0da7ef60;
  if (puVar2 + 3 != (undefined8 *)0x0) {
    puVar2[3] = 0;
    *(undefined8 **)(param_1 + 0x10) = puVar2 + 3;
    *(undefined8 **)(param_1 + 0x18) = puVar2;
    return param_1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4405188);
  (*pcVar1)();
}


