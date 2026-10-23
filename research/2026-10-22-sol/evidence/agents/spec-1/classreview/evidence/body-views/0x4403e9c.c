// ==== 0x4403e9c  ArcImportSource::initWithImportSource:  (124 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcImportSource::initWithImportSource_(ID param_1,SEL param_2,const_ImportSource__ param_3)

{
  uint uVar1;
  code *pcVar2;
  ID IVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcImportSource;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    *(long *)(IVar3 + 8) = (long)(int)param_3->field0_0x0;
    uVar1 = param_3->field1_0x4 + 1;
    if ((0xe < uVar1) || ((0x7f1dU >> (ulong)(uVar1 & 0x1f) & 1) == 0)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x4403f18);
      (*pcVar2)();
    }
    *(ulong *)(IVar3 + 0x10) = (ulong)(byte)(&DAT_0c9d8158)[uVar1];
  }
  return IVar3;
}


