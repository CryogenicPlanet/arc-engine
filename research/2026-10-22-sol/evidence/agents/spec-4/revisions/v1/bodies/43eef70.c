// ==== 0x43eef70  objc::Categories::ArcBrowserContext_Extensions_::setContentSettingsForOrigin:withContentSettingTypes:withSiteSetting:  (596 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setContentSettingsForOrigin_withContentSettingTypes_withSiteSetting_
               (ID param_1,SEL param_2,ID param_3,ID param_4,unsigned_long_long param_5)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  long lVar5;
  long local_d0;
  long local_c8;
  long local_c0;
  undefined8 local_b8 [2];
  undefined7 local_a8;
  char cStack_a1;
  undefined1 auStack_a0 [8];
  long *local_98;
  byte local_90;
  long *local_88;
  undefined4 *puStack_80;
  undefined1 *local_78;
  undefined4 *local_70;
  undefined1 *puStack_68;
  long *local_60;
  long local_58;
  
  _objc_msgSend(param_1,"browser_context",auStack_a0);
  if ((local_90 & 1) != 0) {
    FUN_043abdf8(local_b8,param_3);
    local_d0 = 0;
    local_c8 = 0;
    local_c0 = 0;
    puVar4 = &DAT_0c9d7d10;
    lVar5 = 0x250;
    do {
      IVar2 = _objc_msgSend(param_4,"containsIndex:",*(undefined8 *)(puVar4 + -2));
      if ((int)IVar2 != 0) {
        local_58 = local_d0 + local_c8 * 4;
        local_88 = &local_58;
        puStack_80 = puVar4;
        local_78 = (undefined1 *)&local_d0;
        local_70 = puVar4;
        puStack_68 = (undefined1 *)&local_d0;
        local_60 = &local_58;
        FUN_042da3c8(local_c8 != local_c0,&local_70,&local_88);
        local_c8 = local_58 - local_d0 >> 2;
        if (local_58 - local_d0 == 0) goto LAB_043ef070;
      }
      puVar4 = puVar4 + 4;
      lVar5 = lVar5 + -0x10;
    } while (lVar5 != 0);
    if (4 < param_5) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43ef2fc);
      (*pcVar1)();
    }
    FUN_043339fc(auStack_a0,local_b8,&local_d0,param_5);
    if (local_d0 != 0) {
      if (local_c8 * 4 != 0) {
        lVar5 = local_c8 * -4;
        lVar3 = local_c8 * 4 + local_d0;
        do {
          lVar3 = lVar3 + -4;
          if (lVar3 == 0) goto LAB_043ef070;
          lVar5 = lVar5 + 4;
        } while (lVar5 != 0);
      }
      local_c8 = 0;
      FUN_00005810(local_d0,local_c0 << 2);
    }
    if (cStack_a1 < '\0') {
      FUN_00005810(local_b8[0],CONCAT17(cStack_a1,local_a8) & 0x7fffffffffffffff);
    }
    if ((local_90 != 0) && (local_98 != (long *)0x0)) {
      LOAcquire();
      lVar5 = local_98[1];
      local_98[1] = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*local_98 + 0x10))(local_98);
        if (local_98[2] != 0) {
          LOAcquire();
          lVar5 = local_98[2];
          local_98[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) {
            return;
          }
        }
        pcVar1 = *(code **)(*local_98 + 0x20);
        if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
          FUN_00005810(local_98,0x20);
        }
        else {
          (*pcVar1)(local_98);
        }
      }
    }
    return;
  }
LAB_043ef070:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43ef074);
  (*pcVar1)();
}


