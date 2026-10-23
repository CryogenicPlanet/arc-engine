// ==== 0x443ba90  ArcErrorStatus::error_status  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ErrorStatus ArcErrorStatus::error_status(ID param_1,SEL param_2)

{
  undefined8 *in_x8;
  
  *in_x8 = *(undefined8 *)(param_1 + 8);
  return (ErrorStatus)param_1;
}


