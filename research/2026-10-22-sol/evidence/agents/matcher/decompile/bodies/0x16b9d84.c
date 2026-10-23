// ==== 0x16b9d84  FUN_016b9d84  (1092 bytes)

void FUN_016b9d84(long param_1,long param_2,uint param_3)

{
  undefined8 uVar1;
  code *pcVar2;
  int iVar3;
  uint uVar4;
  ulong uVar5;
  undefined4 *puVar6;
  undefined8 *puVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  undefined8 *puVar11;
  undefined8 uVar12;
  undefined8 *puVar13;
  ulong uVar14;
  int *piVar15;
  undefined8 *puVar16;
  undefined8 local_188;
  char *local_180 [2];
  undefined1 auStack_170 [288];
  
  uVar5 = FUN_018efea8(param_2);
  if (uVar5 != 0) {
    if (*(long *)(param_1 + 8) == 0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba144);
      (*pcVar2)();
    }
    iVar3 = FUN_00027de0(*(long *)(param_1 + 8),param_2);
    if (iVar3 != -1) {
      if ((param_3 & 1) != 0) {
        if (*(long *)(param_1 + 8) == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba194);
          (*pcVar2)();
        }
        lVar8 = *(long *)(*(long *)(param_1 + 8) + 0x90);
        lVar9 = 0;
        if (lVar8 != 0) {
          lVar9 = *(long *)(lVar8 + 0x40);
        }
        if (param_2 == lVar9) {
          local_188 = 0x9d;
          if (DAT_0e4a44ed != '\0') {
            local_180[0] = "BrowserUiController::UpdateToolbar";
            FUN_0000a280(DAT_0e4a44ed,&local_188,local_180,1);
          }
          if (*(long *)(param_1 + 0x10) == 0) {
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba1c0);
            (*pcVar2)();
          }
          if (DAT_0e4a3090 == 0) {
            uVar4 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
          }
          else {
            uVar4 = FUN_083a6f28(DAT_0e4a3090,"../../arc/browser/ui/window.cc",0x1e);
          }
          if (0 < (int)uVar4) {
            FUN_00f195d8(local_180,"../../arc/browser/ui/window.cc",0x1fd,0xffffffff);
            FUN_00040060(auStack_170,
                         "virtual void arc::browser::ui::Window::UpdateToolbar(content::WebContents *)"
                         ,0x4c);
            FUN_00f1950c(local_180);
          }
          local_188 = 0x9d;
          if (DAT_0e4a44ed != '\0') {
            local_180[0] = (char *)0x0;
            FUN_0000a280(DAT_0e4a44ed,&local_188,local_180,2);
          }
        }
        else {
          if (*(long *)(param_1 + 0x10) == 0) {
                    /* WARNING: Does not return */
            pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba1a0);
            (*pcVar2)();
          }
          if (DAT_0e4a3090 == 0) {
            uVar4 = -DAT_0e4a3078 | -DAT_0e4a3078 >> 0x1f;
          }
          else {
            uVar4 = FUN_083a6f28(DAT_0e4a3090,"../../arc/browser/ui/window.cc",0x1e);
          }
          if (0 < (int)uVar4) {
            FUN_00f195d8(local_180,"../../arc/browser/ui/window.cc",0x20a,0xffffffff);
            FUN_00040060(auStack_170,
                         "virtual void arc::browser::ui::Window::ResetToolbarTabState(content::WebContents *)"
                         ,0x53);
            FUN_00f1950c(local_180);
          }
        }
        param_3 = param_3 & 0xfffffffe;
      }
      if ((param_3 >> 2 & 1) == 0) {
        if (param_3 == 0) {
          return;
        }
      }
      else {
        if (*(long *)(param_1 + 8) == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba150);
          (*pcVar2)();
        }
        FUN_016837a0(*(long *)(param_1 + 8),uVar5,1);
        plVar10 = *(long **)(uVar5 + 0x318);
        if (plVar10 != (long *)0x0) {
          do {
            while ((undefined **)plVar10[4] < (undefined **)0xdf35d21) {
              if ((undefined **)plVar10[4] == &PTR_s_TabUIHelper_kDataKeyImpl_0df35d20) {
                if (plVar10 != (long *)(uVar5 + 0x318)) {
                  uVar14 = plVar10[5];
                  if ((uVar14 & 0xfffffffc00000000) == DAT_0e2f2208) {
                    FUN_0000ffa8(uVar14);
                  }
                  goto LAB_016b9ee8;
                }
                goto LAB_016b9eb4;
              }
              plVar10 = (long *)plVar10[1];
              if (plVar10 == (long *)0x0) goto LAB_016b9eb4;
            }
            plVar10 = (long *)*plVar10;
          } while (plVar10 != (long *)0x0);
        }
LAB_016b9eb4:
        uVar14 = 0;
LAB_016b9ee8:
        if ((uVar14 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0001fee4(uVar14);
        }
        FUN_00c1e45c(uVar14 + 0x68);
      }
      puVar11 = *(undefined8 **)(param_1 + 0x28);
      puVar13 = (undefined8 *)(param_1 + 0x28);
      while (puVar16 = puVar13, puVar11 != (undefined8 *)0x0) {
        while (puVar7 = puVar11, puVar13 = puVar7, (ulong)puVar7[4] <= uVar5) {
          if (uVar5 <= (ulong)puVar7[4]) goto LAB_016b9f30;
          puVar11 = (undefined8 *)puVar7[1];
          if ((undefined8 *)puVar7[1] == (undefined8 *)0x0) {
            puVar16 = puVar7 + 1;
            goto LAB_016ba040;
          }
        }
        puVar11 = (undefined8 *)*puVar7;
      }
LAB_016ba040:
      puVar7 = (undefined8 *)FUN_00008790(0x30);
      if ((puVar7 == (undefined8 *)0x0) || (puVar7 == (undefined8 *)0xffffffffffffffe0)) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x16ba158);
        (*pcVar2)();
      }
      puVar7[4] = uVar5;
      *(undefined4 *)(puVar7 + 5) = 0;
      *puVar7 = 0;
      puVar7[1] = 0;
      puVar7[2] = puVar13;
      *puVar16 = puVar7;
      puVar13 = puVar7;
      if (**(long **)(param_1 + 0x20) != 0) {
        *(long *)(param_1 + 0x20) = **(long **)(param_1 + 0x20);
        puVar13 = (undefined8 *)*puVar16;
      }
      FUN_00067d9c(*(undefined8 *)(param_1 + 0x28),puVar13);
      *(long *)(param_1 + 0x30) = *(long *)(param_1 + 0x30) + 1;
LAB_016b9f30:
      *(uint *)(puVar7 + 5) = *(uint *)(puVar7 + 5) | param_3;
      if ((*(long *)(param_1 + 0x60) == 0) || (**(int **)(param_1 + 0x58) == 1)) {
        uVar1 = 0;
        if (*(char *)(param_1 + 0x38) == '\0') {
          uVar1 = 200000;
        }
        plVar10 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
        if (((undefined8 *)*plVar10 == (undefined8 *)0x0) ||
           (puVar13 = *(undefined8 **)*plVar10, puVar13 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba15c);
          (*pcVar2)();
        }
        FUN_00029e40(local_180,"ScheduleUIUpdate",
                     "../../chrome/browser/ui/browser_ui_controller/browser_ui_controller.cc",0xb2);
        piVar15 = *(int **)(param_1 + 0x58);
        if ((piVar15 != (int *)0x0) && (iVar3 = *piVar15, *piVar15 = iVar3 + 1, iVar3 == 0x7fffffff)
           ) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(0,0x16ba188);
          (*pcVar2)();
        }
        uVar12 = *(undefined8 *)(param_1 + 0x60);
        puVar6 = (undefined4 *)FUN_00008790(0x40);
        *puVar6 = 1;
        *(code **)(puVar6 + 2) = FUN_00dfc980;
        *(code **)(puVar6 + 4) = FUN_039a31c0;
        *(undefined1 **)(puVar6 + 6) = &LAB_04074f00;
        *(code **)(puVar6 + 8) = FUN_01683318;
        *(undefined8 *)(puVar6 + 10) = 0;
        *(int **)(puVar6 + 0xc) = piVar15;
        *(undefined8 *)(puVar6 + 0xe) = uVar12;
        (**(code **)*puVar13)(puVar13,local_180,puVar6,uVar1);
      }
    }
  }
  return;
}


