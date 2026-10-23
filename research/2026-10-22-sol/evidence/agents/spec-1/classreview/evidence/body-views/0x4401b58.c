// ==== 0x4401b58  objc::Categories::ArcColor_Init_::initWithR:g:b:a:  (56 bytes)

/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcColor_Init_::initWithR_g_b_a_
             (ID param_1,SEL param_2,unsigned_char param_3,unsigned_char param_4,
             unsigned_char param_5,unsigned_char param_6)

{
  ID IVar1;
  unsigned_char local_14;
  unsigned_char local_13;
  unsigned_char local_12;
  unsigned_char local_11;
  
  local_14 = param_3;
  local_13 = param_4;
  local_12 = param_5;
  local_11 = param_6;
  IVar1 = _objc_msgSend(param_1,"initWithColor:",&local_14);
  return IVar1;
}


