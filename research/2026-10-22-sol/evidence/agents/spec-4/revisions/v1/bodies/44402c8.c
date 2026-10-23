// ==== 0x44402c8  ArcMenuItemModel::menu_item_model  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMenuItemModel::menu_item_model
               (optional<arc::MenuItemModel> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x48) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::MenuItemModel> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x4440308);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x40);
    pIVar4 = *(Impl **)(param_1 + 0x38);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x40);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}


