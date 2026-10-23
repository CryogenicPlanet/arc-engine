// ==== 0x4b37ee8  FUN_04b37ee8  (144 bytes)

void FUN_04b37ee8(void)

{
  code *pcVar1;
  undefined1 *local_b0;
  long lStack_a8;
  char local_99;
  undefined1 *local_98;
  long lStack_90;
  char *local_68;
  undefined8 uStack_60;
  undefined1 auStack_38 [24];
  
  local_68 = "Attempting to fetch value instead of handling error ";
  uStack_60 = 0x34;
  FUN_04b37ea8(&local_b0);
  local_98 = local_b0;
  if (-1 < (long)local_99) {
    local_98 = (undefined1 *)&local_b0;
  }
  lStack_90 = lStack_a8;
  if (-1 < local_99) {
    lStack_90 = (long)local_99;
  }
  FUN_028b5a88(auStack_38,&local_68,&local_98);
  (*(code *)PTR_FUN_0e2b8230)
            (3,"../../third_party/abseil-cpp/absl/status/statusor.cc",99,auStack_38);
  FUN_09c8a2e0(auStack_38);
  FUN_09c8a2e0(&local_b0);
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4b37f78);
  (*pcVar1)();
}


