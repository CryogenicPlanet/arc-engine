// ==== 0x4414c18  ArcWebContentsConfiguration::setForcedViewSize:  (112 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcWebContentsConfiguration::setForcedViewSize_(ID param_1,SEL param_2,CGSize param_3)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  ulong uVar4;
  undefined1 uVar5;
  double dVar6;
  double dVar7;
  
  dVar7 = param_3.field1_0x8;
  dVar6 = param_3.field0_0x0;
  uVar4 = 0;
  uVar5 = 0;
  if ((dVar6 <= 0.10000000149011612) || (dVar7 <= 0.10000000149011612)) {
LAB_04414c78:
    *(ulong *)(param_1 + 0x14) = uVar4;
    *(undefined1 *)(param_1 + 0x1c) = uVar5;
    return;
  }
  if (dVar6 <= 2147483647.0) {
    bVar2 = true;
    bVar3 = false;
    if (dVar7 <= 2147483647.0) {
      bVar2 = false;
      bVar3 = true;
      if (!NAN(dVar7)) {
        bVar2 = dVar7 < -2147483648.0;
        bVar3 = false;
      }
    }
    if (bVar2 == bVar3) {
      uVar4 = CONCAT44((int)dVar7,(int)dVar6) & 0xffffffffffffff00 | (ulong)((int)dVar6 & 0xff);
      uVar5 = 1;
      goto LAB_04414c78;
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4414c88);
  (*pcVar1)();
}


