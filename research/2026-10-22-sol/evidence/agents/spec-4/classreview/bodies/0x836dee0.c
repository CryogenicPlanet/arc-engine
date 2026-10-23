// ==== 0x836dee0  FUN_0836dee0  (164 bytes)

undefined8 FUN_0836dee0(long *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  long lVar2;
  undefined1 auStack_170 [256];
  undefined8 local_70 [2];
  undefined8 uStack_5f;
  undefined8 local_50 [2];
  undefined8 uStack_3f;
  
  lVar2 = *param_1;
  FUN_09273058(local_70);
  FUN_0836df84(auStack_170,param_4);
  if (*(long *)(lVar2 + 0x130) != 0) {
    local_50[0] = local_70[0];
    uStack_3f = uStack_5f;
    lVar2 = FUN_09564ac8(local_50);
    if (((lVar2 != 0) && (*(char *)(lVar2 + 0x18) != '\0')) &&
       (lVar2 = FUN_095a0160(lVar2,param_3), lVar2 != 0)) {
      uVar1 = FUN_0959d360(lVar2,auStack_170);
      goto LAB_0836df54;
    }
  }
  uVar1 = 0;
LAB_0836df54:
  FUN_0926bce4(auStack_170);
  return uVar1;
}


