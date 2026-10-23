// ==== 0x43fce5c  ArcBrowserContextExtensions::listExtensions  (364 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextExtensions::listExtensions(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  code *pcVar2;
  ID IVar3;
  ID IVar4;
  undefined1 *puVar5;
  long lVar6;
  const_void__ local_68;
  long local_60;
  long local_58;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    local_68 = (const_void__)0x0;
    local_60 = 0;
    local_58 = 0;
    FUN_042e65f4(&local_68,*(undefined8 *)(param_1 + 0x20));
    IVar3 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
    pvVar1 = local_68;
    for (lVar6 = local_60 * 0xd8; lVar6 != 0; lVar6 = lVar6 + -0xd8) {
      IVar4 = _objc_alloc(&objc::class_t::ArcExtensionInfo);
      IVar4 = ArcExtensionInfo::initWithExtensionInfo_(IVar4,"initWithExtensionInfo:",pvVar1);
      NSMutableArray::addObject_(IVar3,"addObject:",IVar4);
      pvVar1 = (const_void__)((long)pvVar1 + 0xd8);
    }
    IVar3 = NSMutableArray::copy(IVar3,"copy");
    pvVar1 = local_68;
    if (local_68 != (const_void__)0x0) {
      if (local_60 * 0xd8 != 0) {
        puVar5 = (undefined1 *)(local_60 * 0xd8 + (long)local_68);
        lVar6 = local_60 * -0xd8;
        do {
          if ((qword *)puVar5 == &section_000000b8.addr) goto LAB_043fcfd8;
          puVar5 = (undefined1 *)FUN_04334d2c();
          lVar6 = lVar6 + 0xd8;
        } while (lVar6 != 0);
      }
      local_60 = ((long)pvVar1 - (long)local_68 >> 3) * -0x7b425ed097b425ed;
      FUN_00005810(local_68,local_58 * 0xd8);
    }
    return IVar3;
  }
LAB_043fcfd8:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x43fcfdc);
  (*pcVar2)();
}


