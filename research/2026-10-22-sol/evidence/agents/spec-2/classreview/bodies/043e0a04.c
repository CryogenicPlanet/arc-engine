// ==== 0x43e0a04  RTCVideoEncoderSettings::setMode:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void RTCVideoEncoderSettings::setMode_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  *(unsigned_long_long *)(param_1 + 0x28) = param_3;
  return;
}


