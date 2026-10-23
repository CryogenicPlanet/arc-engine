// ==== 0x4377a08  FUN_04377a08  (100 bytes)

void FUN_04377a08(undefined8 *param_1,long param_2)

{
  undefined8 uVar1;
  
  if (*(long *)(param_2 + 0x130) == 0) {
    uVar1 = FUN_04b37104(0x25,"managed webcontents is null",0x1b,0x729,
                         "../../arc/browser/web_contents/web_contents_impl.cc");
  }
  else {
    (**(code **)(**(long **)(*(long *)(*(long *)(param_2 + 0x130) + 0x8d8) + 0x118) + 0x1b8))();
    uVar1 = 1;
  }
  *param_1 = uVar1;
  return;
}


