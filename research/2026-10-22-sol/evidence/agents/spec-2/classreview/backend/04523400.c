// ==== 0x4523400  FUN_04523400  (452 bytes)

/* WARNING: Type propagation algorithm not settling */

void FUN_04523400(undefined8 *param_1,long param_2)

{
  undefined8 *puVar1;
  long *plVar2;
  size_t sVar3;
  ulong uVar4;
  char cVar5;
  int iVar6;
  undefined8 *******pppppppuVar7;
  ulong uVar8;
  long *plVar9;
  undefined1 *puVar10;
  undefined8 *******local_80;
  ulong uStack_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 *******local_58;
  ulong uStack_50;
  undefined8 local_48;
  
  plVar9 = *(long **)(param_2 + 0x18);
  if (plVar9 != (long *)0x0) {
    cVar5 = *(char *)(param_2 + 0x3f);
    puVar1 = *(void **)(param_2 + 0x28);
    if (-1 < (long)cVar5) {
      puVar1 = (undefined8 *)(param_2 + 0x28);
    }
    uVar8 = *(ulong *)(param_2 + 0x30);
    if (-1 < cVar5) {
      uVar8 = (long)cVar5;
    }
    do {
      cVar5 = *(char *)((long)plVar9 + 0x37);
      plVar2 = (long *)plVar9[4];
      if (-1 < (long)cVar5) {
        plVar2 = plVar9 + 4;
      }
      uVar4 = plVar9[5];
      if (-1 < cVar5) {
        uVar4 = (long)cVar5;
      }
      sVar3 = uVar4;
      if (uVar8 <= uVar4) {
        sVar3 = uVar8;
      }
      iVar6 = _memcmp(puVar1,plVar2,sVar3);
      if (iVar6 == 0) {
        if (uVar8 == uVar4) {
          if ((plVar9 != (long *)(param_2 + 0x18)) && ((*(byte *)(plVar9 + 0xb) & 1) != 0)) {
            uStack_78 = 0;
            local_80 = (undefined8 *******)0x0;
            local_68 = 0;
            local_70 = 0;
            if ((*(byte *)(plVar9 + 0xb) & 1) == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_04b08a00();
            }
            uVar8 = plVar9[9];
            if (0x7fffffffffffffee < uVar8) {
                    /* WARNING: Subroutine does not return */
              FUN_04b089c0();
            }
            puVar10 = (undefined1 *)plVar9[8];
            if (0x16 < uVar8) {
              pppppppuVar7 = (undefined8 *******)FUN_00008790((uVar8 & 0x7ffffffffffffff0) + 0x10);
              local_48 = (uVar8 & 0x7ffffffffffffff0) + 0x8000000000000010;
              local_58 = pppppppuVar7;
              uStack_50 = uVar8;
              goto LAB_045235a8;
            }
            local_48 = CONCAT17((char)uVar8,(undefined7)local_48);
            pppppppuVar7 = &local_58;
            for (; uVar8 != 0; uVar8 = uVar8 - 1) {
LAB_045235a8:
              *(undefined1 *)pppppppuVar7 = *puVar10;
              puVar10 = puVar10 + 1;
              pppppppuVar7 = (undefined8 *******)((long)pppppppuVar7 + 1);
            }
            *(undefined1 *)pppppppuVar7 = 0;
            local_70 = local_48;
            uVar8 = local_70;
            uStack_78 = uStack_50;
            local_80 = local_58;
            local_70._7_1_ = (char)(local_48 >> 0x38);
            pppppppuVar7 = local_58;
            if (-1 < (long)local_70._7_1_) {
              pppppppuVar7 = &local_80;
            }
            uVar4 = (ulong)local_68 >> 8;
            local_68 = CONCAT71((int7)uVar4,1);
            uVar4 = uStack_50;
            if (-1 < (long)local_48) {
              uVar4 = (long)local_70._7_1_;
            }
            uStack_50 = 0;
            local_48 = 0;
            local_58 = (undefined8 *******)0x0;
            local_70 = uVar8;
            FUN_013158b8(pppppppuVar7,uVar4,&local_58);
            param_1[1] = uStack_50;
            *param_1 = local_58;
            param_1[2] = local_48;
            *(undefined1 *)(param_1 + 3) = 1;
            if ((char)local_68 != '\0') {
              if ((long)local_70 < 0) {
                FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
                return;
              }
              return;
            }
            return;
          }
          break;
        }
        if (uVar8 < uVar4) goto LAB_045234c4;
LAB_0452347c:
        plVar9 = (long *)plVar9[1];
      }
      else {
        if (-1 < iVar6) goto LAB_0452347c;
LAB_045234c4:
        plVar9 = (long *)*plVar9;
      }
    } while (plVar9 != (long *)0x0);
  }
  *(undefined1 *)param_1 = 0;
  *(undefined1 *)(param_1 + 3) = 0;
  return;
}


