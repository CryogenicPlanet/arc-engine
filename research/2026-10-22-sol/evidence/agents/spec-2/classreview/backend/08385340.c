// ==== 0x8385340  FUN_08385340  (516 bytes)

void FUN_08385340(undefined8 *param_1,undefined8 param_2)

{
  code ******ppppppcVar1;
  long lVar2;
  code ***pppcVar3;
  long lVar4;
  code ****ppppcVar5;
  code *****local_98;
  long lStack_90;
  undefined8 local_88;
  code ***local_80;
  long local_78;
  undefined8 local_70;
  code ***local_68 [2];
  undefined8 local_58;
  
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  FUN_00b89b80(param_1,"{\"histograms\":[",local_68);
  FUN_0838525c(&local_80,1,0x80);
  local_68[0] = (code ***)FUN_083859c0;
  if (local_78 * 8 == 0) {
    lVar4 = 0;
  }
  else {
    FUN_04f1aba0(local_80,local_80 + local_78,local_68,LZCOUNT(local_78 * 8 >> 3) << 1 ^ 0x7e,1);
    lVar4 = local_78 << 3;
  }
  local_58 = local_70;
  ppppcVar5 = (code ****)local_80;
  local_68[0] = local_80;
  local_78 = 0;
  local_70 = 0;
  local_80 = (code ***)0x0;
  local_98 = (code *****)&local_80;
  FUN_0267ce40(&local_98);
  if (lVar4 != 0) {
    pppcVar3 = *ppppcVar5;
    thunk_FUN_000b8fc0(param_1,"",0);
    local_98 = (code *****)0x0;
    lStack_90 = 0;
    local_88 = 0;
    FUN_0837ff98(pppcVar3,&local_98,param_2);
    ppppppcVar1 = (code ******)local_98;
    if (-1 < (long)local_88._7_1_) {
      ppppppcVar1 = &local_98;
    }
    lVar2 = lStack_90;
    if (-1 < (long)local_88) {
      lVar2 = (long)local_88._7_1_;
    }
    thunk_FUN_000b8fc0(param_1,ppppppcVar1,lVar2);
    if ((long)local_88 < 0) {
      thunk_FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
    }
    lVar4 = lVar4 + -8;
    if (lVar4 != 0) {
      do {
        ppppcVar5 = ppppcVar5 + 1;
        pppcVar3 = *ppppcVar5;
        thunk_FUN_000b8fc0(param_1,",",1);
        local_98 = (code *****)0x0;
        lStack_90 = 0;
        local_88 = 0;
        FUN_0837ff98(pppcVar3,&local_98,param_2);
        ppppppcVar1 = (code ******)local_98;
        if (-1 < (long)local_88._7_1_) {
          ppppppcVar1 = &local_98;
        }
        lVar2 = lStack_90;
        if (-1 < (long)local_88) {
          lVar2 = (long)local_88._7_1_;
        }
        thunk_FUN_000b8fc0(param_1,ppppppcVar1,lVar2);
        if ((long)local_88 < 0) {
          thunk_FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
        }
        lVar4 = lVar4 + -8;
      } while (lVar4 != 0);
    }
  }
  local_98 = (code *****)local_68;
  FUN_0267ce40(&local_98);
  thunk_FUN_000b8fc0(param_1,"]}",2);
  return;
}


