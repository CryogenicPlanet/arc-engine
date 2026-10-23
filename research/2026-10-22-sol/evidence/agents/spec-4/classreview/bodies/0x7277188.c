// ==== 0x7277188  FUN_07277188  (164 bytes)

void FUN_07277188(undefined8 param_1)

{
  bool bVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  undefined8 *local_50;
  undefined8 local_48;
  undefined8 local_40;
  undefined1 *local_38;
  
  local_50 = (undefined8 *)0x0;
  local_48 = 0;
  local_40 = 0;
  FUN_0727722c(&local_50);
  puVar2 = local_50;
  while (puVar2 != &local_48) {
    FUN_07276fc0(param_1,*(undefined4 *)(puVar2 + 4),1);
    puVar3 = (undefined8 *)puVar2[1];
    if ((undefined8 *)puVar2[1] == (undefined8 *)0x0) {
      do {
        puVar3 = (undefined8 *)puVar2[2];
        bVar1 = puVar2 != (undefined8 *)*puVar3;
        puVar2 = puVar3;
      } while (bVar1);
    }
    else {
      do {
        puVar2 = puVar3;
        puVar3 = (undefined8 *)*puVar2;
      } while (puVar3 != (undefined8 *)0x0);
    }
  }
  local_38 = (undefined1 *)&local_50;
  FUN_06ae4a24(&local_38,local_48);
  return;
}


