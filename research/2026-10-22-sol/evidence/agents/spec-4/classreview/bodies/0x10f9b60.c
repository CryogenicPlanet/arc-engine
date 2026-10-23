// ==== 0x10f9b60  FUN_010f9b60  (244 bytes)

undefined8 FUN_010f9b60(long *param_1)

{
  undefined8 uVar1;
  long lVar2;
  undefined1 auStack_38 [32];
  undefined1 auStack_18 [8];
  
  if (*(code **)(*param_1 + 0xb0) == FUN_034fca2c) {
    if (param_1 != (long *)0x0) {
      do {
        if ((long *)param_1[0x2f] == (long *)0x0) {
          lVar2 = *(long *)(param_1[0x22] + 0x38);
          if (lVar2 == 0) goto LAB_010f9b9c;
          break;
        }
        lVar2 = param_1[0xea];
        param_1 = (long *)param_1[0x2f];
      } while (lVar2 == 0);
      goto LAB_010f9bc8;
    }
LAB_010f9b9c:
    FUN_0837a430(auStack_38,"../../content/browser/renderer_host/render_frame_host_impl.cc",
                 "rsions/A/CoreMedia");
    FUN_08376274(auStack_18,auStack_38);
    FUN_083762f0(auStack_18);
  }
  else {
    if (param_1 != (long *)0x0) {
      do {
        if ((long *)param_1[0x2f] == (long *)0x0) {
          lVar2 = *(long *)(param_1[0x22] + 0x38);
          if (lVar2 == 0) goto LAB_010f9c28;
          break;
        }
        lVar2 = param_1[0xea];
        param_1 = (long *)param_1[0x2f];
      } while (lVar2 == 0);
      goto LAB_010f9bc8;
    }
LAB_010f9c28:
    FUN_0837a430(auStack_38,"../../content/browser/renderer_host/render_frame_host_impl.cc",
                 "rsions/A/CoreMedia");
    FUN_08376274(auStack_18,auStack_38);
    FUN_083762f0(auStack_18);
  }
  lVar2 = 0;
LAB_010f9bc8:
  if ((*(long *)(lVar2 + 0x348) == 0) || (*(char *)(*(long *)(lVar2 + 0x348) + 4) != '\0')) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined8 *)(lVar2 + 0x350);
  }
  return uVar1;
}


