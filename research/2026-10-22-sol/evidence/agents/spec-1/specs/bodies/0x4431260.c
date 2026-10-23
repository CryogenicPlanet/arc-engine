// ==== 0x4431260  ArcContentNode::content_node  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcContentNode::content_node
               (optional<arc::ContentNode> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 0x30) = 0;
  if (*(char *)(param_1 + 0x1b8) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ContentNode> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x44312a8);
      (*pcVar1)();
    }
    FUN_0429539c(__return_storage_ptr__,param_1 + 0x18);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 0x30) = 1;
  }
  return;
}


