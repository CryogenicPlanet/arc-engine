// ==== 0x4443ee0  ArcWebContents::web_contents  (60 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContents::web_contents
               (optional<arc::WebContents> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  __shared_weak_count *p_Var1;
  code *pcVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x128) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::WebContents> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x4443f1c);
      (*pcVar2)();
    }
    p_Var1 = *(__shared_weak_count **)(param_1 + 0x120);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = *(Impl **)(param_1 + 0x118);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 = p_Var1;
    if (p_Var1 != (__shared_weak_count *)0x0) {
      *(long *)(p_Var1 + 8) = *(long *)(p_Var1 + 8) + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}


