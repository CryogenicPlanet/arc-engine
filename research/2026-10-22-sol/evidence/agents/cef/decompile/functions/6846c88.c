// ==== 0x6846c88  FUN_06846c88  (572 bytes)

void FUN_06846c88(long param_1,undefined8 *param_2)

{
  code *pcVar1;
  uint *puVar2;
  long *plVar3;
  ulong local_a8;
  undefined **local_a0;
  undefined8 local_98;
  int local_90;
  undefined8 local_8c;
  uint local_84;
  ulong local_80;
  undefined **local_78;
  undefined8 local_70;
  int local_68;
  undefined8 local_64;
  uint local_5c;
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [32];
  
  if ((undefined **)*param_2 == &PTR_LAB_0dd2f188) {
    FUN_0837a430(auStack_50,"../../content/browser/renderer_host/render_widget_host_view_mac.mm",
                 0x5b2);
    FUN_08375df4(auStack_58,
                 "!static_cast<RenderWidgetHostViewBase*>(view) ->IsRenderWidgetHostViewChildFrame()"
                 ,0x98,auStack_50);
    FUN_0837612c(auStack_58);
  }
  CATransaction::begin((ID)&_OBJC_CLASS___CATransaction,"begin");
  CATransaction::setValue_forKey_
            ((ID)&_OBJC_CLASS___CATransaction,"setValue:forKey:",PTR____kCFBooleanTrue_0d824a00,
             *(undefined8 *)PTR__kCATransactionDisableActions_0d8254e8);
  FUN_066ad888(param_1,param_2);
  puVar2 = (uint *)FUN_041670b0(param_2[0x3e]);
  if (puVar2 != (uint *)0x0) {
    plVar3 = *(long **)(param_1 + 0x438);
    local_78 = &PTR_FUN_0dd281b0;
    local_70 = 0;
    local_80 = (ulong)*puVar2;
    local_68 = 0;
    if (puVar2[6] != 0) {
      local_68 = FUN_04d8dff8(puVar2[6]);
    }
    local_64 = *(undefined8 *)(puVar2 + 7);
    local_5c = puVar2[9];
    (**(code **)(*plVar3 + 0x38))(plVar3,&local_80);
    if (local_68 != 0) {
      FUN_04d8e080();
    }
    local_78 = &PTR_FUN_0dd281b0;
    if (local_70._4_1_ != '\0') {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x6846ee0);
      (*pcVar1)();
    }
    if ((int)local_70 != 0) {
      FUN_00433750();
      local_70 = local_70 & 0xffffffff00000000;
    }
  }
  FUN_04167a60(*(undefined8 *)(param_1 + 0x1f0),param_2[0x3e]);
  puVar2 = (uint *)FUN_041670b0(*(undefined8 *)(param_1 + 0x1f0));
  if (puVar2 != (uint *)0x0) {
    plVar3 = *(long **)(param_1 + 0x438);
    local_a0 = &PTR_FUN_0dd281b0;
    local_98 = 0;
    local_a8 = (ulong)*puVar2;
    local_90 = 0;
    if (puVar2[6] != 0) {
      local_90 = FUN_04d8dff8(puVar2[6]);
    }
    local_8c = *(undefined8 *)(puVar2 + 7);
    local_84 = puVar2[9];
    (**(code **)(*plVar3 + 0x38))(plVar3,&local_a8);
    if (local_90 != 0) {
      FUN_04d8e080();
    }
    local_a0 = &PTR_FUN_0dd281b0;
    if (local_98._4_1_ != '\0') {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x6846ef8);
      (*pcVar1)();
    }
    if ((int)local_98 != 0) {
      FUN_00433750();
      local_98 = local_98 & 0xffffffff00000000;
    }
    if (*(int *)(param_1 + 0x4f0) != *(int *)(param_1 + 0x4ec)) {
      *(int *)(param_1 + 0x4ec) = *(int *)(param_1 + 0x4f0);
      (**(code **)(**(long **)(param_1 + 0x438) + 0x40))();
    }
  }
  CATransaction::commit((ID)&_OBJC_CLASS___CATransaction,"commit");
  return;
}


