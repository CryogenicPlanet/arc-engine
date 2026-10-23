// ==== 0x443c36c  ArcMediaPosition::currentTime  (96 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcMediaPosition::currentTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  double dVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443c3cc);
    (*pcVar1)();
  }
  lVar2 = FUN_032b5dd0(*(undefined8 *)(param_1 + 8));
  if (1 < lVar2 + 0x8000000000000001U) {
    return (double)lVar2 / 1000000.0;
  }
  dVar3 = -INFINITY;
  if (-1 < lVar2) {
    dVar3 = INFINITY;
  }
  return dVar3;
}


