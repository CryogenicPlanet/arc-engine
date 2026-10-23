// ==== 0x2dba704  FUN_02dba704  (800 bytes)

/* WARNING: Type propagation algorithm not settling */

long FUN_02dba704(undefined8 *param_1,long param_2)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  undefined1 auStack_190 [64];
  undefined1 auStack_150 [64];
  char *local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  char **local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined1 auStack_b0 [64];
  long local_70;
  long local_68 [2];
  char *local_58;
  
  local_58 = (char *)((long)&MACH_HEADER.flags + 3);
  local_70 = param_2;
  if (DAT_0e4a446b != '\0') {
    local_110 = "WebContentsImpl::CreateWithOpener";
    local_c8 = (char **)0xd1db514;
    FUN_02059a48(DAT_0e4a446b,&local_58,&local_110,1,&local_c8,&local_70);
  }
  if (local_70 == 0) {
    lVar5 = 0;
  }
  else {
    lVar5 = *(long *)(local_70 + 0x140);
  }
  lVar2 = DAT_0e4a5380;
  if (DAT_0e4a5380 == 0) {
    lVar2 = FUN_0445ac8c();
  }
  lVar2 = FUN_0005cdd0(lVar2,"k/Versions/A/CoreServices",0);
  uVar3 = FUN_0350e9b8(lVar2,*param_1);
  FUN_02dbaa24(uVar3,lVar5,*(undefined1 *)(param_1 + 3));
  if (lVar5 != 0) {
    uVar3 = FUN_04150974(local_70);
    iVar1 = FUN_0385b100(local_70,0x800);
    if (iVar1 != 0) {
      local_110 = (char *)0x0;
      uStack_108 = 0;
      local_100 = 0;
      local_c8 = (char **)0x0;
      uStack_c0 = 0;
      local_b8 = 0;
      FUN_0582d480(auStack_b0,uVar3,&local_110,&local_c8,0);
      FUN_0131e2a8(lVar2 + 0x398,auStack_b0);
      FUN_0188db90(auStack_b0);
      local_58 = (char *)&local_c8;
      FUN_0147a4a0(&local_58);
      local_c8 = &local_110;
      FUN_0141a798(&local_c8);
    }
    FUN_06601444(lVar2 + 0x398,param_1 + 7);
    FUN_06601500(lVar2 + 0x398,local_70 + 0x288);
  }
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  uStack_108 = 0;
  local_110 = (char *)0x0;
  uStack_f8 = 0;
  local_100 = 0;
  FUN_005dffb8(&local_110,lVar2 + 0x4e0);
  local_110 = (char *)CONCAT44(local_110._4_4_,(uint)local_110 | *(uint *)(param_1 + 0x1a));
  FUN_005dffb8(auStack_150,&local_110);
  FUN_0131e2a8(lVar2 + 0x398,auStack_150);
  FUN_0188db90(auStack_150);
  if (*(char *)((long)param_1 + 0x19) != '\0') {
    *(undefined1 *)(lVar2 + 0x199) = 1;
  }
  if (param_1[0x17] == 0) {
    lVar5 = 0;
  }
  else {
    FUN_0638fd2c(lVar2);
    lVar5 = (**(code **)(*(long *)param_1[0x17] + 0x18))();
  }
  lVar4 = param_1[0x27];
  *(long *)(
           "/System/Library/Frameworks/ApplicationServices.framework/Versions/A/ApplicationServices"
           + lVar2 + 8) = lVar4;
  if (lVar4 == -1) {
    local_68[0] = local_70;
    local_68[1] = 0xc5;
    if (DAT_0e4a4515 != '\0') {
      local_c8 = (char **)0xd1db2fc;
      local_58 = "render_frame_host";
      FUN_02059a48(DAT_0e4a4515,local_68 + 1,&local_c8,1,&local_58,local_68);
    }
    lVar4 = 0;
    if (local_68[0] != 0) {
      lVar4 = 0;
      if (*(long *)(local_68[0] + 0x118) != 0) {
        lVar4 = *(long *)(local_68[0] + 0x118) + -0x28;
      }
    }
    local_58 = section_000000b8.sectname + 0xd;
    if (DAT_0e4a4515 != '\0') {
      local_c8 = (char **)0x0;
      FUN_0000a280(DAT_0e4a4515,&local_58,&local_c8,2);
    }
    if (lVar4 != 0) {
      *(undefined8 *)
       ("/System/Library/Frameworks/ApplicationServices.framework/Versions/A/ApplicationServices" +
       lVar2 + 8) = *(undefined8 *)
                     (
                     "/System/Library/Frameworks/ApplicationServices.framework/Versions/A/ApplicationServices"
                     + lVar4 + 8);
    }
  }
  FUN_005dffb8(auStack_190,&local_110);
  FUN_02dbab40(lVar2,param_1,auStack_190);
  FUN_0188db90(auStack_190);
  if (lVar5 != 0) {
    FUN_06789920(lVar5,lVar2);
  }
  FUN_0188db90(&local_110);
  local_c8 = (char **)0x1b;
  if (DAT_0e4a446b != '\0') {
    local_110 = (char *)0x0;
    FUN_0000a280(DAT_0e4a446b,&local_c8,&local_110,2);
  }
  return lVar2;
}


