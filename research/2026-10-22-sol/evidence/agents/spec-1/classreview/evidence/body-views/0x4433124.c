// ==== 0x4433124  ArcCommandBarRankingExample::init:candidates:selectedIndex:  (908 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x24 bytes */

ID ArcCommandBarRankingExample::init_candidates_selectedIndex_
             (ID param_1,SEL param_2,ID param_3,ID param_4,int param_5)

{
  long *plVar1;
  code *pcVar2;
  ID IVar3;
  ID IVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long local_1f8;
  long local_1f0;
  long local_1e8;
  undefined8 local_150;
  long lStack_148;
  long *local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  ID local_108;
  class_t *pcStack_100;
  undefined1 auStack_f8 [128];
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_100 = &objc::class_t::ArcCommandBarRankingExample;
  local_108 = param_1;
  IVar3 = NSObject::init((ID)&local_108,"init");
  if (IVar3 == 0) {
LAB_0443343c:
    if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
      ___stack_chk_fail(0);
    }
    return IVar3;
  }
  plVar1 = (long *)(IVar3 + 8);
  if (*(char *)(IVar3 + 0x40) != '\0') {
    lVar8 = *(long *)(IVar3 + 0x20);
    if (lVar8 != 0) {
      lVar5 = *(long *)(IVar3 + 0x28) * 0xa8;
      lVar9 = lVar8;
      if (lVar5 != 0) {
        lVar5 = lVar5 + lVar8;
        lVar9 = *(long *)(IVar3 + 0x28) * -0xa8;
        do {
          if (lVar5 == 0xa8) goto LAB_04433488;
          lVar5 = FUN_042a3ff4();
          lVar9 = lVar9 + 0xa8;
        } while (lVar9 != 0);
        lVar9 = *(long *)(IVar3 + 0x20);
      }
      *(long *)(IVar3 + 0x28) = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
      FUN_00005810(lVar9,*(long *)(IVar3 + 0x30) * 0xa8);
    }
    lVar8 = *plVar1;
    if (lVar8 != 0) {
      lVar5 = *(long *)(IVar3 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar9 = *(long *)(IVar3 + 0x10) * -4;
        lVar5 = lVar5 + lVar8;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04433488;
          lVar9 = lVar9 + 4;
        } while (lVar9 != 0);
      }
      *(undefined8 *)(IVar3 + 0x10) = 0;
      FUN_00005810(lVar8,*(long *)(IVar3 + 0x18) << 2);
    }
  }
  *(undefined8 *)(IVar3 + 0x38) = 0;
  *(undefined8 *)(IVar3 + 0x20) = 0;
  *(undefined8 *)(IVar3 + 0x18) = 0;
  *(undefined8 *)(IVar3 + 0x30) = 0;
  *(undefined8 *)(IVar3 + 0x28) = 0;
  *(undefined8 *)(IVar3 + 0x10) = 0;
  *plVar1 = 0;
  *(undefined1 *)(IVar3 + 0x40) = 1;
  FUN_04432fcc(&local_1f8,param_3);
  if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
    if (plVar1 != &local_1f8) {
      FUN_015cdd2c(IVar3 + 8,local_1f8,local_1f8 + local_1f0 * 4,(local_1f0 << 2) >> 2);
    }
    if (local_1f8 != 0) {
      if (local_1f0 * 4 != 0) {
        lVar8 = local_1f0 * -4;
        lVar5 = local_1f0 * 4 + local_1f8;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04433488;
          lVar8 = lVar8 + 4;
        } while (lVar8 != 0);
      }
      local_1f0 = 0;
      FUN_00005810(local_1f8,local_1e8 << 2);
    }
    if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
      IVar4 = _objc_msgSend(param_4,"count");
      FUN_043d9858(IVar3 + 0x20,IVar4);
      uStack_128 = 0;
      local_130 = 0;
      uStack_118 = 0;
      uStack_120 = 0;
      lStack_148 = 0;
      local_150 = 0;
      uStack_138 = 0;
      local_140 = (long *)0x0;
      IVar4 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_150,
                            auStack_f8,0x10);
      if (IVar4 == 0) {
LAB_04433424:
        if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
          *(int *)(IVar3 + 0x38) = param_5;
          goto LAB_0443343c;
        }
      }
      else {
        uVar7 = 0;
        lVar5 = *local_140;
        lVar8 = lVar5;
        while( true ) {
          if (lVar8 != lVar5) {
            _objc_enumerationMutation(param_4);
          }
          if ((*(byte *)(IVar3 + 0x40) & 1) == 0) break;
          FUN_04434c34(&local_1f8,*(undefined8 *)(lStack_148 + uVar7 * 8));
          if (*(long *)(IVar3 + 0x28) == *(long *)(IVar3 + 0x30)) {
            lVar8 = FUN_043da9d0(IVar3 + 0x20,&local_1f8);
            lVar9 = *(long *)(IVar3 + 0x20);
            lVar6 = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
          }
          else {
            lVar8 = *(long *)(IVar3 + 0x20) + *(long *)(IVar3 + 0x28) * 0xa8;
            if (lVar8 == 0) break;
            FUN_042a3ea8(lVar8,&local_1f8);
            lVar8 = lVar8 + 0xa8;
            lVar9 = *(long *)(IVar3 + 0x20);
            lVar6 = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
            *(long *)(IVar3 + 0x28) = lVar6;
          }
          *(long *)(IVar3 + 0x28) = lVar6;
          if (lVar8 == lVar9) break;
          FUN_042a3ff4(&local_1f8);
          uVar7 = uVar7 + 1;
          if (IVar4 <= uVar7) {
            IVar4 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_150,
                                  auStack_f8,0x10);
            if (IVar4 == 0) goto LAB_04433424;
            uVar7 = 0;
          }
          lVar8 = *local_140;
        }
      }
    }
  }
LAB_04433488:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443348c);
  (*pcVar2)();
}


