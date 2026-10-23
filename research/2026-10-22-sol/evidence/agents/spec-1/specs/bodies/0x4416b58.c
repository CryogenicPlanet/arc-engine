// ==== 0x4416b58  ArcWebContentsThumbnailsGenerator::hash  (56 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcWebContentsThumbnailsGenerator::hash(ID param_1,SEL param_2)

{
  unsigned_long_long uVar1;
  undefined8 local_18;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    local_18 = *(undefined8 *)(param_1 + 0x10);
    uVar1 = FUN_00018f3c(&local_18,8);
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}


