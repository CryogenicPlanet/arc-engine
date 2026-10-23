// ==== 0x4402ad8  ArcHistogramSamples::initWithPeriodicHistogramSample:  (2040 bytes)

/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcHistogramSamples::initWithPeriodicHistogramSample_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID IVar2;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcHistogramSamples;
  local_40 = param_1;
  IVar1 = NSObject::init((ID)&local_40,"init");
  if (IVar1 != 0) {
    if (*(char *)((long)param_3 + 0x1c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x14));
    }
    *(ID *)(IVar1 + 8) = IVar2;
    if (*(char *)((long)param_3 + 0x10) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 8));
    }
    *(ID *)(IVar1 + 0x10) = IVar2;
    if (*(char *)((long)param_3 + 0x28) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x20));
    }
    *(ID *)(IVar1 + 0x18) = IVar2;
    if (*(char *)((long)param_3 + 0x34) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x2c));
    }
    *(ID *)(IVar1 + 0x20) = IVar2;
    if (*(char *)((long)param_3 + 0x40) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x38));
    }
    *(ID *)(IVar1 + 0x28) = IVar2;
    if (*(char *)((long)param_3 + 0x4c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x44));
    }
    *(ID *)(IVar1 + 0x30) = IVar2;
    if (*(char *)((long)param_3 + 0x58) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x50));
    }
    *(ID *)(IVar1 + 0x38) = IVar2;
    if (*(char *)((long)param_3 + 100) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x5c));
    }
    *(ID *)(IVar1 + 0x48) = IVar2;
    if (*(char *)((long)param_3 + 0x70) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x68));
    }
    *(ID *)(IVar1 + 0x40) = IVar2;
    if (*(char *)((long)param_3 + 0x7c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x74));
    }
    *(ID *)(IVar1 + 0x50) = IVar2;
    if (*(char *)((long)param_3 + 0x88) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x80));
    }
    *(ID *)(IVar1 + 0x58) = IVar2;
    if (*(char *)((long)param_3 + 0x94) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x8c));
    }
    *(ID *)(IVar1 + 0x60) = IVar2;
    if (*(char *)((long)param_3 + 0xa0) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x98));
    }
    *(ID *)(IVar1 + 0x68) = IVar2;
    if (*(char *)((long)param_3 + 0xac) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xa4));
    }
    *(ID *)(IVar1 + 0x70) = IVar2;
    if (*(char *)((long)param_3 + 0xb8) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xb0));
    }
    *(ID *)(IVar1 + 0x78) = IVar2;
    if (*(char *)((long)param_3 + 0xc4) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xbc));
    }
    *(ID *)(IVar1 + 0x80) = IVar2;
    if (*(char *)((long)param_3 + 0xd0) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 200));
    }
    *(ID *)(IVar1 + 0x88) = IVar2;
    if (*(char *)((long)param_3 + 0xdc) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xd4));
    }
    *(ID *)(IVar1 + 0x90) = IVar2;
    if (*(char *)((long)param_3 + 0xe8) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xe0));
    }
    *(ID *)(IVar1 + 0x98) = IVar2;
    if (*(char *)((long)param_3 + 0xf4) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xec));
    }
    *(ID *)(IVar1 + 0xa0) = IVar2;
    if (*(char *)((long)param_3 + 0x100) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0xf8));
    }
    *(ID *)(IVar1 + 0xa8) = IVar2;
    if (*(char *)((long)param_3 + 0x10c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x104));
    }
    *(ID *)(IVar1 + 0xb0) = IVar2;
    if (*(char *)((long)param_3 + 0x118) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x110));
    }
    *(ID *)(IVar1 + 0xb8) = IVar2;
    if (*(char *)((long)param_3 + 0x130) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x128));
    }
    *(ID *)(IVar1 + 0xc0) = IVar2;
    if (*(char *)((long)param_3 + 0x124) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x11c));
    }
    *(ID *)(IVar1 + 200) = IVar2;
    if (*(char *)((long)param_3 + 0x184) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x17c));
    }
    *(ID *)(IVar1 + 0xd0) = IVar2;
    if (*(char *)((long)param_3 + 400) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x188));
    }
    *(ID *)(IVar1 + 0xd8) = IVar2;
    if (*(char *)((long)param_3 + 0x19c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x194));
    }
    *(ID *)(IVar1 + 0xe0) = IVar2;
    if (*(char *)((long)param_3 + 0x1a8) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x1a0));
    }
    *(ID *)(IVar1 + 0xe8) = IVar2;
    if (*(char *)((long)param_3 + 0x1b4) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x1ac));
    }
    *(ID *)(IVar1 + 0xf0) = IVar2;
    if (*(char *)((long)param_3 + 0x1c0) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x1b8));
    }
    *(ID *)(IVar1 + 0xf8) = IVar2;
    if (*(char *)((long)param_3 + 0x13c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x134));
    }
    *(ID *)(IVar1 + 0x100) = IVar2;
    if (*(char *)((long)param_3 + 0x148) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x140));
    }
    *(ID *)(IVar1 + 0x108) = IVar2;
    if (*(char *)((long)param_3 + 0x160) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x158));
    }
    *(ID *)(IVar1 + 0x110) = IVar2;
    if (*(char *)((long)param_3 + 0x16c) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x164));
    }
    *(ID *)(IVar1 + 0x118) = IVar2;
    if (*(char *)((long)param_3 + 0x178) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x170));
    }
    *(ID *)(IVar1 + 0x120) = IVar2;
    if (*(char *)((long)param_3 + 0x154) == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcHistogramSample);
      IVar2 = ArcHistogramSample::initWithHistogramSample_
                        (IVar2,"initWithHistogramSample:",
                         (const_HistogramSample__)((long)param_3 + 0x14c));
    }
    *(ID *)(IVar1 + 0x128) = IVar2;
  }
  return IVar1;
}


