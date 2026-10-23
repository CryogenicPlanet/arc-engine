// ==== 0x440cda8  ArcTracingController::isTracing  (112 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcTracingController::isTracing(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  int iVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x20);
    iVar3 = _pthread_mutex_trylock((pthread_mutex_t *)(lVar4 + 0xc0));
    if (iVar3 != 0) {
      FUN_002b39c8(lVar4 + 0xc0,&PTR_s_BaseLock_0df6cbd8);
    }
    bVar2 = *(long *)(lVar4 + 0xb8) != 0;
    _pthread_mutex_unlock((pthread_mutex_t *)(lVar4 + 0xc0));
    *(bool *)(param_1 + 8) = bVar2;
    return (bool)bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440ce04);
  (*pcVar1)();
}


