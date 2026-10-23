// ==== 0x16220b0  FUN_016220b0  (9416 bytes)

ushort * FUN_016220b0(long param_1,byte *param_2,long *param_3,undefined8 param_4,ushort *param_5,
                     undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  ulong uVar1;
  undefined4 uVar2;
  ushort uVar3;
  ushort uVar4;
  byte bVar5;
  byte bVar6;
  char cVar7;
  byte bVar8;
  byte bVar9;
  long lVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  uint uVar14;
  ushort *puVar16;
  undefined4 *puVar17;
  uint uVar15;
  undefined1 (*pauVar18) [16];
  undefined8 *puVar19;
  undefined8 *puVar20;
  ushort *puVar21;
  undefined8 *puVar22;
  undefined8 uVar23;
  ulong uVar24;
  code *UNRECOVERED_JUMPTABLE_00;
  int iVar25;
  uint uVar26;
  long lVar27;
  ulong uVar28;
  uint uVar29;
  ulong uVar30;
  long lVar31;
  uint uVar32;
  uint *puVar33;
  ulong *puVar34;
  long *plVar35;
  long *plVar36;
  undefined8 uVar37;
  undefined8 uVar38;
  undefined8 uVar39;
  undefined8 uVar40;
  undefined8 uVar41;
  undefined8 uVar42;
  undefined1 auVar43 [16];
  undefined1 auVar44 [16];
  undefined1 auVar45 [16];
  undefined1 auVar46 [16];
  undefined1 auVar47 [16];
  undefined1 auVar48 [16];
  undefined1 auVar49 [16];
  undefined1 auVar50 [16];
  undefined1 auVar51 [12];
  undefined1 auVar52 [12];
  undefined1 auVar53 [12];
  undefined1 auVar54 [12];
  ulong in_stack_ffffffffffffff30;
  ulong *local_b0;
  undefined4 **ppuStack_a8;
  ulong *local_a0;
  undefined8 local_98;
  ulong local_90;
  undefined4 *local_88;
  ulong *local_80;
  ulong **local_78;
  undefined8 local_70;
  undefined8 local_68;
  
  puVar21 = (ushort *)(param_2 + 1);
  uVar30 = (ulong)*param_2;
  if ((char)*param_2 < '\0') {
    bVar6 = (byte)*puVar21;
    if ((char)bVar6 < '\0') {
      bVar5 = param_2[2];
      uVar30 = (uVar30 & 0x7f) << 0x32 | (ulong)bVar6 << 0x39;
      if ((char)bVar5 < '\0') {
        bVar6 = param_2[3];
        uVar30 = uVar30 >> 7 | (long)(char)bVar5 << 0x39;
        if ((char)bVar6 < '\0') {
          uVar24 = (ulong)(char)param_2[4];
          if ((long)uVar24 < 0) goto LAB_016221e8;
          uVar30 = (uVar30 >> 7 | (long)(char)bVar6 << 0x39 | uVar24) >> 0x24 | uVar24 << 0x1c;
          puVar21 = (ushort *)(param_2 + 5);
        }
        else {
          uVar30 = uVar30 >> 0x2b | (ulong)bVar6 << 0x15;
          puVar21 = (ushort *)(param_2 + 4);
        }
      }
      else {
        uVar30 = uVar30 >> 0x32 | (ulong)bVar5 << 0xe;
        puVar21 = (ushort *)(param_2 + 3);
      }
    }
    else {
      uVar30 = uVar30 & 0x7f | (ulong)bVar6 << 7;
      puVar21 = (ushort *)(param_2 + 2);
    }
  }
  if (puVar21 == (ushort *)0x0) {
LAB_016221e8:
    puVar21 = (ushort *)switchD_016221e4::caseD_8();
    return puVar21;
  }
  uVar32 = (uint)(uVar30 >> 3) & 0x1fffffff;
  uVar15 = uVar32 - 1;
  if (0x1f < uVar15) {
    for (puVar33 = (uint *)((ulong)param_5[5] + (long)param_5); *puVar33 <= uVar32;
        puVar33 = (uint *)((long)puVar33 + (ulong)(ushort)puVar33[1] * 4 + 6)) {
      uVar15 = uVar32 - *puVar33;
      uVar29 = uVar15 >> 4;
      if (uVar29 < (ushort)puVar33[1]) {
        puVar16 = (ushort *)((long)puVar33 + (ulong)(uVar29 << 1) * 2 + 6);
        uVar3 = *puVar16;
        uVar15 = uVar15 & 0xf;
        if ((uVar3 >> (ulong)uVar15 & 1) == 0) {
          uVar32 = (1 << (ulong)uVar15) - 1U & (uint)uVar3;
          uVar15 = (uVar15 - (byte)(POPCOUNT((char)uVar32) + POPCOUNT((char)(uVar32 >> 8)))) +
                   (uint)puVar16[1];
          goto LAB_016221b8;
        }
        break;
      }
    }
LAB_016220ec:
                    /* WARNING: Could not recover jumptable at 0x016220f4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    puVar21 = (ushort *)(**(code **)(param_5 + 0x18))();
    return puVar21;
  }
  uVar32 = 1 << (ulong)(uVar15 & 0x1f);
  if ((*(uint *)(param_5 + 6) & uVar32) != 0) goto LAB_016220ec;
  uVar32 = *(uint *)(param_5 + 6) & uVar32 - 1;
  uVar15 = uVar15 - (byte)(POPCOUNT((char)uVar32) + POPCOUNT((char)(uVar32 >> 8)) +
                           POPCOUNT((char)(uVar32 >> 0x10)) + POPCOUNT((char)(uVar32 >> 0x18)));
LAB_016221b8:
  lVar27 = (long)param_5 + (ulong)uVar15 * 0xc + (ulong)*(uint *)(param_5 + 8);
  uVar28 = (ulong)(uint)((int)lVar27 - (int)param_5);
  uVar24 = uVar30 & 0xffffffff | uVar28 << 0x20;
  uVar32 = (uint)param_6;
  uVar15 = (uint)uVar30;
  switch(*(ushort *)(lVar27 + 10) & 0xf) {
  case 0:
                    /* WARNING: Could not recover jumptable at 0x0848bdb4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    puVar21 = (ushort *)(**(code **)(param_5 + 0x18))();
    return puVar21;
  case 1:
    puVar33 = (uint *)((long)param_5 + uVar28);
    uVar3 = *(ushort *)((long)puVar33 + 10);
    if ((uVar3 & 0x30) == 0x20) {
      puVar21 = (ushort *)FUN_08493160();
      return puVar21;
    }
    local_78 = (ulong **)param_6;
    local_70 = param_3;
    local_68 = param_5;
    if ((uVar30 & 7) == 0) {
      puVar16 = (ushort *)FUN_01622840(puVar21,&stack0xffffffffffffffa8);
      if (puVar16 == (ushort *)0x0) {
        puVar21 = (ushort *)switchD_016221e4::caseD_8(param_1);
        return puVar21;
      }
      uVar4 = uVar3 & 0x1c0;
      if (((uVar4 != 0xc0) && (uVar4 == 0x80)) && ((uVar3 >> 10 & 1) != 0)) {
        uVar23 = *(undefined8 *)
                  ((long)local_68 + (ulong)(ushort)puVar33[2] * 8 + (ulong)*(uint *)(local_68 + 0xc)
                  );
        if ((uVar3 & 0x600) == 0x600) {
          bVar11 = false;
          bVar12 = false;
          bVar13 = false;
          if ((int)uVar23 < 1) {
            iVar25 = (int)((ulong)uVar23 >> 0x20);
            bVar13 = SBORROW4(0,iVar25);
            bVar11 = -iVar25 < 0;
            bVar12 = iVar25 == 0;
          }
          if (!bVar12 && bVar11 == bVar13) goto LAB_0848c014;
        }
        else {
          uVar28 = FUN_022c24b0(0);
          if ((uVar28 & 1) == 0) {
LAB_0848c014:
            puVar21 = (ushort *)FUN_08490050(param_1,puVar21,loca