// ==== 0x4436dec  ArcBrowserContextEffects::registerZoomFactor:forHostPattern:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

void ArcBrowserContextEffects::registerZoomFactor_forHostPattern_
               (ID param_1,SEL param_2,float param_3,ID param_4)

{
  code *pcVar1;
  long lVar2;
  SEL extraout_x1;
  double dVar3;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_4 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_4);
      param_2 = extraout_x1;
    }
    lVar2 = *(long *)(*(long *)(param_1 + 0x10) + 0x30);
    if (lVar2 != 0) {
      FUN_0119426c(lVar2,param_2);
      lVar2 = FUN_04146794();
      if (lVar2 != 0) {
        dVar3 = (double)_log((double)param_3);
        FUN_064fee44(dVar3 / 0.1823215567939546,lVar2,&local_48);
      }
    }
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4436ec0);
  (*pcVar1)();
}


