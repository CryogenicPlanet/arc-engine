// ==== 0x4440308  ArcMenuItemModel::menu_item_model_delegate_bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<ArcMenuItemModelDelegateBridge>
ArcMenuItemModel::menu_item_model_delegate_bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<ArcMenuItemModelDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<ArcMenuItemModelDelegateBridge> *)(param_1 + 0x28);
  if (*(long *)(param_1 + 0x30) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x30) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


