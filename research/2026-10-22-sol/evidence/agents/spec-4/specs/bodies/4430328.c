// ==== 0x4430328  ArcCookie::setName:  (228 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setName_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 **ppuVar1;
  undefined8 ***pppuVar2;
  code *pcVar3;
  undefined8 ***pppuVar4;
  undefined8 **local_38;
  undefined8 *puStack_30;
  undefined8 *local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x443040c);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    puStack_30 = (undefined8 **)0x0;
    local_28 = (undefined8 **)0x0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  pppuVar4 = *(undefined8 ****)(param_1 + 8);
  if (pppuVar4 != &local_38) {
    if (*(char *)((long)pppuVar4 + 0x17) < '\0') {
      ppuVar1 = (undefined8 **)puStack_30;
      pppuVar2 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        ppuVar1 = (undefined8 **)((ulong)local_28 >> 0x38);
        pppuVar2 = &local_38;
      }
      FUN_00359ad8(pppuVar4,pppuVar2,ppuVar1);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar4,local_38,puStack_30);
    }
    else {
      pppuVar4[2] = (undefined8 **)local_28;
      pppuVar4[1] = (undefined8 **)puStack_30;
      *pppuVar4 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,(ulong)local_28 & 0x7fffffffffffffff);
  }
  return;
}


