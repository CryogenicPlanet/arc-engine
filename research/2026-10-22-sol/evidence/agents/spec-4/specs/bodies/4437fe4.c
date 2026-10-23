// ==== 0x4437fe4  ArcBrowserContext::bookmarkObservers  (168 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContext::bookmarkObservers
               (vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
                *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 *puVar6;
  ulong uVar7;
  
  (__return_storage_ptr__->field0_0x0).field0_0x0 = (void *)0x0;
  (__return_storage_ptr__->field0_0x0).field1_0x8 = 0;
  (__return_storage_ptr__->field0_0x0).field2_0x10 = 0;
  uVar7 = *(ulong *)(param_1 + 0x50);
  if (uVar7 != 0) {
    if (uVar7 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    puVar6 = *(undefined8 **)(param_1 + 0x48);
    puVar1 = puVar6 + uVar7 * 2;
    puVar3 = (undefined8 *)FUN_00008790(uVar7 << 4);
    (__return_storage_ptr__->field0_0x0).field0_0x0 = puVar3;
    (__return_storage_ptr__->field0_0x0).field2_0x10 = uVar7;
    puVar4 = puVar3;
    do {
      if (puVar4 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x4438088);
        (*pcVar2)();
      }
      *puVar4 = *puVar6;
      lVar5 = puVar6[1];
      puVar4[1] = lVar5;
      if (lVar5 != 0) {
        *(long *)(lVar5 + 8) = *(long *)(lVar5 + 8) + 1;
      }
      puVar6 = puVar6 + 2;
      puVar4 = puVar4 + 2;
    } while (puVar6 != puVar1);
    (__return_storage_ptr__->field0_0x0).field1_0x8 = (long)puVar4 - (long)puVar3 >> 4;
  }
  return;
}


