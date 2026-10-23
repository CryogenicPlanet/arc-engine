// ==== 0x443181c  FUN_0443181c  (444 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_0443181c(long *param_1,ID param_2)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  ID IVar4;
  undefined1 local_174 [4];
  undefined8 local_170;
  long lStack_168;
  long *local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  long *local_128;
  undefined1 *puStack_120;
  long *local_118;
  undefined1 *local_110;
  long *plStack_108;
  long *local_100;
  long local_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  IVar2 = _objc_msgSend(param_2,"count");
  FUN_05834b80(param_1,IVar2);
  uStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  uStack_140 = 0;
  lStack_168 = 0;
  local_170 = 0;
  uStack_158 = 0;
  local_160 = (long *)0x0;
  IVar2 = _objc_msgSend(param_2,"countByEnumeratingWithState:objects:count:",&local_170,auStack_f0,
                        0x10);
  if (IVar2 != 0) {
    lVar3 = *local_160;
    do {
      IVar4 = 0;
      do {
        if (*local_160 != lVar3) {
          _objc_enumerationMutation(param_2);
        }
        _objc_msgSend(*(ID *)(lStack_168 + IVar4 * 8),"floatValue");
        local_f8 = *param_1 + param_1[1] * 4;
        local_128 = &local_f8;
        puStack_120 = local_174;
        local_118 = param_1;
        local_110 = local_174;
        plStack_108 = param_1;
        local_100 = &local_f8;
        FUN_01f01f1c(param_1[1] != param_1[2],&local_110,&local_128);
        param_1[1] = local_f8 - *param_1 >> 2;
        if (local_f8 - *param_1 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x4431a34);
          (*pcVar1)();
        }
        IVar4 = IVar4 + 1;
      } while (IVar2 != IVar4);
      IVar2 = _objc_msgSend(param_2,"countByEnumeratingWithState:objects:count:",&local_170,
                            auStack_f0,0x10);
    } while (IVar2 != 0);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


