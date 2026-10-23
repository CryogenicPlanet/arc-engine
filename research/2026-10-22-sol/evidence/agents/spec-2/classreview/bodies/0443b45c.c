// ==== 0x443b45c  ArcConfirmInfobar::confirm_infobar  (76 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcConfirmInfobar::confirm_infobar
               (optional<arc::ConfirmInfobar> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 0;
  if (*(char *)(param_1 + 0x20) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ConfirmInfobar> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x443b4a8);
      (*pcVar2)();
    }
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0 = (undefined4 **)&PTR_FUN_0da7a7d8;
    lVar3 = *(long *)(param_1 + 0x18);
    pIVar4 = *(Impl **)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field1.field1_0x8.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field1.field1_0x8.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 1;
  }
  return;
}


