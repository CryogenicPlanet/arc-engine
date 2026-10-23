// ==== 0x437a678  FUN_0437a678  (396 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_0437a678(undefined8 *param_1,long param_2)

{
  undefined8 *puVar1;
  long lVar2;
  long lVar3;
  undefined8 uVar4;
  char *pcVar5;
  undefined8 uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  undefined8 *local_40 [2];
  
  if (*(long *)(param_2 + 0x130) == 0) {
    pcVar5 = "Attempted to open certificate dialog for nullptr WebContents.";
    uVar4 = 0x3d;
    uVar6 = 0x94a;
LAB_0437a7b4:
    uVar4 = FUN_04b37104(0x25,pcVar5,uVar4,uVar6,
                         "../../arc/browser/web_contents/web_contents_impl.cc");
    *param_1 = uVar4;
    return;
  }
  lVar2 = FUN_01383300();
  if (lVar2 == 0) {
    pcVar5 = "Unexpected: Security helper without the visible security state.";
    uVar4 = 0x3f;
    uVar6 = 0x951;
    goto LAB_0437a7b4;
  }
  lVar9 = *(long *)(lVar2 + 0x108);
  if (lVar9 == 0) {
    uVar4 = FUN_04b37104(0xd,"Attempted to open certificate dialog for page without SSL.",0x3a,0x958
                         ,"../../arc/browser/web_contents/web_contents_impl.cc");
    goto LAB_0437a73c;
  }
  lVar3 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),
                       *(undefined8 *)(param_2 + 0x130));
  if ((lVar3 == 0) || (lVar3 = FUN_0031dab4(), lVar3 == 0)) {
LAB_0437a6e4:
    lVar7 = 0;
  }
  else {
    lVar7 = 0;
    if (*(long *)(lVar3 + 0x28) != 0) {
      if (*(char *)(*(long *)(lVar3 + 0x28) + 4) != '\0') goto LAB_0437a6e4;
      lVar7 = 0;
      if (*(long *)(lVar3 + 0x30) != 0) {
        lVar8 = 0;
        if (*(long *)(lVar3 + 0x28) != 0) {
          if (*(char *)(*(long *)(lVar3 + 0x28) + 4) == '\0') {
            lVar8 = *(long *)(lVar3 + 0x30);
          }
          else {
            lVar8 = 0;
          }
        }
        lVar7 = 0;
        if (lVar8 != 0) {
          lVar7 = lVar8 + -0x28;
        }
      }
    }
  }
  (**(code **)(**(long **)(lVar7 + 0x48) + 0x20))(local_40);
  thunk_FUN_0c421b00(*local_40[0],lVar9);
  puVar1 = local_40[0];
  local_40[0] = (undefined8 *)0x0;
  if (puVar1 != (undefined8 *)0x0) {
    _objc_destroyWeak(puVar1);
    FUN_00005810(puVar1,8);
  }
  uVar4 = 1;
LAB_0437a73c:
  *param_1 = uVar4;
  uVar4 = FUN_01382ee0(lVar2);
  FUN_00005810(uVar4,0x130);
  return;
}


