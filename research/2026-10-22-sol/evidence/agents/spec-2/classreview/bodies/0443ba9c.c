// ==== 0x443ba9c  ArcErrorStatus::setError_status:  (12 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcErrorStatus::setError_status_(ID param_1,SEL param_2,ErrorStatus param_3)

{
  *(undefined8 *)(param_1 + 8) = *(undefined8 *)param_3;
  return;
}


