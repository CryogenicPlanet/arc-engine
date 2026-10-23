// ==== 0x4360b58  FUN_04360b58  (196 bytes)

void FUN_04360b58(undefined8 *param_1,long param_2)

{
  char *pcVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  
  if (((*(long *)(param_2 + 0x30) == 0) || (*(char *)(*(long *)(param_2 + 0x30) + 4) != '\0')) ||
     (*(long *)(param_2 + 0x38) == 0)) {
    pcVar1 = "managed webcontents is null";
    uVar2 = 0x1b;
    uVar3 = 0x14b;
  }
  else {
    lVar4 = 0;
    if (*(long *)(param_2 + 0x30) != 0) {
      if (*(char *)(*(long *)(param_2 + 0x30) + 4) == '\0') {
        lVar4 = *(long *)(param_2 + 0x38);
      }
      else {
        lVar4 = 0;
      }
    }
    lVar4 = FUN_000805e0(lVar4 + 8,&DAT_0c767030);
    if ((lVar4 != 0) && (lVar4 + -0x28 != 0)) {
      uVar2 = 1;
      FUN_06551e40(lVar4 + -0x28,1);
      goto LAB_04360ba4;
    }
    pcVar1 = "no active media sessions";
    uVar2 = 0x18;
    uVar3 = 0x150;
  }
  uVar2 = FUN_04b37104(0x25,pcVar1,uVar2,uVar3,
                       "../../arc/browser/web_contents/media_session_impl.cc");
LAB_04360ba4:
  *param_1 = uVar2;
  return;
}


