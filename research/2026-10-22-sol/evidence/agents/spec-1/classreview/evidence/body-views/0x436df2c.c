// ==== 0x436df2c  FUN_0436df2c  (200 bytes)

void FUN_0436df2c(undefined8 *param_1,long param_2)

{
  long lVar1;
  undefined8 uVar2;
  char *pcVar3;
  undefined8 uVar4;
  
  if (*(long *)(param_2 + 0x18) == 0) {
    pcVar3 = "managed webcontents is null";
    uVar2 = 0x1b;
    uVar4 = 0x3b;
LAB_0436dfcc:
    uVar2 = FUN_04b37104(0x25,pcVar3,uVar2,uVar4,
                         "../../arc/browser/web_contents/thumbnails_generator_impl.cc");
  }
  else {
    if ((*(long *)(param_2 + 0x70) != 0) && (*(long *)(param_2 + 0x40) != 0)) {
      lVar1 = FUN_018efea8();
      if ((lVar1 == 0) ||
         (lVar1 = FUN_004969fc(lVar1 + 0x310,&PTR_s_ThumbnailTabHelper_kDataKeyImpl_0df14bf0),
         lVar1 == 0)) {
        pcVar3 = "thumbnail helper is unavailable";
        uVar2 = 0x1f;
        uVar4 = 0x47;
        goto LAB_0436dfcc;
      }
      if (*(int *)(*(long *)(lVar1 + 0x18) + 0xae0) == 2) {
        FUN_0731d500();
      }
      else if (*(long *)(lVar1 + 0x68) != 0) {
        FUN_0731c7a8();
      }
    }
    uVar2 = 1;
  }
  *param_1 = uVar2;
  return;
}


