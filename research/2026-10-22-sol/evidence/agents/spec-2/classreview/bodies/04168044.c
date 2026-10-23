// ==== 0x4168044  RTCEncodedImage::setCaptureTimeMs:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void RTCEncodedImage::setCaptureTimeMs_(ID param_1,SEL param_2,long_long param_3)

{
  *(long_long *)(param_1 + 0x20) = param_3;
  return;
}


