// ==== 0x43ded78  ArcHSLAMatrix::initWithMatrix:  (132 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcHSLAMatrix::initWithMatrix_(ID param_1,SEL param_2,float *param_3)

{
  code *pcVar1;
  int iVar2;
  ID IVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcHSLAMatrix;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    iVar2 = FUN_00122014(param_3,0x50,4);
    if (iVar2 != 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x43dedfc);
      (*pcVar1)();
    }
    uVar4 = *(undefined8 *)param_3;
    uVar6 = *(undefined8 *)(param_3 + 6);
    uVar5 = *(undefined8 *)(param_3 + 4);
    uVar8 = *(undefined8 *)(param_3 + 10);
    uVar7 = *(undefined8 *)(param_3 + 8);
    uVar10 = *(undefined8 *)(param_3 + 0xe);
    uVar9 = *(undefined8 *)(param_3 + 0xc);
    uVar12 = *(undefined8 *)(param_3 + 0x12);
    uVar11 = *(undefined8 *)(param_3 + 0x10);
    *(undefined8 *)(IVar3 + 0x10) = *(undefined8 *)(param_3 + 2);
    *(undefined8 *)(IVar3 + 8) = uVar4;
    *(undefined8 *)(IVar3 + 0x20) = uVar6;
    *(undefined8 *)(IVar3 + 0x18) = uVar5;
    *(undefined8 *)(IVar3 + 0x30) = uVar8;
    *(undefined8 *)(IVar3 + 0x28) = uVar7;
    *(undefined8 *)(IVar3 + 0x40) = uVar10;
    *(undefined8 *)(IVar3 + 0x38) = uVar9;
    *(undefined8 *)(IVar3 + 0x50) = uVar12;
    *(undefined8 *)(IVar3 + 0x48) = uVar11;
  }
  return IVar3;
}


