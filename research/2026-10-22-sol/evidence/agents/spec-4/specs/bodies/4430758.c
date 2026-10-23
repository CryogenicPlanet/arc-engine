// ==== 0x4430758  ArcCookie::setPath:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setPath_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ulong uVar2;
  undefined8 ***pppuVar3;
  code *pcVar4;
  long lVar5;
  undefined8 **local_38;
  ulong uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4430840);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  lVar5 = *(long *)(param_1 + 8);
  pppuVar1 = (undefined8 ***)(lVar5 + 0x30);
  if (pppuVar1 != &local_38) {
    if (*(char *)(lVar5 + 0x47) < '\0') {
      uVar2 = uStack_30;
      pppuVar3 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        uVar2 = local_28 >> 0x38;
        pppuVar3 = &local_38;
      }
      FUN_00359ad8(pppuVar1,pppuVar3,uVar2);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar1,local_38,uStack_30);
    }
    else {
      *(ulong *)(lVar5 + 0x40) = local_28;
      *(ulong *)(lVar5 + 0x38) = uStack_30;
      *pppuVar1 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}


