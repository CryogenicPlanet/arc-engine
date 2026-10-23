// ==== 0x4407384  ArcMediaImage::initWithMediaImage:  (256 bytes)

/* Function Stack Size: 0x20 bytes */

ID ArcMediaImage::initWithMediaImage_(ID param_1,SEL param_2,MediaImage param_3)

{
  undefined8 *puVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 *puVar4;
  MediaImage_conflict *pMVar5;
  size_t sVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined1 auVar9 [16];
  undefined8 *local_58;
  ulong local_50;
  long local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pMVar5 = param_3.field0_0x0.field0_0x0;
  pcStack_38 = &objc::class_t::ArcMediaImage;
  local_40 = param_1;
  IVar3 = NSObject::init((ID)&local_40,"init",pMVar5,param_3.field0_0x0.field1_0x8);
  if (IVar3 != 0) {
    FUN_04331e20(IVar3 + 0x10,pMVar5);
    FUN_0433cab0(&local_58,*(long *)pMVar5 + 0x90);
    sVar6 = local_50 << 4;
    if (local_50 >> 0x3c != 0) {
      sVar6 = 0xffffffffffffffff;
    }
    puVar4 = (undefined8 *)FUN_00008790(sVar6);
    _bzero(puVar4,sVar6);
    puVar7 = puVar4;
    puVar1 = local_58;
    for (lVar8 = local_50 * 8; lVar8 != 0; lVar8 = lVar8 + -8) {
      auVar9._0_8_ = (long)(int)*puVar1;
      auVar9._8_8_ = (long)(int)((ulong)*puVar1 >> 0x20);
      auVar9 = NEON_scvtf(auVar9,8);
      puVar7[1] = auVar9._8_8_;
      *puVar7 = auVar9._0_8_;
      puVar7 = puVar7 + 2;
      puVar1 = puVar1 + 1;
    }
    *(undefined8 **)(IVar3 + 8) = puVar4;
    if (local_58 != (undefined8 *)0x0) {
      if (local_50 * 8 != 0) {
        lVar8 = local_50 * -8;
        puVar7 = local_58 + local_50;
        do {
          puVar7 = puVar7 + -1;
          if (puVar7 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(1,0x4407494);
            (*pcVar2)();
          }
          lVar8 = lVar8 + 8;
        } while (lVar8 != 0);
      }
      local_50 = 0;
      FUN_00005810(local_58,local_48 << 3);
    }
  }
  return IVar3;
}


