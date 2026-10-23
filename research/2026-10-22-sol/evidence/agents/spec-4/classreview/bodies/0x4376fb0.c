// ==== 0x4376fb0  FUN_04376fb0  (268 bytes)

void FUN_04376fb0(undefined8 *param_1,long param_2,undefined8 *param_3)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  char *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  
  puVar6 = (undefined8 *)param_3[3];
  if (*(char *)(puVar6 + 2) == '\0') {
    if (*(long *)(param_2 + 0x130) != 0) {
      puVar1 = (undefined4 *)FUN_00008790(0x48);
      *puVar1 = 1;
      *(undefined1 **)(puVar1 + 2) = &LAB_041b7dc0;
      *(code **)(puVar1 + 4) = FUN_042a34c0;
      *(code **)(puVar1 + 6) = FUN_00b77080;
      *(undefined1 **)(puVar1 + 8) = &LAB_042d62c0;
      uVar2 = *param_3;
      *(undefined8 *)(puVar1 + 0xc) = param_3[1];
      *(undefined8 *)(puVar1 + 10) = uVar2;
      *(undefined8 *)(puVar1 + 0xe) = param_3[2];
      *(undefined8 **)(puVar1 + 0x10) = puVar6;
      if ((code *)*puVar6 != (code *)0x0) {
        uVar2 = (*(code *)*puVar6)(*param_3);
        *(undefined8 *)(puVar1 + 10) = uVar2;
      }
      FUN_0435da28(param_1,param_2 + 0x488,puVar1);
      return;
    }
    pcVar3 = "managed webcontents is null";
    uVar2 = 0x25;
    uVar4 = 0x1b;
    uVar5 = 0x5ea;
  }
  else {
    pcVar3 = "callback is null";
    uVar2 = 0xd;
    uVar4 = 0x10;
    uVar5 = 0x5e6;
  }
  uVar2 = FUN_04b37104(uVar2,pcVar3,uVar4,uVar5,
                       "../../arc/browser/web_contents/web_contents_impl.cc");
  *param_1 = uVar2;
  return;
}


