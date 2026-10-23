// ==== 0x440ae34  ArcSessionTabGroup::groupId  (44 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSessionTabGroup::groupId(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  undefined8 *puVar6;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar6 = *(undefined8 **)(param_1 + 8);
    cVar3 = *(char *)((long)puVar6 + 0x17);
    puVar1 = (undefined8 *)*puVar6;
    if (-1 < (long)cVar3) {
      puVar1 = puVar6;
    }
    lVar2 = puVar6[1];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(puVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x440ae60);
  (*pcVar4)();
}


