// ==== 0x434b768  FUN_0434b768  (176 bytes)

undefined8 FUN_0434b768(long param_1)

{
  int iVar1;
  long *plVar2;
  ulong uVar3;
  
  iVar1 = _pthread_mutex_trylock((pthread_mutex_t *)(param_1 + 0xc0));
  if (iVar1 != 0) {
    FUN_002b39c8(param_1 + 0xc0,&PTR_s_BaseLock_0df6cbd8);
  }
  if ((*(long *)(param_1 + 0xb8) != 0) &&
     (plVar2 = *(long **)(param_1 + 0x88), plVar2 != (long *)0x0)) {
    if (*(code **)(*plVar2 + 0x10) == (code *)&LAB_0444efc0) {
      if ((plVar2[4] != 0) && (*(int *)plVar2[3] != 1)) {
LAB_0434b7e4:
        FUN_009de658(param_1 + 0x58);
        _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xc0));
        FUN_0434a4d8(param_1);
        return 1;
      }
    }
    else {
      uVar3 = (**(code **)(*plVar2 + 0x10))();
      if ((uVar3 & 1) != 0) goto LAB_0434b7e4;
    }
  }
  _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xc0));
  return 0;
}


