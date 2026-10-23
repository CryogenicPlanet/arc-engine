// ==== 0x43fe184  ArcWebContentsDevTools::nativeDevTools  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsDevTools::nativeDevTools
               (optional<arc::WebContents::DevTools> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x30) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::WebContents::DevTools> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x43fe1c4);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x28);
    pIVar4 = *(Impl **)(param_1 + 0x20);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x28);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}


