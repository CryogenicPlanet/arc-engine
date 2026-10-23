// ==== 0x437a510  FUN_0437a510  (352 bytes)

void FUN_0437a510(undefined8 *param_1,long param_2)

{
  undefined1 *puVar1;
  undefined1 uVar2;
  code *pcVar3;
  undefined8 uVar4;
  long lVar5;
  undefined8 local_60;
  char *local_58;
  
  local_60 = 10;
  if (DAT_0e4a445a != '\0') {
    local_58 = "WebContents::Impl::ActivateTab";
    FUN_0000a280(DAT_0e4a445a,&local_60,&local_58,1);
  }
  lVar5 = *(long *)(param_2 + 0x130);
  if (lVar5 == 0) {
    uVar4 = FUN_04b37104(0x39,"The object is detached",0x16,0x92d,
                         "../../arc/browser/web_contents/web_contents_impl.cc");
    *param_1 = uVar4;
  }
  else {
    puVar1 = (undefined1 *)(param_2 + 0x170);
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_2 + 0x170);
    }
    uVar2 = *(undefined1 *)(param_2 + 0x170);
    *(undefined1 *)(param_2 + 0x170) = 1;
    if (((*(long *)(param_2 + 0x58) != 0) && (*(char *)(*(long *)(param_2 + 0x58) + 4) == '\0')) &&
       (*(long *)(param_2 + 0x60) != 0)) {
      if ((*(long *)(param_2 + 0x58) == 0) || (*(char *)(*(long *)(param_2 + 0x58) + 4) != '\0')) {
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(0,0x437a670);
        (*pcVar3)();
      }
      (**(code **)(**(long **)(param_2 + 0x60) + 0x28))(*(long **)(param_2 + 0x60),lVar5);
    }
    if (puVar1 != (undefined1 *)0x0) {
      *puVar1 = uVar2;
    }
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(puVar1);
    }
    *param_1 = 1;
  }
  local_60 = 10;
  if (DAT_0e4a445a != '\0') {
    local_58 = (char *)0x0;
    FUN_0000a280(DAT_0e4a445a,&local_60,&local_58,2);
  }
  return;
}


