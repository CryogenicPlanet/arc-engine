// ==== 0x4432d2c  ArcCommandBarRankingInput::initWithCommandBarRankingInput:  (244 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingInput::initWithCommandBarRankingInput_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  void *pvVar4;
  long lVar5;
  long lVar6;
  void *pvVar7;
  ulong uVar8;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingInput;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if (*(char *)(IVar2 + 0x20) != '\0') {
      lVar3 = *(long *)(IVar2 + 8);
      if (lVar3 != 0) {
        lVar6 = *(long *)(IVar2 + 0x10) * 4;
        if (lVar6 != 0) {
          lVar5 = *(long *)(IVar2 + 0x10) * -4;
          lVar6 = lVar6 + lVar3;
          do {
            lVar6 = lVar6 + -4;
            if (lVar6 == 0) {
                    /* WARNING: Does not return */
              pcVar1 = (code *)SoftwareBreakpoint(1,0x4432e20);
              (*pcVar1)();
            }
            lVar5 = lVar5 + 4;
          } while (lVar5 != 0);
        }
        *(undefined8 *)(IVar2 + 0x10) = 0;
        FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
      }
      *(undefined1 *)(IVar2 + 0x20) = 0;
    }
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    uVar8 = *(ulong *)((long)param_3 + 8);
    if (uVar8 != 0) {
      if (uVar8 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar7 = *(void **)param_3;
      pvVar4 = (void *)FUN_00008790(uVar8 << 2);
      *(void **)(IVar2 + 8) = pvVar4;
      *(undefined8 *)(IVar2 + 0x10) = 0;
      *(ulong *)(IVar2 + 0x18) = uVar8;
      _memcpy(pvVar4,pvVar7,uVar8 << 2);
      *(long *)(IVar2 + 0x10) = (long)(uVar8 << 2) >> 2;
    }
    *(undefined1 *)(IVar2 + 0x20) = 1;
  }
  return IVar2;
}


