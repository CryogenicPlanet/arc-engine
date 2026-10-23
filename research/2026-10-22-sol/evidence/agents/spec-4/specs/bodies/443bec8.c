// ==== 0x443bec8  ArcExternalProtocolDialog::external_protocol_dialog  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExternalProtocolDialog::external_protocol_dialog
               (optional<arc::ExternalProtocolDialog> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x90 = 0;
  if (*(char *)(param_1 + 0x98) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ExternalProtocolDialog> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443bf10);
      (*pcVar1)();
    }
    FUN_04335c68(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)&__return_storage_ptr__->field1_0x90 = 1;
  }
  return;
}


