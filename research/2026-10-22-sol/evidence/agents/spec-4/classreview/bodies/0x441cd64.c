// ==== 0x441cd64  ArcWebDialog::initWithWebDialog:  (408 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcWebDialog::initWithWebDialog_(ID param_1,SEL param_2,const_void__ param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  byte bVar3;
  const_void__ pvVar4;
  undefined8 *puVar5;
  long lVar6;
  ID IVar7;
  long *plVar8;
  code *pcVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcWebDialog;
  local_30 = param_1;
  IVar7 = NSObject::init((ID)&local_30,"init");
  if (IVar7 != 0) {
    if ((undefined8 *)(IVar7 + 8) != param_3) {
      bVar3 = *(byte *)((long)param_3 + 0x17);
      if (*(char *)(IVar7 + 0x1f) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 8);
        pvVar4 = *(const_void__ *)param_3;
        if (-1 < (char)bVar3) {
          uVar2 = (ulong)bVar3;
          pvVar4 = param_3;
        }
        FUN_007ab764(IVar7 + 8,pvVar4,uVar2);
      }
      else if ((char)bVar3 < '\0') {
        FUN_007ab624(IVar7 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
      }
      else {
        uVar11 = *(undefined8 *)((long)param_3 + 8);
        uVar10 = *(undefined8 *)param_3;
        *(undefined8 *)(IVar7 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
        *(undefined8 *)(IVar7 + 0x10) = uVar11;
        *(undefined8 *)(IVar7 + 8) = uVar10;
      }
    }
    uVar10 = *(undefined8 *)((long)param_3 + 0x18);
    *(undefined8 *)(IVar7 + 0x28) = *(undefined8 *)((long)param_3 + 0x20);
    *(undefined8 *)(IVar7 + 0x20) = uVar10;
    puVar1 = (undefined8 *)((long)param_3 + 0x28);
    if ((undefined8 *)(IVar7 + 0x30) != puVar1) {
      bVar3 = *(byte *)((long)param_3 + 0x3f);
      if (*(char *)(IVar7 + 0x47) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 0x30);
        puVar5 = *(undefined8 **)((long)param_3 + 0x28);
        if (-1 < (char)bVar3) {
          uVar2 = (ulong)bVar3;
          puVar5 = puVar1;
        }
        FUN_007ab764(IVar7 + 0x30,puVar5,uVar2);
      }
      else if ((char)bVar3 < '\0') {
        FUN_007ab624(IVar7 + 0x30,*(undefined8 *)((long)param_3 + 0x28),
                     *(undefined8 *)((long)param_3 + 0x30));
      }
      else {
        uVar11 = *(undefined8 *)((long)param_3 + 0x30);
        uVar10 = *puVar1;
        *(undefined8 *)(IVar7 + 0x40) = *(undefined8 *)((long)param_3 + 0x38);
        *(undefined8 *)(IVar7 + 0x38) = uVar11;
        *(undefined8 *)(IVar7 + 0x30) = uVar10;
      }
    }
    uVar11 = *(undefined8 *)((long)param_3 + 0x48);
    uVar10 = *(undefined8 *)((long)param_3 + 0x40);
    if (*(long *)((long)param_3 + 0x48) != 0) {
      plVar8 = (long *)(*(long *)((long)param_3 + 0x48) + 0x10);
      *plVar8 = *plVar8 + 1;
    }
    plVar8 = *(long **)(IVar7 + 0x50);
    *(undefined8 *)(IVar7 + 0x50) = uVar11;
    *(undefined8 *)(IVar7 + 0x48) = uVar10;
    if (plVar8 != (long *)0x0) {
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar6 = plVar8[2];
        plVar8[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) {
          return IVar7;
        }
      }
      pcVar9 = *(code **)(*plVar8 + 0x20);
      if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar9)();
      }
    }
  }
  return IVar7;
}


