// ==== 0x434b818  FUN_0434b818  (880 bytes)

undefined8 FUN_0434b818(long param_1)

{
  long *plVar1;
  long lVar2;
  int iVar3;
  long *plVar4;
  code *pcVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  long lVar9;
  undefined1 local_78 [16];
  long local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined *local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  iVar3 = _pthread_mutex_trylock((pthread_mutex_t *)(param_1 + 0xc0));
  if (iVar3 != 0) {
    FUN_002b39c8(param_1 + 0xc0,&PTR_s_BaseLock_0df6cbd8);
  }
  if ((*(long *)(param_1 + 0xb8) != 0) &&
     (plVar4 = *(long **)(param_1 + 0x88), plVar4 != (long *)0x0)) {
    if (*(code **)(*plVar4 + 0x10) == (code *)&LAB_0444efc0) {
      if ((plVar4[4] != 0) && (*(int *)plVar4[3] != 1)) {
LAB_0434b8d8:
        FUN_009de658(param_1 + 0x58);
        lVar2 = *(long *)(param_1 + 0x100);
        lVar6 = *(long *)(param_1 + 0x108) * 8;
        lVar9 = lVar2;
        if (lVar6 != 0) {
          plVar4 = (long *)(lVar6 + lVar2);
          lVar9 = *(long *)(param_1 + 0x108) * -8;
          do {
            plVar4 = plVar4 + -1;
            if (plVar4 == (long *)0x0) {
                    /* WARNING: Does not return */
              pcVar5 = (code *)SoftwareBreakpoint(1,0x434bb58);
              (*pcVar5)();
            }
            lVar6 = *plVar4;
            *plVar4 = 0;
            if (lVar6 != 0) {
              FUN_023fbaac();
            }
            lVar9 = lVar9 + 8;
          } while (lVar9 != 0);
          lVar9 = *(long *)(param_1 + 0x100);
        }
        *(long *)(param_1 + 0x108) = lVar2 - lVar9 >> 3;
        local_68 = *(long *)(*(long *)(param_1 + 0xb8) + 8);
        uStack_60 = *(undefined8 *)(*(long *)(param_1 + 0xb8) + 0x10);
        local_58 = (code *)&LAB_03240180;
        local_50 = &DAT_0c4b4f90;
        (**(code **)(**(long **)(local_68 + 0x18) + 0x10))(*(long **)(local_68 + 0x18),&local_68);
        if (*(code **)(local_50 + 8) != (code *)0x0) {
          (**(code **)(local_50 + 8))(local_68);
        }
        puVar8 = *(undefined8 **)(param_1 + 0xb8);
        *(undefined8 *)(param_1 + 0xb8) = 0;
        if (puVar8 != (undefined8 *)0x0) {
          *puVar8 = &PTR_LAB_0d845290;
          local_68 = puVar8[1];
          uStack_60 = puVar8[2];
          local_58 = FUN_040491a4;
          local_50 = &DAT_0c4b4f30;
          (**(code **)(**(long **)(local_68 + 0x18) + 0x10))(*(long **)(local_68 + 0x18),&local_68);
          if (*(code **)(local_50 + 8) != (code *)0x0) {
            (**(code **)(local_50 + 8))(local_68);
          }
          FUN_00005810(puVar8,0x20);
        }
        local_78 = FUN_04341f9c();
        plVar4 = local_78._8_8_;
        if (plVar4 == (long *)0x0) {
          uVar7 = 1;
          FUN_042d4298(param_1 + 0x28,"arc::TracingControllerDelegate::DidAbortTracing",0x20,1,
                       local_78);
          goto LAB_0434b894;
        }
        plVar1 = plVar4 + 1;
        lVar2 = *plVar1;
        *plVar1 = lVar2 + 1;
        FUN_042d4298(lVar2,param_1 + 0x28,"arc::TracingControllerDelegate::DidAbortTracing",0x20,1,
                     local_78);
        LOAcquire();
        lVar2 = *plVar1;
        *plVar1 = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          (**(code **)(*plVar4 + 0x10))(plVar4);
          if (plVar4[2] != 0) {
            LOAcquire();
            lVar2 = plVar4[2];
            plVar4[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_0434ba18;
          }
          pcVar5 = *(code **)(*plVar4 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar4,0x20);
          }
          else {
            (*pcVar5)(plVar4);
          }
        }
LAB_0434ba18:
        LOAcquire();
        lVar2 = *plVar1;
        *plVar1 = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          (**(code **)(*plVar4 + 0x10))(plVar4);
          if (plVar4[2] != 0) {
            LOAcquire();
            lVar2 = plVar4[2];
            plVar4[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_0434ba20;
          }
          pcVar5 = *(code **)(*plVar4 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar4,0x20);
          }
          else {
            (*pcVar5)(plVar4);
          }
        }
LAB_0434ba20:
        uVar7 = 1;
        goto LAB_0434b894;
      }
    }
    else {
      iVar3 = (**(code **)(*plVar4 + 0x10))();
      if (iVar3 != 0) goto LAB_0434b8d8;
    }
  }
  uVar7 = 0;
LAB_0434b894:
  iVar3 = _pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0xc0));
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(iVar3);
  }
  return uVar7;
}


