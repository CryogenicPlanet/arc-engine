// ==== 0x167ca68  FUN_0167ca68  (324 bytes)

void FUN_0167ca68(long param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  code *pcVar1;
  long lVar2;
  undefined8 uVar3;
  long *plVar4;
  undefined8 local_88 [2];
  undefined7 local_78;
  char cStack_71;
  byte local_70;
  undefined1 auStack_68 [32];
  undefined1 auStack_48 [8];
  
  lVar2 = FUN_005b4234();
  if (lVar2 == 0) {
    FUN_0837a430(auStack_68,"../../components/prefs/pref_service.cc",0x23c);
    FUN_08376274(auStack_48,auStack_68);
    uVar3 = FUN_07454274(auStack_48);
    FUN_00040060(uVar3,"Trying to write an unregistered pref: ",0x26);
    uVar3 = FUN_07454274(auStack_48);
    FUN_00040060(uVar3,param_2,param_3);
    FUN_083762f0(auStack_48);
  }
  else {
    if (*(char *)(lVar2 + 0x18) != *(char *)(param_4 + 0x18)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x167cb54);
      (*pcVar1)();
    }
    plVar4 = *(long **)(param_1 + 0x18);
    FUN_01891db8(local_88,param_4);
    (**(code **)(*plVar4 + 0x40))(plVar4,param_2,param_3,local_88,*(uint *)(lVar2 + 0x1c) >> 7 & 2);
    if ((local_70 != 0xff) && ((local_70 & 0xfd) != 0)) {
      if (local_70 == 4) {
        if (cStack_71 < '\0') {
          FUN_00005810(local_88[0],CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff);
        }
      }
      else {
        (*(code *)(&PTR__cxx_destruct_0df6d258)[(uint)local_70])(auStack_68,local_88);
      }
    }
  }
  return;
}


