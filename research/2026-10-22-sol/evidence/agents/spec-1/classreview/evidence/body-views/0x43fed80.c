// ==== 0x43fed80  ArcAnnotatedVisit::init  (224 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAnnotatedVisit::init(ID param_1,SEL param_2)

{
  ID IVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAnnotatedVisit;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x10) != '\0') {
      lVar2 = *(long *)(IVar1 + 8);
      *(undefined8 *)(IVar1 + 8) = 0;
      if (lVar2 != 0) {
        uVar3 = FUN_04336e98();
        FUN_00005810(uVar3,0x150);
      }
    }
    puVar4 = (undefined8 *)FUN_00008790(0x150);
    puVar4[1] = 0;
    *puVar4 = 0;
    puVar4[3] = 0;
    puVar4[2] = 0;
    puVar4[5] = 0;
    puVar4[4] = 0;
    puVar4[7] = 0;
    puVar4[6] = 0;
    puVar4[9] = 0;
    puVar4[8] = 0;
    puVar4[0xb] = 0;
    puVar4[10] = 0;
    puVar4[0xd] = 0;
    puVar4[0xc] = 0;
    puVar4[0xf] = 0;
    puVar4[0xe] = 0;
    puVar4[0x11] = 0;
    puVar4[0x10] = 0;
    puVar4[0x13] = 0;
    puVar4[0x12] = 0;
    puVar4[0x15] = 0;
    puVar4[0x14] = 0;
    puVar4[0x17] = 0;
    puVar4[0x16] = 0;
    puVar4[0x1b] = 0;
    puVar4[0x1a] = 0;
    puVar4[0x1d] = 0;
    puVar4[0x1c] = 0;
    puVar4[0x1f] = 0;
    puVar4[0x1e] = 0;
    puVar4[0x21] = 0;
    puVar4[0x20] = 0;
    puVar4[0x23] = 0;
    puVar4[0x22] = 0;
    puVar4[0x25] = 0;
    puVar4[0x24] = 0;
    puVar4[0x27] = 0;
    puVar4[0x26] = 0;
    puVar4[0x29] = 0;
    puVar4[0x28] = 0;
    puVar4[0x19] = 0;
    puVar4[0x18] = 0;
    puVar4[0x12] = 0xffffffffffffffff;
    puVar4[0x11] = 0xffffffffffffffff;
    puVar4[0x10] = 0xffffffffffffffff;
    puVar4[0x14] = 0;
    puVar4[0x13] = 0;
    puVar4[0x16] = 0;
    puVar4[0x15] = 0;
    *(undefined8 *)((long)puVar4 + 0xb5) = 0;
    puVar4[0x19] = 0;
    puVar4[0x18] = 0;
    puVar4[0x1b] = 0;
    puVar4[0x1a] = 0;
    puVar4[0x1d] = 0;
    puVar4[0x1c] = 0;
    puVar4[0x1f] = 0;
    puVar4[0x1e] = 0;
    puVar4[0x21] = 0;
    puVar4[0x20] = 0;
    puVar4[0x23] = 0;
    puVar4[0x22] = 0;
    puVar4[0x25] = 0;
    puVar4[0x24] = 0;
    puVar4[0x27] = 0;
    puVar4[0x26] = 0;
    *(undefined8 *)((long)puVar4 + 0x141) = 0;
    *(undefined8 *)((long)puVar4 + 0x139) = 0;
    *(undefined8 **)(IVar1 + 8) = puVar4;
    *(undefined1 *)(IVar1 + 0x10) = 1;
  }
  return IVar1;
}


