// ==== 0x786cd50  FUN_0786cd50  (336 bytes)

long FUN_0786cd50(long param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  long lVar4;
  undefined1 auStack_1f0 [32];
  undefined1 local_1d0;
  undefined1 auStack_1c8 [8];
  undefined8 local_1c0;
  undefined1 auStack_1b8 [32];
  undefined1 auStack_198 [336];
  undefined8 local_48;
  
  local_48 = 0;
  puVar2 = (undefined8 *)FUN_07849f00();
  *puVar2 = &PTR_FUN_0d9a2988;
  puVar2[2] = &PTR_LAB_0d9a2a10;
  *(undefined1 *)(puVar2 + 0x2d) = 1;
  FUN_00029e40(auStack_1b8,"WebstoreInstallWithPrompt",
               "../../chrome/browser/extensions/webstore_install_with_prompt.cc",0x1b);
  FUN_02728fa4(auStack_198,param_3,auStack_1b8);
  uVar3 = FUN_02dba6c0(auStack_198);
  *(undefined8 *)(param_1 + 0x170) = uVar3;
  thunk_FUN_026f6980(auStack_198);
  FUN_03b5f430(param_1 + 0x178,param_4);
  *(undefined8 *)(param_1 + 0x180) = *(undefined8 *)(param_4 + 8);
  *(undefined8 *)(param_1 + 0x188) = 0;
  iVar1 = FUN_044559d4(param_1 + 0x178);
  if (iVar1 != 0) {
    FUN_03b5f430(auStack_1c8,param_4);
    local_1c0 = *(undefined8 *)(param_4 + 8);
    uVar3 = FUN_0c1fe670(auStack_1c8);
    lVar4 = *(long *)(param_1 + 0x188);
    *(undefined8 *)(param_1 + 0x188) = uVar3;
    if (lVar4 != 0) {
      FUN_0c1fe610();
    }
    FUN_01b23e1c(auStack_1c8);
  }
  lVar4 = *(long *)(param_1 + 0x170);
  FUN_00029e40(auStack_198,"WebstoreInstallWithPrompt",
               "../../chrome/browser/extensions/webstore_install_with_prompt.cc",0x1f);
  FUN_0135f2ac(auStack_1f0,auStack_198);
  local_1d0 = 1;
  FUN_067ada24(s__System_Library_Frameworks_Secur_000011a8 + lVar4 + 0x28,auStack_1f0);
  *(undefined4 *)(param_1 + 0x60) = 2;
  FUN_00d5ff9c(&local_48);
  return param_1;
}


