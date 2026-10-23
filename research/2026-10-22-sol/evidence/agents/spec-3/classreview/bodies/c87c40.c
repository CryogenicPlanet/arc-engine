// ==== 0xc87c40  FUN_00c87c40  (208 bytes)

undefined8 * FUN_00c87c40(undefined8 *param_1,undefined8 ****param_2,ulong param_3)

{
  code *pcVar1;
  undefined8 ****ppppuVar2;
  undefined8 ***local_58;
  ulong uStack_50;
  undefined8 local_48;
  
  if (0x7fffffffffffffee < param_3) {
                    /* WARNING: Subroutine does not return */
    FUN_04b089c0();
  }
  if (param_3 < 0x17) {
    local_48 = CONCAT17((char)param_3,(undefined7)local_48);
    ppppuVar2 = &local_58;
  }
  else {
    ppppuVar2 = (undefined8 ****)FUN_00008790((param_3 & 0x7ffffffffffffff0) + 0x10);
    local_48 = (param_3 & 0x7ffffffffffffff0) + 0x8000000000000010;
    local_58 = ppppuVar2;
    uStack_50 = param_3;
  }
  if ((ppppuVar2 <= (undefined8 ****)((long)ppppuVar2 + param_3)) &&
     ((undefined8 ****)((long)ppppuVar2 + param_3) <= param_2 || param_2 < ppppuVar2)) {
    _memmove(ppppuVar2,param_2,param_3);
    *(undefined1 *)((long)ppppuVar2 + param_3) = 0;
    param_1[1] = uStack_50;
    *param_1 = local_58;
    param_1[2] = local_48;
    *(undefined1 *)(param_1 + 3) = 4;
    return param_1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0xc87d0c);
  (*pcVar1)();
}


