// ==== 0x40bf0c8  FUN_040bf0c8  (128 bytes)

void FUN_040bf0c8(long param_1,long param_2)

{
  long lVar1;
  long *plVar2;
  undefined8 uVar3;
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [32];
  
  uVar3 = *(undefined8 *)(param_1 + 0x28);
  *(undefined8 *)(param_1 + 0x28) = 0;
  lVar1 = *(long *)(param_2 + 0x80);
  if (lVar1 == 0) {
    FUN_0837a430(auStack_40,"../../content/browser/gpu/gpu_process_host.cc",0x61d);
    FUN_08375df4(auStack_48,"gpu_host_",0x9f,auStack_40);
    FUN_0837612c(auStack_48);
    lVar1 = *(long *)(param_2 + 0x80);
  }
  plVar2 = (long *)FUN_0214df40(lVar1);
                    /* WARNING: Could not recover jumptable at 0x040bf104. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*plVar2 + 0xd8))(plVar2,uVar3);
  return;
}


