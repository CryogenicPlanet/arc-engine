// ==== 0x44077b4  ArcMemoryDumpProvider::initWithName:dataProvider:  (468 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcMemoryDumpProvider::initWithName_dataProvider_
             (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  ID IVar1;
  ID *pIVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 local_90;
  undefined8 uStack_88;
  ulong local_80;
  ID local_78;
  class_t *pcStack_70;
  ID *local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_70 = &objc::class_t::ArcMemoryDumpProvider;
  local_78 = param_1;
  IVar1 = NSObject::init((ID)&local_78,"init",param_3,param_4,(ulong)param_5);
  if (IVar1 != 0) {
    local_90 = 0;
    uStack_88 = 0;
    local_80 = 0;
    if (param_3 == 0) {
      local_90 = 0;
      uStack_88 = 0;
      local_80 = 0;
    }
    else {
      FUN_01b356b8(&local_90,param_3);
    }
    pIVar2 = (ID *)FUN_00008790(8);
    *pIVar2 = param_4;
    uVar3 = FUN_00008790(8);
    uStack_60 = 0;
    local_58 = FUN_04407b5c;
    local_50 = &PTR_FUN_0df6bbe0;
    local_68 = pIVar2;
    FUN_0432b4b8(uVar3,&local_90,&local_68);
    if ((code *)local_50[1] != (code *)0x0) {
      (*(code *)local_50[1])(local_68);
    }
    puVar4 = *(undefined8 **)(IVar1 + 8);
    *(undefined8 *)(IVar1 + 8) = uVar3;
    if (puVar4 != (undefined8 *)0x0) {
      puVar5 = (undefined8 *)*puVar4;
      *puVar4 = 0;
      if (puVar5 != (undefined8 *)0x0) {
        *puVar5 = &PTR_FUN_0da7e4c0;
        uVar3 = FUN_00f80684();
        FUN_01e3d3f8(uVar3,puVar5,0);
        if (*(char *)((long)puVar5 + 0x3f) < '\0') {
          FUN_00005810(puVar5[5],puVar5[7] & 0x7fffffffffffffff);
        }
        if (*(code **)(puVar5[4] + 8) != (code *)0x0) {
          (**(code **)(puVar5[4] + 8))(puVar5[1]);
        }
        FUN_00005810(puVar5,0x40);
      }
      FUN_00005810(puVar4,8);
    }
    if ((long)local_80 < 0) {
      FUN_00005810(local_90,local_80 & 0x7fffffffffffffff);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return IVar1;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


