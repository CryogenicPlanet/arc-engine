// ==== 0x435c51c  FUN_0435c51c  (456 bytes)

void FUN_0435c51c(long param_1,undefined8 param_2,undefined1 *param_3,ulong param_4)

{
  undefined8 *******pppppppuVar1;
  code *pcVar2;
  int iVar3;
  long *plVar4;
  undefined4 *puVar5;
  undefined8 uVar6;
  undefined8 *puVar7;
  int *piVar8;
  ulong local_70;
  undefined8 ******local_68;
  ulong uStack_60;
  ulong local_58;
  undefined8 ******local_50;
  ulong uStack_48;
  undefined8 local_40;
  
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
    if (param_4 < 0x800001) {
      if (0x7fffffffffffffee < param_4) {
                    /* WARNING: Subroutine does not return */
        FUN_04b089c0();
      }
      if (param_4 < 0x17) {
        local_40 = CONCAT17((char)param_4,(undefined7)local_40);
        pppppppuVar1 = &local_50;
      }
      else {
        local_50 = (undefined8 ******)FUN_00008790((param_4 & 0x7ffffffffffffff0) + 0x10);
        local_40 = (param_4 & 0x7ffffffffffffff0) + 0x8000000000000010;
        pppppppuVar1 = (undefined8 *******)local_50;
        uStack_48 = param_4;
      }
      for (; param_4 != 0; param_4 = param_4 - 1) {
        *(undefined1 *)pppppppuVar1 = *param_3;
        pppppppuVar1 = (undefined8 *******)((long)pppppppuVar1 + 1);
        param_3 = param_3 + 1;
      }
      *(undefined1 *)pppppppuVar1 = 0;
      uStack_60 = uStack_48;
      local_68 = local_50;
      local_58 = local_40;
      local_70 = 1;
      FUN_0435c8fc(param_1,&local_70);
      if (local_70 == 1) {
        if ((long)local_58 < 0) {
          FUN_00005810(local_68,local_58 & 0x7fffffffffffffff);
        }
      }
      else if ((local_70 & 1) == 0) {
        FUN_04b35898();
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x30) = 1;
      plVar4 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
      if (((undefined8 *)*plVar4 == (undefined8 *)0x0) ||
         (puVar7 = *(undefined8 **)*plVar4, puVar7 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x435c6e0);
        (*pcVar2)();
      }
      FUN_00029e40(&local_50,"DispatchProtocolMessage",
                   "../../arc/browser/web_contents/devtools_impl.cc",0x5a);
      piVar8 = *(int **)(param_1 + 0x38);
      if ((piVar8 != (int *)0x0) && (iVar3 = *piVar8, *piVar8 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(0,0x435c6ec);
        (*pcVar2)();
      }
      uVar6 = *(undefined8 *)(param_1 + 0x40);
      puVar5 = (undefined4 *)FUN_00008790(0x40);
      *puVar5 = 1;
      *(undefined1 **)(puVar5 + 2) = &LAB_040c5ad0;
      *(code **)(puVar5 + 4) = FUN_040a6300;
      *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
      *(code **)(puVar5 + 8) = FUN_0435c74c;
      *(undefined8 *)(puVar5 + 10) = 0;
      *(int **)(puVar5 + 0xc) = piVar8;
      *(undefined8 *)(puVar5 + 0xe) = uVar6;
      (**(code **)*puVar7)(puVar7,&local_50,puVar5,0);
    }
  }
  return;
}


