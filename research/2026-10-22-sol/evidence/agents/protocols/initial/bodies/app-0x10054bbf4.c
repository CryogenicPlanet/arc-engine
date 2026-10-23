// ==== 0x10054bbf4  _TtC3ADK14ImportDelegate::-[ImportDelegate_didImportLockDialogShow:withDialogBody:completion:]  (152 bytes)

/* Function Stack Size: 0x28 bytes */

void _TtC3ADK14ImportDelegate::__ImportDelegate_didImportLockDialogShow_withDialogBody_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  if (DAT_1044cc760 != -1) {
    _swift_once(&DAT_1044cc760,FUN_100550860);
  }
  FUN_1007efc10(4,&DAT_d000000000000018,0x80000001039324b0,0xd000000000000035,0x8000000103932760,
                0x62,DAT_104613448);
                    /* WARNING: Could not recover jumptable at 0x00010054bc70. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(param_5 + 0x10))(param_5,0);
  return;
}


