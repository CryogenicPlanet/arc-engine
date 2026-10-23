// ==== 0x4341874  FUN_04341874  (240 bytes)

/* WARNING: Removing unreachable block (ram,0x04341938) */
/* WARNING: Removing unreachable block (ram,0x04341950) */

void FUN_04341874(long param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  undefined8 local_e8 [2];
  undefined7 local_d8;
  char cStack_d1;
  undefined1 auStack_c8 [80];
  undefined1 auStack_78 [8];
  undefined1 auStack_70 [80];
  
  cVar3 = *(char *)((long)param_2 + 0x17);
  puVar1 = (undefined8 *)*param_2;
  if (-1 < (long)cVar3) {
    puVar1 = param_2;
  }
  lVar2 = param_2[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  FUN_00c28450(local_e8,puVar1,lVar2);
  FUN_004b7f9c(auStack_70,local_e8);
  if (*(char *)(param_1 + 0x80) == '\0') {
    if (param_1 + 0x30 == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x4341950);
      (*pcVar4)();
    }
    FUN_00826908(param_1 + 0x30,auStack_70);
    *(undefined1 *)(param_1 + 0x80) = 1;
  }
  else {
    FUN_00b10fe0(param_1 + 0x30,auStack_70);
  }
  FUN_000282bc(auStack_78,0);
  FUN_0002827c(auStack_c8);
  if (cStack_d1 < '\0') {
    FUN_00005810(local_e8[0],CONCAT17(cStack_d1,local_d8) & 0x7fffffffffffffff);
  }
  return;
}


