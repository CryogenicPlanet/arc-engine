// ==== 0x4044a1c  objc::Categories::ArcBrowserContextHistory_Extensions_::deleteByOriginatorVisitIDs:originatorCacheGuid:startTime:endTime:completion:  (856 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Type propagation algorithm not settling */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::
     deleteByOriginatorVisitIDs_originatorCacheGuid_startTime_endTime_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
               undefined4 param_8)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  long lVar4;
  undefined8 extraout_d0;
  undefined8 extraout_d0_00;
  ID local_1b8 [3];
  long *local_1a0;
  undefined8 uStack_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 uStack_180;
  undefined8 uStack_178;
  long local_170;
  ID local_168;
  ID local_160;
  long *local_158;
  ID *pIStack_150;
  long *local_148;
  ID *local_140;
  long *plStack_138;
  long *local_130;
  long local_128;
  ID *local_120 [2];
  undefined1 *local_110;
  undefined **local_108;
  undefined1 auStack_100 [128];
  long local_80;
  
  local_80 = *(long *)PTR____stack_chk_guard_0d824020;
  local_170 = 0;
  local_168 = 0;
  local_160 = 0;
  IVar2 = _objc_msgSend(param_3,"count");
  if (IVar2 != 0) {
    if (IVar2 >> 0x3d != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_170 = FUN_00008790(IVar2 << 3);
    local_168 = 0;
    local_160 = IVar2;
  }
  uStack_188 = 0;
  local_190 = 0;
  uStack_178 = 0;
  uStack_180 = 0;
  local_1b8[2] = 0;
  local_1b8[1] = 0;
  uStack_198 = 0;
  local_1a0 = (long *)0x0;
  IVar2 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",local_1b8 + 1,
                        auStack_100,0x10);
  if (IVar2 != 0) {
    lVar4 = *local_1a0;
    do {
      IVar3 = 0;
      do {
        if (*local_1a0 != lVar4) {
          _objc_enumerationMutation(param_3);
        }
        local_1b8[0] = _objc_msgSend(*(ID *)(local_1b8[2] + IVar3 * 8),"longLongValue");
        local_128 = local_170 + local_168 * 8;
        local_158 = &local_128;
        pIStack_150 = local_1b8;
        local_148 = &local_170;
        local_140 = local_1b8;
        plStack_138 = &local_170;
        local_130 = &local_128;
        FUN_01ab9b20(local_168 != local_160,&local_140,&local_158);
        local_168 = local_128 - local_170 >> 3;
        if (local_128 - local_170 == 0) goto LAB_04044d68;
        IVar3 = IVar3 + 1;
      } while (IVar2 != IVar3);
      IVar2 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",local_1b8 + 1,
                            auStack_100,0x10);
    } while (IVar2 != 0);
  }
  local_140 = (ID *)0x0;
  plStack_138 = (long *)0x0;
  local_130 = (long *)0x0;
  FUN_03b60ee8(&local_140,param_4);
  _objc_msgSend(param_1,"browser_context_history",&local_158);
  if (((ulong)local_148 & 1) != 0) {
    _objc_msgSend(param_5,"timeIntervalSince1970");
    _objc_msgSend(param_6,"timeIntervalSince1970");
    local_110 = &LAB_04044e30;
    local_108 = &PTR_FUN_0da87ce8;
    local_120[0] = (ID *)FUN_00008790(8);
    *local_120[0] = param_7;
    FUN_04333408(extraout_d0,extraout_d0_00,&local_158,&local_140,&local_170,local_120);
    if ((code *)local_108[1] != (code *)0x0) {
      (*(code *)local_108[1])(local_120[0]);
    }
    if ((char)local_148 != '\0') {
      FUN_04332750(&local_158);
    }
    if ((long)local_130 < 0) {
      FUN_00005810(local_140,(ulong)local_130 & 0x7fffffffffffffff);
    }
    if (local_170 == 0) {
LAB_04044c24:
      if (*(long *)PTR____stack_chk_guard_0d824020 != local_80) {
                    /* WARNING: Subroutine does not return */
        ___stack_chk_fail(0);
      }
      return;
    }
    lVar4 = local_168 << 3;
    do {
      if (lVar4 == 0) {
        local_168 = 0;
        FUN_00005810(local_170,local_160 << 3);
        goto LAB_04044c24;
      }
      lVar4 = lVar4 + -8;
    } while (local_170 + lVar4 != 0);
  }
LAB_04044d68:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4044d6c);
  (*pcVar1)();
}


