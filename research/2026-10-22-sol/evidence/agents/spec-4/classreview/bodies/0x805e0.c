// ==== 0x805e0  FUN_000805e0  (436 bytes)

undefined8 FUN_000805e0(long param_1,ulong *param_2)

{
  ulong *puVar1;
  undefined1 auVar2 [16];
  long lVar3;
  code *pcVar4;
  ulong *puVar5;
  ulong *puVar6;
  long lVar7;
  char *pcVar8;
  ulong uVar9;
  ulong uVar10;
  byte bVar11;
  ulong uVar12;
  ulong uVar13;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  byte bVar20;
  undefined8 uVar14;
  byte bVar21;
  
  puVar5 = *(ulong **)(param_1 + 8);
  uVar10 = *puVar5;
  if ((uVar10 & 0x3e) == 0) {
    if (0x7fffff < uVar10) {
      puVar1 = puVar5 + 1;
      puVar6 = *(ulong **)*puVar1;
      puVar5 = puVar6;
      if (puVar6 == param_2) {
        puVar5 = (ulong *)&DAT_0c4b347a;
      }
      pcVar8 = (char *)0x0;
      if (puVar6 == param_2) {
        pcVar8 = (char *)*puVar1;
      }
      goto LAB_00080694;
    }
  }
  else {
    lVar7 = 0;
    puVar5 = (ulong *)puVar5[1];
    uVar9 = -1L << (uVar10 & 0x3f);
    auVar2._8_8_ = 0;
    auVar2._0_8_ = (ulong)param_2 ^ 0xddcf5f0;
    lVar3 = 7 - uVar9;
    Hint_Prefetch(puVar5,0,2,0);
    uVar12 = SUB168(auVar2 * ZEXT816(0x79d5f9e0de1e8cf5),8) ^
             ((ulong)param_2 ^ 0xddcf5f0) * 0x79d5f9e0de1e8cf5;
    uVar10 = uVar10 >> 6 & 0x1f ^ uVar12;
    bVar11 = (byte)(uVar12 >> 0x39);
    while( true ) {
      uVar10 = uVar10 & (uVar9 ^ 0xffffffffffffffff);
      Hint_Prefetch((char *)((long)puVar5 + uVar10 * 0x10 + lVar3),0,0,0);
      uVar14 = *(undefined8 *)((long)puVar5 + uVar10);
      bVar15 = (byte)((ulong)uVar14 >> 8);
      bVar16 = (byte)((ulong)uVar14 >> 0x10);
      bVar17 = (byte)((ulong)uVar14 >> 0x18);
      bVar18 = (byte)((ulong)uVar14 >> 0x20);
      bVar19 = (byte)((ulong)uVar14 >> 0x28);
      bVar20 = (byte)((ulong)uVar14 >> 0x30);
      bVar21 = (byte)((ulong)uVar14 >> 0x38);
      for (uVar12 = CONCAT17(-(bVar21 == bVar11),
                             CONCAT16(-(bVar20 == bVar11),
                                      CONCAT15(-(bVar19 == bVar11),
                                               CONCAT14(-(bVar18 == bVar11),
                                                        CONCAT13(-(bVar17 == bVar11),
                                                                 CONCAT12(-(bVar16 == bVar11),
                                                                          CONCAT11(-(bVar15 ==
                                                                                    bVar11),-((byte)
                                                  uVar14 == bVar11)))))))); uVar12 != 0;
          uVar12 = (uVar12 & 0x8080808080808080) - 0x80 & uVar12 & 0x8080808080808080) {
        uVar13 = (uVar12 & 0xaaaaaaaaaaaaaaaa) >> 1 | (uVar12 & 0x5555555555555555) << 1;
        uVar13 = (uVar13 & 0xcccccccccccccccc) >> 2 | (uVar13 & 0x3333333333333333) << 2;
        uVar13 = (uVar13 & 0xf0f0f0f0f0f0f0f0) >> 4 | (uVar13 & 0xf0f0f0f0f0f0f0f) << 4;
        uVar13 = (uVar13 & 0xff00ff00ff00ff00) >> 8 | (uVar13 & 0xff00ff00ff00ff) << 8;
        uVar13 = (uVar13 & 0xffff0000ffff0000) >> 0x10 | (uVar13 & 0xffff0000ffff) << 0x10;
        uVar13 = uVar10 + ((ulong)LZCOUNT(uVar13 >> 0x20 | uVar13 << 0x20) >> 3) &
                 (uVar9 ^ 0xffffffffffffffff);
        if (*(ulong **)((long)puVar5 + uVar13 * 0x10 + lVar3) == param_2) {
          pcVar8 = (char *)((long)puVar5 + uVar13 * 0x10 + lVar3);
          puVar5 = (ulong *)((long)puVar5 + uVar13);
          goto LAB_00080694;
        }
      }
      if (CONCAT17(-(bVar21 == 0x80),
                   CONCAT16(-(bVar20 == 0x80),
                            CONCAT15(-(bVar19 == 0x80),
                                     CONCAT14(-(bVar18 == 0x80),
                                              CONCAT13(-(bVar17 == 0x80),
                                                       CONCAT12(-(bVar16 == 0x80),
                                                                CONCAT11(-(bVar15 == 0x80),
                                                                         -((byte)uVar14 == 0x80)))))
                                    ))) != 0) break;
      lVar7 = lVar7 + 8;
      uVar10 = lVar7 + uVar10;
    }
  }
  pcVar8 = (char *)0x0;
LAB_00080694:
  if ((pcVar8 != &DAT_0e480b40 && pcVar8 != (char *)0x0) && ((char)*puVar5 < '\0')) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x8073c);
    (*pcVar4)();
  }
  if (pcVar8 == &DAT_0e480b40) {
    FUN_04b08ed0(3,"raw_hash_set.h",0x6ce,"Invalid iterator comparison. %s");
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x80768);
    (*pcVar4)();
  }
  if (pcVar8 == (char *)0x0) {
    uVar14 = 0;
  }
  else {
    if ((char)*puVar5 < '\0') {
      FUN_04b08ed0(3,"raw_hash_set.h",0x67c,
                   "%s called on invalid iterator. The element might have been erased or the table might have rehashed. Consider running with --config=asan to diagnose rehashing issues."
                  );
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x80794);
      (*pcVar4)();
    }
    uVar14 = *(undefined8 *)(pcVar8 + 8);
  }
  return uVar14;
}


