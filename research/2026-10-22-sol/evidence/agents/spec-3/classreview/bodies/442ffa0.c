// ==== 0x442ffa0  ArcCookie::init  (420 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCookie::init(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  ID IVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  long *plVar6;
  ID local_170;
  class_t *pcStack_168;
  undefined **local_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined2 local_108;
  undefined4 local_104;
  undefined1 local_100;
  undefined1 local_90;
  undefined8 local_88;
  undefined1 local_80;
  undefined1 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  pcStack_168 = &objc::class_t::ArcCookie;
  local_170 = param_1;
  IVar3 = NSObject::init((ID)&local_170,"init");
  if (IVar3 == 0) {
    return 0;
  }
  if (*(char *)(IVar3 + 0x18) == '\0') goto LAB_0442fffc;
  plVar6 = *(long **)(IVar3 + 0x10);
  if (plVar6 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar6[1];
    plVar6[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar6 + 0x10))(plVar6);
      if (plVar6[2] != 0) {
        LOAcquire();
        lVar2 = plVar6[2];
        plVar6[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_0442fff8;
      }
      pcVar1 = *(code **)(*plVar6 + 0x20);
      if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
        FUN_00005810(plVar6,0x20);
      }
      else {
        (*pcVar1)(plVar6);
      }
    }
  }
LAB_0442fff8:
  *(undefined1 *)(IVar3 + 0x18) = 0;
LAB_0442fffc:
  local_104 = 0;
  local_100 = 0;
  local_90 = 0;
  uStack_150 = 0;
  local_158 = 0;
  uStack_140 = 0;
  local_148 = 0;
  uStack_130 = 0;
  local_138 = 0;
  uStack_120 = 0;
  local_128 = 0;
  uStack_110 = 0;
  local_118 = 0;
  local_108 = 0;
  local_88 = 0xffffffff00000000;
  local_160 = &PTR_FUN_0daa6758;
  local_80 = 0;
  local_58 = 0;
  uStack_40 = 0;
  local_48 = 0;
  local_50 = 0;
  local_38 = 0x300000001;
  puVar4 = (undefined8 *)FUN_00008790(0x90);
  puVar4[1] = 0;
  puVar4[2] = 0;
  *puVar4 = &PTR_viewWillStartLiveResize_0da7e888;
  if (puVar4 + 3 != (undefined8 *)0x0) {
    uVar5 = FUN_043345f8(puVar4 + 3,&local_160);
    *(undefined8 *)(IVar3 + 8) = uVar5;
    *(undefined8 **)(IVar3 + 0x10) = puVar4;
    FUN_016a2f44(&local_160);
    *(undefined1 *)(IVar3 + 0x18) = 1;
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430144);
  (*pcVar1)();
}


