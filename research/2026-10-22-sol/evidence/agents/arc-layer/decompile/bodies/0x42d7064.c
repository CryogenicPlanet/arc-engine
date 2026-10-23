// ==== 0x42d7064  FUN_042d7064  (280 bytes)

void FUN_042d7064(undefined8 param_1,long param_2,long param_3)

{
  undefined8 uVar1;
  undefined1 auStack_1ec [28];
  undefined8 local_1d0;
  undefined8 uStack_1c8;
  undefined4 local_1c0;
  undefined8 local_1b8;
  undefined8 local_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined4 local_50;
  undefined8 local_48;
  
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_a8 = 0;
  uStack_b0 = 0;
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  uStack_f8 = 0;
  local_100 = 0;
  uStack_e8 = 0;
  uStack_f0 = 0;
  uStack_118 = 0;
  local_120 = 0;
  uStack_108 = 0;
  uStack_110 = 0;
  uStack_138 = 0;
  local_140 = 0;
  uStack_128 = 0;
  uStack_130 = 0;
  uStack_158 = 0;
  local_160 = 0;
  uStack_148 = 0;
  uStack_150 = 0;
  uStack_178 = 0;
  local_180 = 0;
  uStack_168 = 0;
  uStack_170 = 0;
  uStack_198 = 0;
  local_1a0 = 0;
  uStack_188 = 0;
  uStack_190 = 0;
  uStack_1a8 = 0;
  local_1b0 = 0;
  uVar1 = *(undefined8 *)(param_2 + 0x88);
  FUN_00029e40(&local_1d0,"CreateWebContents",
               "../../arc/browser/browser_context/browser_context_impl.cc",0x76b);
  uStack_58 = uStack_1c8;
  local_60 = local_1d0;
  local_50 = local_1c0;
  local_48 = local_1b8;
  FUN_02728fe0(&local_1b0,uVar1,0,&local_60);
  uVar1 = 0;
  local_100 = CONCAT71(local_100._1_7_,*(undefined1 *)(param_3 + 10));
  uStack_68._0_5_ = CONCAT14(*(undefined1 *)(param_3 + 1),(undefined4)uStack_68);
  if (local_1a0._4_4_ != -2) {
    uVar1 = FUN_0012aa58(local_1a0,0);
  }
  uVar1 = FUN_02dba704(&local_1b0,uVar1);
  FUN_04345a80(auStack_1ec,param_3);
  FUN_04346528(param_1,uVar1,auStack_1ec);
  FUN_026f6980(&local_1b0);
  return;
}


