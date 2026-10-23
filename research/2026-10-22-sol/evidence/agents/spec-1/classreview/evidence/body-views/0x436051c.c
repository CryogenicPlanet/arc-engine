// ==== 0x436051c  FUN_0436051c  (348 bytes)

void FUN_0436051c(undefined8 *param_1,long param_2)

{
  code *pcVar1;
  ulong uVar2;
  char *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  long lVar6;
  
  if (((*(long *)(param_2 + 0x30) == 0) || (*(char *)(*(long *)(param_2 + 0x30) + 4) != '\0')) ||
     (*(long *)(param_2 + 0x38) == 0)) {
    pcVar3 = "managed webcontents is null";
    uVar4 = 0x1b;
    uVar5 = 0xd3;
  }
  else {
    if ((*(long *)(param_2 + 0x30) == 0) || (*(char *)(*(long *)(param_2 + 0x30) + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x4360678);
      (*pcVar1)();
    }
    if (*(char *)(*(long *)(param_2 + 0x38) + 0xf90) == '\0') {
      lVar6 = 0;
      if (*(long *)(param_2 + 0x30) != 0) {
        if (*(char *)(*(long *)(param_2 + 0x30) + 4) == '\0') {
          lVar6 = *(long *)(param_2 + 0x38);
        }
        else {
          lVar6 = 0;
        }
      }
      lVar6 = FUN_000805e0(lVar6 + 8,&DAT_0c767030);
      if ((lVar6 == 0) || (lVar6 + -0x28 == 0)) {
        pcVar3 = "no active media sessions";
        uVar4 = 0x18;
        uVar5 = 0xde;
      }
      else {
        uVar2 = FUN_06551f0c(lVar6 + -0x28,10);
        if ((uVar2 & 1) != 0) {
LAB_04360644:
          FUN_065534f8(lVar6 + -0x28);
          uVar4 = 1;
          goto LAB_04360568;
        }
        if (*(long *)(lVar6 + 0x58) == 1) {
          uVar2 = FUN_02dda8c0(*(undefined8 *)(*(long *)(lVar6 + 0x48) + 0x20),
                               *(undefined4 *)(*(long *)(lVar6 + 0x48) + 0x28));
          if ((uVar2 & 1) != 0) goto LAB_04360644;
        }
        pcVar3 = "media session does not support picture in picture";
        uVar4 = 0x31;
        uVar5 = 0xe4;
      }
    }
    else {
      pcVar3 = "webcontents has active picture in picture video";
      uVar4 = 0x2f;
      uVar5 = 0xd8;
    }
  }
  uVar4 = FUN_04b37104(0x25,pcVar3,uVar4,uVar5,
                       "../../arc/browser/web_contents/media_session_impl.cc");
LAB_04360568:
  *param_1 = uVar4;
  return;
}


