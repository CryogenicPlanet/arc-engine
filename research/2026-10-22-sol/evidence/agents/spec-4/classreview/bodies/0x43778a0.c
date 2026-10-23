// ==== 0x43778a0  FUN_043778a0  (344 bytes)

void FUN_043778a0(undefined8 *param_1,long param_2)

{
  uint uVar1;
  code *pcVar2;
  long lVar3;
  char *pcVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  if (*(long *)(param_2 + 0x130) == 0) {
    pcVar4 = "managed webcontents is null";
    uVar5 = 0x1b;
    uVar6 = 0x707;
  }
  else {
    uVar1 = *(uint *)(*(long *)(param_2 + 0x130) + 0x958);
    if (uVar1 != 4) {
      if (0xb < uVar1) {
LAB_043779b8:
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x43779bc);
        (*pcVar2)();
      }
      if ((0x58eU >> (ulong)(uVar1 & 0x1f) & 1) != 0) {
        pcVar4 = "managed webcontents has crashed";
        uVar5 = 0x1f;
        uVar6 = 0x70b;
        goto LAB_0437794c;
      }
      if (uVar1 != 0) {
        if (uVar1 == 0xb) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x43779e0);
          (*pcVar2)();
        }
        goto LAB_043779b8;
      }
    }
    lVar3 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8));
    if (((((lVar3 != 0) && (lVar3 = FUN_0031dab4(), lVar3 != 0)) && (*(long *)(lVar3 + 0x28) != 0))
        && ((*(char *)(*(long *)(lVar3 + 0x28) + 4) == '\0' && (*(long *)(lVar3 + 0x30) != 0)))) &&
       ((*(long *)(lVar3 + 0x28) != 0 &&
        ((*(char *)(*(long *)(lVar3 + 0x28) + 4) == '\0' &&
         (lVar3 = *(long *)(lVar3 + 0x30), lVar3 != 0 && lVar3 != 0x28)))))) {
      uVar6 = *(undefined8 *)(param_2 + 0x130);
      uVar5 = (**(code **)(**(long **)(lVar3 + 0x10) + 0x168))();
      uVar5 = FUN_003bc2f4(uVar5,"printing.print_preview_disabled",0x1f);
      FUN_07f73e84(uVar6,uVar5,0);
      uVar5 = 1;
      goto LAB_04377950;
    }
    pcVar4 = "webcontents not attached to any browser";
    uVar5 = 0x27;
    uVar6 = 0x711;
  }
LAB_0437794c:
  uVar5 = FUN_04b37104(0x25,pcVar4,uVar5,uVar6,"../../arc/browser/web_contents/web_contents_impl.cc"
                      );
LAB_04377950:
  *param_1 = uVar5;
  return;
}


