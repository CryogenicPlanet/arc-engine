// SOURCE /home/exedev/work/inputs/decompile/0x0.c:8840
// ==== 0xfbc0  ArcWebContentsConfiguration::.cxx_destruct  (4 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x0.c:13995
// ==== 0x15ac0  PiPNSView::viewWillStartLiveResize  (4 bytes)

/* Function Stack Size: 0x10 bytes */

void PiPNSView::viewWillStartLiveResize(ID param_1,SEL param_2)

{
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x18.c:117132
// ==== 0x189a538  RTCVideoEncoderSettings::startBitrate  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_int RTCVideoEncoderSettings::startBitrate(ID param_1,SEL param_2)

{
  return *(unsigned_int *)(param_1 + 0xc);
}



// SOURCE /home/exedev/work/inputs/decompile/0x2b.c:27260
// ==== 0x2b25740  ArcWebContentsConfiguration::foreground  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::foreground(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x2d.c:11381
// ==== 0x2d0fb40  WebNNMLFeatureProvider::featureNames  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID WebNNMLFeatureProvider::featureNames(ID param_1,SEL param_2)

{
  return *(ID *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x35.c:53505
// ==== 0x3547bf0  RTCEncodedImage::frameType  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long RTCEncodedImage::frameType(ID param_1,SEL param_2)

{
  return *(unsigned_long_long *)(param_1 + 0x40);
}



// SOURCE /home/exedev/work/inputs/decompile/0x35.c:177752
// ==== 0x35f0300  ArcCpuInfo::extFamily  (8 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcCpuInfo::extFamily(ID param_1,SEL param_2)

{
  return *(long_long *)(param_1 + 0x60);
}



// SOURCE /home/exedev/work/inputs/decompile/0x38.c:71510
// ==== 0x387a780  ArcWebContentsConfiguration::disallowContextMenus  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::disallowContextMenus(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:79544
// ==== 0x4041fec  objc::Categories::ArcBrowserContextDownloads_Extensions_::setDelegate:  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContextDownloads_Extensions_::setDelegate_
               (ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  long *extraout_x1;
  long *extraout_x1_00;
  long *extraout_x1_01;
  long *plVar5;
  undefined1 auStack_68 [16];
  byte local_58;
  
  IVar3 = _objc_msgSend(param_1,"downloadsDelegate");
  if (param_3 == 0) {
    if (extraout_x1 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar2 = extraout_x1[1];
    extraout_x1[1] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
    (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
    plVar4 = extraout_x1;
  }
  else {
    if (extraout_x1 != (long *)0x0) {
      LOAcquire();
      lVar2 = extraout_x1[1];
      extraout_x1[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
        FUN_02527d18(extraout_x1);
      }
    }
    if (IVar3 == 0) {
      IVar3 = _objc_msgSend(param_1,"owner");
      plVar4 = (long *)FUN_00008790(0x30);
      plVar5 = plVar4 + 1;
      *plVar5 = 0;
      plVar4[2] = 0;
      *plVar4 = (long)&PTR_LAB_0dd2bf70;
      if (plVar4 + 3 != (long *)0x0) {
        plVar4[3] = (long)&PTR_FUN_0dd2bfb0;
        _objc_initWeak(plVar4 + 4,param_3);
        _objc_initWeak(plVar4 + 5,IVar3);
        if (param_1 == 0) {
          if (plVar4 != (long *)0x0) {
            LOAcquire();
            lVar2 = *plVar5;
            *plVar5 = lVar2 + -1;
            LORelease();
            if (lVar2 == 0) {
              (**(code **)(*plVar4 + 0x10))(plVar4);
              FUN_02527d18(plVar4);
            }
          }
        }
        else {
          _objc_msgSend(param_1,"setDownloadsDelegate:",plVar4 + 3,plVar4);
        }
        _objc_msgSend(param_1,"browser_context_downloads",auStack_68);
        if ((local_58 & 1) != 0) {
          IVar3 = _objc_msgSend(param_1,"downloadsDelegate");
          if (extraout_x1_01 != (long *)0x0) {
            extraout_x1_01[2] = extraout_x1_01[2] + 1;
          }
          FUN_04332814(auStack_68,IVar3,extraout_x1_01);
          if (extraout_x1_01 != (long *)0x0) {
            LOAcquire();
            lVar2 = extraout_x1_01[1];
            extraout_x1_01[1] = lVar2 + -1;
            LORelease();
            if (lVar2 == 0) {
              (**(code **)(*extraout_x1_01 + 0x10))(extraout_x1_01);
              FUN_02527d18(extraout_x1_01);
            }
          }
          if (local_58 == 0) {
            return;
          }
          FUN_04332750(auStack_68);
          return;
        }
      }
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4042260);
      (*pcVar1)();
    }
    IVar3 = _objc_msgSend(param_1,"downloadsDelegate");
    *(ID *)(IVar3 + 8) = param_3;
    if (extraout_x1_00 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar2 = extraout_x1_00[1];
    extraout_x1_00[1] = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
    (**(code **)(*extraout_x1_00 + 0x10))(extraout_x1_00);
    plVar4 = extraout_x1_00;
  }
  FUN_02527d18(plVar4);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:79668
// ==== 0x40422b0  objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload:error:  (276 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextDownloads_Extensions_::startDownload_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  code *pcVar1;
  uint uVar2;
  ID IVar3;
  uint *puVar4;
  uint *local_70;
  undefined8 local_68 [2];
  undefined7 local_58;
  char cStack_51;
  undefined1 auStack_50 [16];
  byte local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context_downloads",auStack_50);
  if ((local_40 & 1) != 0) {
    IVar3 = _objc_msgSend(param_3,"absoluteString");
    FUN_03b60ee8(local_68,IVar3);
    FUN_043328cc(&local_38,auStack_50,local_68);
    if (cStack_51 < '\0') {
      FUN_00005810(local_68[0],CONCAT17(cStack_51,local_58) & 0x7fffffffffffffff);
    }
    if (local_40 != 0) {
      FUN_04332750(auStack_50);
    }
    if (local_38 != (uint *)((long)&MACH_HEADER.magic + 1) && param_4 != (ID *)0x0) {
      local_70 = local_38;
      puVar4 = local_38;
      if (((ulong)local_38 & 1) == 0) {
        uVar2 = *local_38;
        *local_38 = uVar2 + 1;
        puVar4 = (uint *)(ulong)uVar2;
      }
      IVar3 = FUN_044025f4(puVar4,&local_70);
      *param_4 = IVar3;
      if (((ulong)local_70 & 1) == 0) {
        FUN_04b35898();
      }
    }
    if (((ulong)local_38 & 1) == 0) {
      FUN_04b35898();
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x40423c4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:102910
// ==== 0x4060580  RTCVideoFrame::buffer  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID RTCVideoFrame::buffer(ID param_1,SEL param_2)

{
  return *(ID *)(param_1 + 0x20);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:119963
// ==== 0x4073a00  ArcCpuInfo::hasSSE41  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSE41(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:132317
// ==== 0x4080f00  RTCEncodedImage::rotation  (8 bytes)

/* Function Stack Size: 0x10 bytes */

long_long RTCEncodedImage::rotation(ID param_1,SEL param_2)

{
  return *(long_long *)(param_1 + 0x48);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:132328
// ==== 0x4080f20  RTCEncodedImage::qp  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID RTCEncodedImage::qp(ID param_1,SEL param_2)

{
  return *(ID *)(param_1 + 0x50);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:133054
// ==== 0x4081a30  ArcAccessibilityTreeNode::nodeId  (8 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAccessibilityTreeNode::nodeId(ID param_1,SEL param_2)

{
  return *(int *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:134671
// ==== 0x4083e80  ArcBrowserApplicationDebugProperty::.cxx_construct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplicationDebugProperty::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:135360
// ==== 0x4084dc0  AXAnnouncementSpec::window  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID AXAnnouncementSpec::window(ID param_1,SEL param_2)

{
  return *(ID *)(param_1 + 0x18);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:135519
// ==== 0x4085370  RTCEncodedImage::contentType  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long RTCEncodedImage::contentType(ID param_1,SEL param_2)

{
  return *(unsigned_long_long *)(param_1 + 0x58);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:136106
// ==== 0x4086678  ArcURLLoadConfiguration::followRedirects  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcURLLoadConfiguration::followRedirects(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xa0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:137039
// ==== 0x40879ac  ArcAccessibilityTreeNode::isEditable  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAccessibilityTreeNode::isEditable(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x71);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:168900
// ==== 0x40b1230  MacScreenMonitor::isScreensaverRunning  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void MacScreenMonitor::isScreensaverRunning
               (void *param_1,void *param_2,int param_3,char *param_4,int param_5,int param_6)

{
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:184658
// ==== 0x40c6270  ArcWebContentsConfiguration::forceDisplayPDFInline  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::forceDisplayPDFInline(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x40.c:188775
// ==== 0x40cbfe8  ArcHistoryQueryOptions::maxResults  (12 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcHistoryQueryOptions::maxResults(ID param_1,SEL param_2)

{
  return *(int *)(*(long *)(param_1 + 8) + 0x28);
}



// SOURCE /home/exedev/work/inputs/decompile/0x41.c:68569
// ==== 0x4154498  ArcAccessibilityTreeNode::isFocusable  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAccessibilityTreeNode::isFocusable(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x72);
}



// SOURCE /home/exedev/work/inputs/decompile/0x41.c:68580
// ==== 0x41544d0  ArcExtensionCommand::isGlobal  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionCommand::isGlobal(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x70);
}



// SOURCE /home/exedev/work/inputs/decompile/0x41.c:83166
// ==== 0x416803c  AlertBridgeHelper::setAlertBridge:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void AlertBridgeHelper::setAlertBridge_(ID param_1,SEL param_2,void *param_3)

{
  *(void **)(param_1 + 0x10) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x41.c:83178
// ==== 0x4168044  RTCEncodedImage::setCaptureTimeMs:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void RTCEncodedImage::setCaptureTimeMs_(ID param_1,SEL param_2,long_long param_3)

{
  *(long_long *)(param_1 + 0x20) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x41.c:84191
// ==== 0x416a680  SDPQueryListener::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID SDPQueryListener::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:8921
// ==== 0x420b588  AXAnnouncementSpec::setAnnouncement:  (12 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x18 bytes */

void AXAnnouncementSpec::setAnnouncement_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x10) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:8934
// ==== 0x420b594  ArcTabGroupResult::setGroupId:  (8 bytes)

/* WARNING: Function: _objc_setProperty_nonatomic_copy replaced with injection:
   _objc_setProperty_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTabGroupResult::setGroupId_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x18) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:9659
// ==== 0x420c7a4  ArcCpuInfo::hasBTI  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasBTI(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x18);
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:47057
// ==== 0x4243428  ArcExtensionToolbarAction::enabled  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionToolbarAction::enabled(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x68);
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:127742
// ==== 0x42b0850  FUN_042b0850  (1168 bytes)

/* WARNING: Removing unreachable block (ram,0x042b0958) */

undefined8 FUN_042b0850(ulong param_1,undefined4 *param_2)

{
  undefined1 *puVar1;
  long *plVar2;
  undefined4 uVar3;
  uint uVar4;
  byte bVar5;
  byte bVar6;
  undefined1 uVar7;
  int *piVar8;
  code *pcVar9;
  int iVar10;
  undefined8 *puVar11;
  long lVar12;
  long *plVar13;
  long lVar14;
  undefined4 uVar15;
  size_t sVar16;
  long *plVar17;
  long lVar18;
  undefined8 uVar19;
  undefined8 *puVar20;
  ulong uVar21;
  int *local_f8;
  undefined4 local_f0;
  long *local_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  int *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  int *local_88;
  int *local_80;
  undefined8 uStack_78;
  ulong local_70;
  
  if (*(long *)(param_1 + 0x98) == 0) {
    uVar3 = param_2[0xe];
    uVar15 = 1;
    if (*(char *)(param_2 + 0x16) == '\0') {
      uVar15 = 2;
    }
    uVar4 = param_2[0x17];
    bVar5 = *(byte *)(param_2 + 0x18);
    bVar6 = *(byte *)((long)param_2 + 0x61);
    if (*(char *)((long)param_2 + 0x57) < '\0') {
      FUN_00060a60(&local_80,*(undefined8 *)(param_2 + 0x10),*(undefined8 *)(param_2 + 0x12));
    }
    else {
      uStack_78 = *(undefined8 *)(param_2 + 0x12);
      local_80 = *(int **)(param_2 + 0x10);
      local_70 = *(ulong *)(param_2 + 0x14);
    }
    uVar19 = FUN_00008790(0x58);
    uStack_b8 = uStack_78;
    local_c0 = local_80;
    local_b0 = local_70;
    local_80 = (int *)0x0;
    uStack_78 = 0;
    local_70 = 0;
    DAT_0e4ac750 = FUN_044d3828(uVar19,uVar3,uVar15,(ulong)uVar4 | 0x100000000,
                                (ulong)bVar5 ^ 1 | 0x100000000,(ulong)bVar6 | 0x100000000,&local_c0)
    ;
    if ((long)local_b0 < 0) {
      FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
    }
    if (*(int *)(DAT_0e4ac750 + 0x14) == 0) {
                    /* WARNING: Does not return */
      pcVar9 = (code *)SoftwareBreakpoint(0,0x42b0cdc);
      (*pcVar9)();
    }
    FUN_0354e8e8();
    puVar1 = (undefined1 *)(param_1 + 0x148);
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1 + 0x148);
    }
    uVar7 = *(undefined1 *)(param_1 + 0x148);
    *(undefined1 *)(param_1 + 0x148) = 1;
    uVar19 = FUN_01f0d54c();
    FUN_028d08b8(uVar19,1);
    uVar19 = FUN_07bf4c40();
    FUN_04d32b00(uVar19,param_1 + 0x28);
    if ((param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_1);
    }
    local_88 = (int *)FUN_00008790(0x38);
    *local_88 = 1;
    *(undefined1 **)(local_88 + 2) = &LAB_040c4380;
    *(code **)(local_88 + 4) = FUN_039a7300;
    *(code **)(local_88 + 6) = FUN_00b77080;
    *(code **)(local_88 + 8) = FUN_042b0ce4;
    local_88[10] = 0;
    local_88[0xb] = 0;
    *(ulong *)(local_88 + 0xc) = param_1;
    FUN_07bf4b40(&local_c0,&local_88);
    FUN_01469964(param_1 + 0x460,&local_c0);
    piVar8 = local_c0;
    if (local_c0 != (int *)0x0) {
      local_c0 = (int *)0x0;
      (**(code **)(piVar8 + 2))(piVar8);
      LOAcquire();
      iVar10 = *piVar8;
      *piVar8 = iVar10 + -1;
      LORelease();
      if (iVar10 == 1) {
        (**(code **)(piVar8 + 4))(piVar8);
      }
      if (local_c0 != (int *)0x0) {
        LOAcquire();
        iVar10 = *local_c0;
        *local_c0 = iVar10 + -1;
        LORelease();
        if (iVar10 == 1) {
          (**(code **)(local_c0 + 4))();
        }
      }
    }
    if (local_88 != (int *)0x0) {
      LOAcquire();
      iVar10 = *local_88;
      *local_88 = iVar10 + -1;
      LORelease();
      if (iVar10 == 1) {
        (**(code **)(local_88 + 4))();
      }
    }
    puVar11 = (undefined8 *)FUN_00008790(0x38);
    *(undefined1 *)(puVar11 + 5) = 0;
    puVar11[6] = 0;
    puVar11[1] = 0;
    *puVar11 = 0;
    puVar11[3] = 0;
    puVar11[2] = 0;
    *(undefined1 *)(puVar11 + 4) = 0;
    lVar12 = *(long *)(param_1 + 0x98);
    *(undefined8 **)(param_1 + 0x98) = puVar11;
    if (lVar12 != 0) {
      uVar19 = FUN_042aa684();
      FUN_00005810(uVar19,0x38);
    }
    uStack_90 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    local_98 = 0;
    local_a0 = 0;
    uStack_b8 = 0;
    local_c0 = (int *)0x0;
    FUN_0421b464(&local_c0,0);
    plVar17 = (long *)(param_1 + 0x318);
    lVar12 = *(long *)(param_1 + 0x318);
    if (lVar12 != 0) {
      lVar18 = *(long *)(param_1 + 800) * 0x18;
      lVar14 = lVar12;
      if (lVar18 != 0) {
        lVar14 = *(long *)(param_1 + 800) * -0x18;
        puVar11 = (undefined8 *)(lVar18 + lVar12);
        do {
          puVar20 = puVar11 + -3;
          if (puVar20 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
            pcVar9 = (code *)SoftwareBreakpoint(1,0x42b0ce4);
            (*pcVar9)();
          }
          if (*(char *)((long)puVar11 + -1) < '\0') {
            FUN_00005810(*puVar20,puVar11[-1] & 0x7fffffffffffffff);
          }
          lVar14 = lVar14 + 0x18;
          puVar11 = puVar20;
        } while (lVar14 != 0);
        lVar14 = *plVar17;
      }
      *(long *)(param_1 + 800) = (lVar12 - lVar14 >> 3) * -0x5555555555555555;
      FUN_00005810(lVar14,*(long *)(param_1 + 0x328) * 0x18);
      *plVar17 = 0;
      *(undefined8 *)(param_1 + 800) = 0;
      *(undefined8 *)(param_1 + 0x328) = 0;
    }
    *(undefined8 *)(param_1 + 0x318) = *(undefined8 *)(param_2 + 2);
    *(undefined8 *)(param_1 + 800) = *(undefined8 *)(param_2 + 4);
    *(undefined8 *)(param_1 + 0x328) = *(undefined8 *)(param_2 + 6);
    *(undefined8 *)(param_2 + 2) = 0;
    *(undefined8 *)(param_2 + 4) = 0;
    *(undefined8 *)(param_2 + 6) = 0;
    uVar21 = *(ulong *)(param_1 + 800);
    sVar16 = uVar21 << 3;
    if (uVar21 >> 0x3d != 0) {
      sVar16 = 0xffffffffffffffff;
    }
    plVar13 = (long *)FUN_00008790(sVar16);
    _bzero(plVar13,sVar16);
    lVar12 = *(long *)(param_1 + 0x330);
    *(long **)(param_1 + 0x330) = plVar13;
    if (lVar12 != 0) {
      FUN_00010ba4();
      uVar21 = *(ulong *)(param_1 + 800);
      plVar13 = *(long **)(param_1 + 0x330);
    }
    lVar12 = uVar21 * 0x18;
    if (lVar12 != 0) {
      plVar17 = (long *)*plVar17;
      do {
        plVar2 = (long *)*plVar17;
        if (-1 < *(char *)((long)plVar17 + 0x17)) {
          plVar2 = plVar17;
        }
        *plVar13 = (long)plVar2;
        plVar17 = plVar17 + 3;
        lVar12 = lVar12 + -0x18;
        plVar13 = plVar13 + 1;
      } while (lVar12 != 0);
      plVar13 = *(long **)(param_1 + 0x330);
    }
    uStack_b8 = CONCAT44(uStack_b8._4_4_,*param_2);
    if (((ulong)plVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(plVar13);
    }
    if ((local_b0 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    uStack_d8 = local_a0;
    local_e0 = uStack_a8;
    local_f8 = local_c0;
    local_f0 = (undefined4)uStack_b8;
    local_c0 = (int *)0x0;
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
    uStack_c8 = uStack_90;
    uStack_d0 = local_98;
    local_e8 = plVar13;
    uVar19 = FUN_042aa7ec(*(undefined8 *)(param_1 + 0x98),&local_f8,0,0,0,param_2 + 8);
    FUN_02440774(&local_f8);
    FUN_02440774(&local_c0);
    if (puVar1 != (undefined1 *)0x0) {
      *puVar1 = uVar7;
    }
    if (((ulong)puVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(puVar1);
    }
  }
  else {
    uVar19 = 0xffffffff;
  }
  return uVar19;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:128510
// ==== 0x42b17e8  FUN_042b17e8  (948 bytes)

undefined1  [16] FUN_042b17e8(void)

{
  long lVar1;
  bool bVar2;
  byte bVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  long *plVar7;
  long *plVar8;
  code *pcVar9;
  long *plVar10;
  undefined1 auVar11 [16];
  
  if (((DAT_0e2f1bf0 & 1) == 0) && (iVar6 = thunk_FUN_0c44b610(&DAT_0e2f1bf0), iVar6 != 0)) {
    DAT_0e2f1be0 = 0;
    DAT_0e2f1be8 = (long *)0x0;
    DAT_0e2f1bf0 = 1;
    iVar6 = _pthread_mutex_lock((pthread_mutex_t *)&DAT_0e3780e0);
    bVar3 = DAT_0e2f1bf1;
    if (iVar6 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to acquire mutex");
    }
    DAT_0e2f1bf1 = 1;
    iVar6 = _pthread_mutex_unlock((pthread_mutex_t *)&DAT_0e3780e0);
    if (iVar6 != 0) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to release mutex");
    }
    if (((bVar3 >> 2 & 1) != 0) &&
       (iVar6 = _pthread_cond_broadcast((pthread_cond_t *)&DAT_0e378120), iVar6 != 0)) {
                    /* WARNING: Subroutine does not return */
      thunk_FUN_0c4495ac("%s failed to broadcast");
    }
  }
  plVar7 = DAT_0e2f1be8;
  if (DAT_0e2f1be8 != (long *)0x0) {
    lVar4 = DAT_0e2f1be8[1];
    do {
      if (lVar4 == -1) goto LAB_042b1860;
      lVar5 = lVar4 + 1;
      lVar1 = DAT_0e2f1be8[1];
      bVar2 = lVar1 != lVar4;
      lVar4 = lVar1;
    } while (bVar2);
    DAT_0e2f1be8[1] = lVar5;
    lVar4 = DAT_0e2f1be0;
    plVar10 = DAT_0e2f1be8;
    if (DAT_0e2f1be0 != 0) goto LAB_042b18f8;
    LOAcquire();
    lVar4 = plVar7[1];
    plVar7[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar4 = plVar7[2];
        plVar7[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_042b1860;
      }
      pcVar9 = *(code **)(*plVar7 + 0x20);
      if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar9)(plVar7);
      }
    }
  }
LAB_042b1860:
  lVar4 = FUN_00008790(0x478);
  FUN_042b0300();
  plVar7 = (long *)FUN_00008790(0x20);
  plVar10 = plVar7 + 1;
  *plVar10 = 0;
  *plVar7 = (long)&PTR_viewWillStartLiveResize_0da7e818;
  plVar7[2] = 0;
  plVar7[3] = lVar4;
  if ((lVar4 == 0) ||
     ((plVar8 = *(long **)(lVar4 + 0x20), plVar8 != (long *)0x0 && (plVar8[1] != -1)))) {
    if (plVar7 != (long *)0x0) goto LAB_042b18b0;
  }
  else {
    if (plVar7 != (long *)0x0) {
      plVar7[1] = plVar7[1] + 1;
      plVar7[2] = plVar7[2] + 1;
    }
    *(long *)(lVar4 + 0x18) = lVar4;
    *(long **)(lVar4 + 0x20) = plVar7;
    if (plVar8 != (long *)0x0) {
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar5 = plVar8[2];
        plVar8[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042b1998;
      }
      pcVar9 = *(code **)(*plVar8 + 0x20);
      if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar9)();
      }
    }
LAB_042b1998:
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar5 = *plVar10;
      *plVar10 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar5 = plVar7[2];
          plVar7[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042b18b0;
        }
        pcVar9 = *(code **)(*plVar7 + 0x20);
        if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar9)(plVar7);
        }
      }
LAB_042b18b0:
      plVar7[2] = plVar7[2] + 1;
    }
  }
  plVar8 = DAT_0e2f1be8;
  DAT_0e2f1be0 = lVar4;
  plVar10 = plVar7;
  if (DAT_0e2f1be8 != (long *)0x0) {
    if (DAT_0e2f1be8[2] != 0) {
      plVar10 = DAT_0e2f1be8 + 2;
      LOAcquire();
      lVar5 = *plVar10;
      DAT_0e2f1be8 = plVar7;
      *plVar10 = lVar5 + -1;
      LORelease();
      plVar10 = DAT_0e2f1be8;
      if (lVar5 != 0) goto LAB_042b18f8;
    }
    DAT_0e2f1be8 = plVar10;
    pcVar9 = *(code **)(*plVar8 + 0x20);
    if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
      FUN_00005810(plVar8,0x20);
      plVar10 = DAT_0e2f1be8;
    }
    else {
      (*pcVar9)();
      plVar10 = DAT_0e2f1be8;
    }
  }
LAB_042b18f8:
  DAT_0e2f1be8 = plVar10;
  auVar11._8_8_ = plVar7;
  auVar11._0_8_ = lVar4;
  return auVar11;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:128681
// ==== 0x42b1ba8  FUN_042b1ba8  (1564 bytes)

undefined8 FUN_042b1ba8(ulong param_1)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  long lVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  long *plVar8;
  code *pcVar9;
  undefined8 uVar10;
  long *plVar11;
  long lVar12;
  long *plVar13;
  undefined1 local_140 [16];
  char local_130 [16];
  undefined2 local_120;
  char cStack_11e;
  undefined4 uStack_11d;
  char cStack_119;
  undefined8 local_110;
  char local_108 [8];
  undefined7 local_100;
  char cStack_f9;
  undefined1 auStack_f8 [128];
  ulong local_78 [2];
  undefined1 *local_68;
  undefined *local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (*(long *)(param_1 + 0x98) == 0) {
    uVar10 = 0xffffffff;
    goto LAB_042b1be0;
  }
  uVar10 = FUN_02bed438();
  if ((param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  puVar6 = (undefined4 *)FUN_00008790(0x38);
  *puVar6 = 1;
  *(undefined1 **)(puVar6 + 2) = &LAB_040bd200;
  *(code **)(puVar6 + 4) = FUN_039a7300;
  *(code **)(puVar6 + 6) = FUN_00b77080;
  *(undefined **)(puVar6 + 8) = &DAT_042b21cc;
  *(undefined8 *)(puVar6 + 10) = 0;
  *(ulong *)(puVar6 + 0xc) = param_1;
  FUN_044cf3a4(local_140,uVar10,puVar6);
  FUN_01469964(param_1 + 0x440,local_140);
  uVar10 = local_140._0_8_;
  if ((long *)local_140._0_8_ != (long *)0x0) {
    local_140._0_8_ = (long *)0x0;
    (**(code **)(uVar10 + 8))(uVar10);
    LOAcquire();
    iVar4 = *(int *)uVar10;
    *(int *)uVar10 = iVar4 + -1;
    LORelease();
    if (iVar4 == 1) {
      (**(code **)(uVar10 + 0x10))(uVar10);
    }
    if ((long *)local_140._0_8_ != (long *)0x0) {
      LOAcquire();
      iVar4 = *(int *)local_140._0_8_;
      *(int *)local_140._0_8_ = iVar4 + -1;
      LORelease();
      if (iVar4 == 1) {
        (**(code **)(local_140._0_8_ + 0x10))();
      }
    }
  }
  local_68 = &LAB_042c18f0;
  local_60 = &DAT_0c9d66d8;
  local_78[0] = param_1;
  FUN_042aaaa0(*(undefined8 *)(param_1 + 0x98),local_78);
  if (*(code **)(local_60 + 8) != (code *)0x0) {
    (**(code **)(local_60 + 8))(local_78[0]);
  }
  uVar10 = FUN_042aac34(*(undefined8 *)(param_1 + 0x98));
  uVar7 = FUN_02bed438();
  cStack_f9 = '\b';
  if ("shutdown" < local_108 && &local_110 < "hutdown") {
LAB_042b21bc:
                    /* WARNING: Does not return */
    pcVar9 = (code *)SoftwareBreakpoint(1,0x42b21c0);
    (*pcVar9)();
  }
  local_110 = 0x6e776f6474756873;
  local_108[0] = '\0';
  cStack_119 = '\x12';
  if ("Main loop returned" < &cStack_11e && local_130 < "ain loop returned") goto LAB_042b21bc;
  local_120 = 0x6465;
  builtin_strncpy(local_130,"Main loop return",0x10);
  cStack_11e = '\0';
  FUN_08b903b4(auStack_f8,&local_110,local_130);
  FUN_02bed570(uVar7,auStack_f8);
  FUN_08b90450(auStack_f8);
  if (cStack_119 < '\0') {
    FUN_00005810(local_130._0_8_,
                 CONCAT17(cStack_119,CONCAT43(uStack_11d,CONCAT12(cStack_11e,local_120))) &
                 0x7fffffffffffffff);
  }
  if (cStack_f9 < '\0') {
    FUN_00005810(local_110,CONCAT17(cStack_f9,local_100) & 0x7fffffffffffffff);
  }
  FUN_042ab360(*(undefined8 *)(param_1 + 0x98));
  if ((*(byte *)(DAT_0e4ac750 + 0x2c) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar9 = (code *)SoftwareBreakpoint(0,0x42b21c4);
    (*pcVar9)();
  }
  if ((*(int *)(DAT_0e4ac750 + 0x28) == 1) &&
     ((*(byte *)(*(long *)(param_1 + 0x168) + 0x10) & 1) == 0)) {
    (**(code **)(param_1 + 0x160))(param_1 + 0x150,1);
  }
  plVar11 = *(long **)(param_1 + 0x70);
  if (plVar11 == (long *)0x0) {
    local_140._0_8_ = (long *)0x0;
    FUN_01469964(param_1 + 0x440,local_140);
    uVar7 = local_140._0_8_;
    if ((long *)local_140._0_8_ == (long *)0x0) goto LAB_042b1be0;
    local_140._0_8_ = (long *)0x0;
    (**(code **)(uVar7 + 8))(uVar7);
    LOAcquire();
    iVar4 = *(int *)uVar7;
    *(int *)uVar7 = iVar4 + -1;
    LORelease();
    if (iVar4 == 1) {
      (**(code **)(uVar7 + 0x10))(uVar7);
    }
    if ((long *)local_140._0_8_ == (long *)0x0) goto LAB_042b1be0;
    LOAcquire();
    iVar4 = *(int *)local_140._0_8_;
    *(int *)local_140._0_8_ = iVar4 + -1;
    LORelease();
    if (iVar4 != 1) goto LAB_042b1be0;
    pcVar9 = *(code **)(local_140._0_8_ + 0x10);
    plVar11 = (long *)local_140._0_8_;
  }
  else {
    lVar12 = *(long *)(param_1 + 0x68);
    plVar11[2] = plVar11[2] + 1;
    lVar5 = plVar11[1];
    do {
      if (lVar5 == -1) goto LAB_042b1f58;
      lVar1 = lVar5 + 1;
      lVar2 = plVar11[1];
      bVar3 = lVar2 != lVar5;
      lVar5 = lVar2;
    } while (bVar3);
    plVar11[1] = lVar1;
    if (lVar12 != 0) {
      local_140 = FUN_042b17e8();
      plVar8 = local_140._8_8_;
      if (plVar8 != (long *)0x0) {
        plVar8[1] = plVar8[1] + 1;
      }
      FUN_043eb430(lVar12,local_140);
      uVar7 = local_140._8_8_;
      if ((long *)local_140._8_8_ != (long *)0x0) {
        LOAcquire();
        lVar5 = *(long *)(local_140._8_8_ + 8);
        *(long *)(local_140._8_8_ + 8) = lVar5 + -1;
        LORelease();
        if (lVar5 == 0) {
          plVar13 = (long *)(local_140._8_8_ + 0x10);
          (**(code **)(*(long *)local_140._8_8_ + 0x10))(local_140._8_8_);
          if (*plVar13 != 0) {
            LOAcquire();
            lVar5 = *(long *)(uVar7 + 0x10);
            *(long *)(uVar7 + 0x10) = lVar5 + -1;
            LORelease();
            if (lVar5 != 0) goto LAB_042b1f34;
          }
          pcVar9 = *(code **)(*(long *)uVar7 + 0x20);
          if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
            FUN_00005810(uVar7,0x20);
          }
          else {
            (*pcVar9)(uVar7);
          }
        }
      }
LAB_042b1f34:
      if (plVar8 != (long *)0x0) {
        LOAcquire();
        lVar5 = plVar8[1];
        plVar8[1] = lVar5 + -1;
        LORelease();
        if (lVar5 == 0) {
          (**(code **)(*plVar8 + 0x10))(plVar8);
          if (plVar8[2] != 0) {
            LOAcquire();
            lVar5 = plVar8[2];
            plVar8[2] = lVar5 + -1;
            LORelease();
            if (lVar5 != 0) goto LAB_042b1f48;
          }
          pcVar9 = *(code **)(*plVar8 + 0x20);
          if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
            FUN_00005810(plVar8,0x20);
          }
          else {
            (*pcVar9)(plVar8);
          }
        }
      }
    }
LAB_042b1f48:
    LOAcquire();
    lVar5 = plVar11[1];
    plVar11[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar11 + 0x10))(plVar11);
      if (plVar11[2] != 0) {
        LOAcquire();
        lVar5 = plVar11[2];
        plVar11[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_042b1f58;
      }
      pcVar9 = *(code **)(*plVar11 + 0x20);
      if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
        FUN_00005810(plVar11,0x20);
      }
      else {
        (*pcVar9)(plVar11);
      }
    }
LAB_042b1f58:
    local_140._0_8_ = (long *)0x0;
    FUN_01469964(param_1 + 0x440,local_140);
    uVar7 = local_140._0_8_;
    if ((long *)local_140._0_8_ != (long *)0x0) {
      local_140._0_8_ = (long *)0x0;
      (**(code **)(uVar7 + 8))(uVar7);
      LOAcquire();
      iVar4 = *(int *)uVar7;
      *(int *)uVar7 = iVar4 + -1;
      LORelease();
      if (iVar4 == 1) {
        (**(code **)(uVar7 + 0x10))(uVar7);
      }
      if ((long *)local_140._0_8_ != (long *)0x0) {
        LOAcquire();
        iVar4 = *(int *)local_140._0_8_;
        *(int *)local_140._0_8_ = iVar4 + -1;
        LORelease();
        if (iVar4 == 1) {
          (**(code **)(local_140._0_8_ + 0x10))();
        }
      }
    }
    if (plVar11[2] != 0) {
      LOAcquire();
      lVar5 = plVar11[2];
      plVar11[2] = lVar5 + -1;
      LORelease();
      if (lVar5 != 0) goto LAB_042b1be0;
    }
    pcVar9 = *(code **)(*plVar11 + 0x20);
    if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
      FUN_00005810(plVar11,0x20);
      goto LAB_042b1be0;
    }
  }
  (*pcVar9)(plVar11);
LAB_042b1be0:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail();
  }
  return uVar10;
}



// SOURCE /home/exedev/work/inputs/decompile/0x42.c:129096
// ==== 0x42b2430  FUN_042b2430  (348 bytes)

undefined8 FUN_042b2430(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  long lVar4;
  code *pcVar5;
  undefined8 uVar6;
  byte *local_168;
  long lStack_160;
  char *local_158 [36];
  char local_32;
  byte local_31;
  
  if (*(long *)(param_1 + 0x98) == 0) {
    uVar2 = 1;
  }
  else {
    if (DAT_0e49cf50 == '\0') {
      DAT_0e49cf50 = '\x01';
      uVar2 = *param_2;
      uVar1 = param_2[1];
      uVar6 = param_2[3];
      uVar3 = param_2[2];
      if (*(code **)param_2[3] != (code *)0x0) {
        uVar2 = (**(code **)param_2[3])();
      }
      lVar4 = *(long *)(param_1 + 0x168);
      *(undefined8 *)(param_1 + 0x168) = uVar6;
      *(undefined8 *)(param_1 + 0x160) = uVar3;
      uVar3 = *(undefined8 *)(param_1 + 0x150);
      *(undefined8 *)(param_1 + 0x150) = uVar2;
      *(undefined8 *)(param_1 + 0x158) = uVar1;
      pcVar5 = *(code **)(lVar4 + 8);
      if (pcVar5 != (code *)0x0) {
        (*pcVar5)(uVar3);
      }
      local_31 = 0;
      local_32 = '\x01';
      local_168 = &local_31;
      local_158[0] = &local_32;
      lStack_160 = param_1;
      FUN_018f0070(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),&local_168,FUN_042c21a8,0,0)
      ;
      if (local_32 == '\0') {
        return 2;
      }
      if ((local_31 & 1) != 0) {
        return 2;
      }
      FUN_042b258c(param_1);
    }
    else if ((DAT_0e4a3078 < 2) && (DAT_0e2f1ce0 != 0 || DAT_0e4a3080 != 0)) {
      FUN_00f195d8(&local_168,"../../arc/browser/browser_application_impl.cc",0x1e4,1);
      FUN_00040060(local_158,
                   "Trying to terminate the browser that is terminating. Deferring to later.",0x48);
      FUN_00f1950c(&local_168);
    }
    uVar2 = 2;
  }
  return uVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:34129
// ==== 0x4332148  FUN_04332148  (184 bytes)

void FUN_04332148(undefined8 *param_1,undefined8 param_2,long *param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 uVar3;
  code *UNRECOVERED_JUMPTABLE;
  
  uVar3 = *param_1;
  if (param_3 == (long *)0x0) {
    FUN_042b7974(uVar3,param_2,0);
    return;
  }
  plVar1 = param_3 + 2;
  lVar2 = *plVar1;
  *plVar1 = lVar2 + 1;
  FUN_042b7974(lVar2,uVar3,param_2,param_3);
  if (param_3[2] != 0) {
    LOAcquire();
    lVar2 = *plVar1;
    *plVar1 = lVar2 + -1;
    LORelease();
    if (lVar2 != 0) {
      return;
    }
  }
  UNRECOVERED_JUMPTABLE = *(code **)(*param_3 + 0x20);
  if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
     (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043321fc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(param_3);
    return;
  }
  FUN_00005810(param_3,0x20);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:34399
// ==== 0x433260c  FUN_0433260c  (120 bytes)

void FUN_0433260c(undefined4 *param_1,undefined4 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  *param_1 = *param_2;
  uVar1 = *(undefined8 *)(param_2 + 2);
  *(undefined8 *)(param_2 + 2) = 0;
  *(undefined8 *)(param_1 + 2) = uVar1;
  uVar1 = *(undefined8 *)(param_2 + 4);
  *(undefined8 *)(param_2 + 4) = 0;
  *(undefined8 *)(param_1 + 4) = uVar1;
  uVar1 = *(undefined8 *)(param_2 + 6);
  *(undefined8 *)(param_2 + 6) = 0;
  *(undefined8 *)(param_1 + 6) = uVar1;
  uVar2 = *(undefined8 *)(param_2 + 10);
  uVar1 = *(undefined8 *)(param_2 + 8);
  *(undefined8 *)(param_1 + 0xc) = *(undefined8 *)(param_2 + 0xc);
  *(undefined8 *)(param_1 + 10) = uVar2;
  *(undefined8 *)(param_1 + 8) = uVar1;
  *(undefined8 *)(param_2 + 10) = 0;
  *(undefined8 *)(param_2 + 0xc) = 0;
  *(undefined8 *)(param_2 + 8) = 0;
  param_1[0xe] = param_2[0xe];
  uVar2 = *(undefined8 *)(param_2 + 0x12);
  uVar1 = *(undefined8 *)(param_2 + 0x10);
  *(undefined8 *)(param_1 + 0x14) = *(undefined8 *)(param_2 + 0x14);
  *(undefined8 *)(param_1 + 0x12) = uVar2;
  *(undefined8 *)(param_1 + 0x10) = uVar1;
  *(undefined8 *)(param_2 + 0x12) = 0;
  *(undefined8 *)(param_2 + 0x14) = 0;
  *(undefined8 *)(param_2 + 0x10) = 0;
  uVar1 = *(undefined8 *)(param_2 + 0x16);
  *(undefined2 *)(param_1 + 0x18) = *(undefined2 *)(param_2 + 0x18);
  *(undefined8 *)(param_1 + 0x16) = uVar1;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:87257
// ==== 0x437b014  FUN_0437b014  (776 bytes)

void FUN_0437b014(undefined8 *param_1,long param_2,long *param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  long *plVar4;
  long lVar5;
  ulong uVar6;
  char *pcVar7;
  ulong extraout_x1;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  long *plVar11;
  long lVar12;
  undefined1 local_58 [16];
  undefined1 local_48;
  
  if (param_3 == (long *)0x0) {
    pcVar7 = "browser is null";
    uVar3 = 0xd;
    uVar8 = 0xf;
    uVar9 = 0x991;
  }
  else if (*(long *)(param_2 + 0x130) == 0) {
    pcVar7 = "managed webcontents is null";
    uVar3 = 0x25;
    uVar8 = 0x1b;
    uVar9 = 0x995;
  }
  else {
    plVar4 = (long *)(**(code **)(*param_3 + 0x78))(param_3);
    lVar5 = FUN_018efdf0(*(undefined8 *)(*(long *)(DAT_0e481618 + 0x2a0) + 8),
                         *(undefined8 *)(param_2 + 0x130));
    if ((((lVar5 == 0) || (lVar5 = FUN_0031dab4(), lVar5 == 0)) || (*(long *)(lVar5 + 0x28) == 0))
       || (((*(char *)(*(long *)(lVar5 + 0x28) + 4) != '\0' || (*(long *)(lVar5 + 0x30) == 0)) ||
           ((*(long *)(lVar5 + 0x28) == 0 ||
            ((*(char *)(*(long *)(lVar5 + 0x28) + 4) != '\0' ||
             (plVar11 = *(long **)(lVar5 + 0x30),
             plVar11 == (long *)0x0 || plVar11 == (long *)segment_command_00000020.segname)))))))) {
      pcVar7 = "webcontents not attached to any browser";
      uVar3 = 0x25;
      uVar8 = 0x27;
      uVar9 = 0x99e;
    }
    else if (plVar11 == param_3) {
      pcVar7 = "browser equals current browser";
      uVar3 = 0xd;
      uVar8 = 0x1e;
      uVar9 = 0x9a3;
    }
    else {
      lVar12 = plVar11[2];
      lVar5 = (**(code **)(*param_3 + 0x38))(param_3);
      if (lVar12 == lVar5) {
        plVar11 = (long *)(**(code **)(*plVar11 + 0x78))(plVar11);
        uVar6 = FUN_00027de0(plVar11,*(undefined8 *)(param_2 + 0x130));
        if (((plVar11[0x12] == 0) || (iVar2 = FUN_00403d24(*plVar11), (extraout_x1 & 1) == 0)) ||
           (iVar2 < 0)) {
          uVar3 = (*(code *)PTR_FUN_0e2f1ea8)();
          if (((int)uVar6 < 0) || (*(int *)(*plVar11 + 0x18) <= (int)uVar6)) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x437b318);
            (*pcVar1)();
          }
          uVar8 = FUN_003abe30(*plVar11,uVar6 & 0xffffffff);
          FUN_0335ad80(plVar11,uVar8,6,uVar3);
        }
        FUN_01e7c404(plVar11,param_2 + 0xe0);
        lVar5 = FUN_00882ba0(*(undefined8 *)(param_2 + 0x130));
        if (lVar5 != 0) {
          FUN_01bdfd00(lVar5 + 0x48,param_2 + 0x100);
        }
        lVar5 = FUN_000805e0(*(long *)(param_2 + 0x130) + 8,&DAT_0cf208a0);
        if (lVar5 != 0) {
          FUN_04d1bbc0(lVar5 + 0xa0,param_2 + 200);
        }
        if (*(long *)(param_2 + 0x2b8) != 0) {
          FUN_04357fd0();
        }
        lVar5 = FUN_07e1094c(plVar11,uVar6);
        lVar12 = FUN_000805e0(*(long *)(param_2 + 0x130) + 8,&DAT_0e4a2f38);
        if (lVar12 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(0,0x437b324);
          (*pcVar1)();
        }
        FUN_02b4d6c8(*(undefined8 *)(lVar5 + 0x40),0);
        FUN_04353534(param_2 + 0x28,0);
        uVar10 = 9;
        if (*(long *)(*plVar4 + 0x18) != 0) {
          uVar10 = 0;
        }
        local_58[0] = 0;
        local_48 = 0;
        FUN_0422a9e4(plVar4,*(long *)(*plVar4 + 0x18),lVar5,uVar10,local_58,0,0);
        if (*(char *)(param_2 + 0x5c0) != '\0') {
          FUN_01e7c404(plVar11,param_2 + 0xe0);
          FUN_0210cd90(plVar4 + 4,param_2 + 0xe0);
          FUN_0210cc84(param_2 + 0xe0,plVar4);
        }
        FUN_04373cc4(param_2);
        uVar3 = 1;
        goto LAB_0437b080;
      }
      pcVar7 = "browser\'s profile does not equal current browse profile";
      uVar3 = 0xd;
      uVar8 = 0x37;
      uVar9 = 0x9a8;
    }
  }
  uVar3 = FUN_04b37104(uVar3,pcVar7,uVar8,uVar9,
                       "../../arc/browser/web_contents/web_contents_impl.cc");
LAB_0437b080:
  *param_1 = uVar3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147604
// ==== 0x43dcbdc  ArcAIServiceConfiguration::init:displayName:libraryPath:libraryVersion:appEnvironment:fullAppVersion:maxRetries:timeout:error:  (1048 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x54 bytes */

ID ArcAIServiceConfiguration::
   init_displayName_libraryPath_libraryVersion_appEnvironment_fullAppVersion_maxRetries_timeout_error_
             (ID param_1,SEL param_2,long_long param_3,ID param_4,ID param_5,ID param_6,ID param_7,
             ID param_8,int param_9,double param_10,ID *param_11)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  ID local_98;
  class_t *pcStack_90;
  undefined8 local_88;
  cfstringStruct *pcStack_80;
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0xffffffffffffffff) {
    local_88 = *(undefined8 *)PTR__NSLocalizedDescriptionKey_0d826478;
    pcStack_80 = &cf_Invalidtype;
    IVar2 = NSDictionary::dictionaryWithObjects_forKeys_count_
                      ((ID)&_OBJC_CLASS___NSDictionary,"dictionaryWithObjects:forKeys:count:",
                       &pcStack_80,&local_88,1);
    IVar2 = NSError::errorWithDomain_code_userInfo_
                      ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                       &cf_company_thebrowser_Arc,2,IVar2);
    *param_11 = IVar2;
    IVar2 = 0;
  }
  else {
    pcStack_90 = &objc::class_t::ArcAIServiceConfiguration;
    local_98 = param_1;
    IVar2 = NSObject::init((ID)&local_98,"init");
    if (IVar2 != 0) {
      if (*(char *)(IVar2 + 0x98) != '\0') {
        FUN_042a3344(IVar2 + 8);
      }
      *(undefined8 *)(IVar2 + 0x90) = 0;
      *(undefined8 *)(IVar2 + 0x88) = 0;
      *(undefined8 *)(IVar2 + 0x80) = 0;
      *(undefined8 *)(IVar2 + 0x78) = 0;
      *(undefined8 *)(IVar2 + 0x70) = 0;
      *(undefined8 *)(IVar2 + 0x68) = 0;
      *(undefined8 *)(IVar2 + 0x60) = 0;
      *(undefined8 *)(IVar2 + 0x58) = 0;
      *(undefined8 *)(IVar2 + 0x50) = 0;
      *(undefined8 *)(IVar2 + 0x48) = 0;
      *(undefined8 *)(IVar2 + 0x40) = 0;
      *(undefined8 *)(IVar2 + 0x38) = 0;
      *(undefined8 *)(IVar2 + 0x30) = 0;
      *(undefined8 *)(IVar2 + 0x28) = 0;
      *(undefined8 *)(IVar2 + 0x20) = 0;
      *(undefined8 *)(IVar2 + 0x18) = 0;
      *(undefined8 *)(IVar2 + 0x10) = 0;
      *(undefined8 *)(IVar2 + 8) = 0;
      *(undefined1 *)(IVar2 + 0x98) = 1;
      *(int *)(IVar2 + 8) = (int)param_3;
      if (param_4 == 0) {
        local_b0 = 0;
        uStack_a8 = 0;
        local_a0 = 0;
      }
      else {
        FUN_01b356b8(&local_b0,param_4);
      }
      if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
        if (*(char *)(IVar2 + 0x27) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x18) = uStack_a8;
        *(undefined8 *)(IVar2 + 0x10) = local_b0;
        *(undefined8 *)(IVar2 + 0x20) = local_a0;
        IVar3 = _objc_msgSend(param_5,"path");
        if (IVar3 == 0) {
          local_b0 = 0;
          uStack_a8 = 0;
          local_a0 = 0;
        }
        else {
          FUN_01b356b8(&local_b0,IVar3);
        }
        if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
          if (*(char *)(IVar2 + 0x3f) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x28),*(ulong *)(IVar2 + 0x38) & 0x7fffffffffffffff
                        );
          }
          *(undefined8 *)(IVar2 + 0x30) = uStack_a8;
          *(undefined8 *)(IVar2 + 0x28) = local_b0;
          *(undefined8 *)(IVar2 + 0x38) = local_a0;
          if (param_6 == 0) {
            local_b0 = 0;
            uStack_a8 = 0;
            local_a0 = 0;
          }
          else {
            FUN_01b356b8(&local_b0,param_6);
          }
          if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
            if (*(char *)(IVar2 + 0x57) < '\0') {
              FUN_00005810(*(undefined8 *)(IVar2 + 0x40),
                           *(ulong *)(IVar2 + 0x50) & 0x7fffffffffffffff);
            }
            *(undefined8 *)(IVar2 + 0x48) = uStack_a8;
            *(undefined8 *)(IVar2 + 0x40) = local_b0;
            *(undefined8 *)(IVar2 + 0x50) = local_a0;
            if (param_7 == 0) {
              local_b0 = 0;
              uStack_a8 = 0;
              local_a0 = 0;
            }
            else {
              FUN_01b356b8(&local_b0,param_7);
            }
            if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
              if (*(char *)(IVar2 + 0x6f) < '\0') {
                FUN_00005810(*(undefined8 *)(IVar2 + 0x58),
                             *(ulong *)(IVar2 + 0x68) & 0x7fffffffffffffff);
              }
              *(undefined8 *)(IVar2 + 0x60) = uStack_a8;
              *(undefined8 *)(IVar2 + 0x58) = local_b0;
              *(undefined8 *)(IVar2 + 0x68) = local_a0;
              if (param_8 == 0) {
                local_b0 = 0;
                uStack_a8 = 0;
                local_a0 = 0;
              }
              else {
                FUN_01b356b8(&local_b0,param_8);
              }
              if ((*(byte *)(IVar2 + 0x98) & 1) != 0) {
                if (*(char *)(IVar2 + 0x87) < '\0') {
                  FUN_00005810(*(undefined8 *)(IVar2 + 0x70),
                               *(ulong *)(IVar2 + 0x80) & 0x7fffffffffffffff);
                  *(undefined8 *)(IVar2 + 0x78) = uStack_a8;
                  *(undefined8 *)(IVar2 + 0x70) = local_b0;
                  *(undefined8 *)(IVar2 + 0x80) = local_a0;
                  if ((*(byte *)(IVar2 + 0x98) & 1) == 0) goto LAB_043dcfd8;
                }
                else {
                  *(undefined8 *)(IVar2 + 0x78) = uStack_a8;
                  *(undefined8 *)(IVar2 + 0x70) = local_b0;
                  *(undefined8 *)(IVar2 + 0x80) = local_a0;
                }
                *(char *)(IVar2 + 0x88) = (char)param_9;
                *(double *)(IVar2 + 0x90) = param_10;
                goto LAB_043dce78;
              }
            }
          }
        }
      }
LAB_043dcfd8:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x43dcfdc);
      (*pcVar1)();
    }
  }
LAB_043dce78:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147781
// ==== 0x43dcff4  ArcAIServiceConfiguration::type  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcAIServiceConfiguration::type(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    return (long)*(int *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43dd008);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147799
// ==== 0x43dd008  ArcAIServiceConfiguration::displayName  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIServiceConfiguration::displayName(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x27);
    plVar1 = (long *)*(long *)(param_1 + 0x10);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x10);
    }
    lVar2 = *(long *)(param_1 + 0x18);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43dd038);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147831
// ==== 0x43dd038  ArcAIServiceConfiguration::libraryPath  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIServiceConfiguration::libraryPath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x3f);
    plVar1 = (long *)*(long *)(param_1 + 0x28);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x28);
    }
    lVar2 = *(long *)(param_1 + 0x30);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43dd068);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147863
// ==== 0x43dd068  ArcAIServiceConfiguration::libraryVersion  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIServiceConfiguration::libraryVersion(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x57);
    plVar1 = (long *)*(long *)(param_1 + 0x40);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x40);
    }
    lVar2 = *(long *)(param_1 + 0x48);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43dd098);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147895
// ==== 0x43dd098  ArcAIServiceConfiguration::appEnvironment  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIServiceConfiguration::appEnvironment(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x6f);
    plVar1 = (long *)*(long *)(param_1 + 0x58);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x58);
    }
    lVar2 = *(long *)(param_1 + 0x60);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43dd0c8);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147927
// ==== 0x43dd0c8  ArcAIServiceConfiguration::fullAppVersion  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIServiceConfiguration::fullAppVersion(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x87);
    plVar1 = (long *)*(long *)(param_1 + 0x70);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x70);
    }
    lVar2 = *(long *)(param_1 + 0x78);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43dd0f8);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147959
// ==== 0x43dd0f8  ArcAIServiceConfiguration::maxRetries  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAIServiceConfiguration::maxRetries(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x98) & 1) != 0) {
    return (int)*(char *)(param_1 + 0x88);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43dd10c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147977
// ==== 0x43dd10c  ArcAIServiceConfiguration::timeout  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIServiceConfiguration::timeout(int param_1)

{
  code *pcVar1;
  
  if ((*(byte *)((ulong)(uint)param_1 + 0x98) & 1) != 0) {
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43dd120);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:147995
// ==== 0x43dd120  ArcAIServiceConfiguration::nativeAIServiceConfig  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIServiceConfiguration::nativeAIServiceConfig
               (optional<arc::AIServiceConfiguration> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 7) = 0;
  if (*(char *)(param_1 + 0x98) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AIServiceConfiguration> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x43dd168);
      (*pcVar1)();
    }
    FUN_043da310(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 7) = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:148021
// ==== 0x43dd184  ArcAIServiceConfiguration::setNativeAIServiceConfig:  (540 bytes)

/* Function Stack Size: 0xa8 bytes */

void ArcAIServiceConfiguration::setNativeAIServiceConfig_
               (ID param_1,SEL param_2,optional<arc::AIServiceConfiguration> *param_3)

{
  ulong uVar1;
  char cVar2;
  byte bVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  cVar2 = *(char *)(param_1 + 0x98);
  if (cVar2 == *(char *)((long)&param_3[1].field0_0x0 + 7)) {
    if (cVar2 != '\0') {
      puVar5 = (undefined8 *)((long)&param_3->field0_0x0 + 8);
      *(int *)(param_1 + 8) = (param_3->field0_0x0).field1.field0_0x0;
      if ((optional<arc::AIServiceConfiguration> *)(param_1 + 8) != param_3) {
        bVar3 = *(byte *)((long)&param_3->field0_0x0 + 0x1f);
        if (*(char *)(param_1 + 0x27) < '\0') {
          uVar1 = *(ulong *)((long)&param_3->field0_0x0 + 0x10);
          puVar4 = *(undefined8 **)((long)&param_3->field0_0x0 + 8);
          if (-1 < (char)bVar3) {
            uVar1 = (ulong)bVar3;
            puVar4 = puVar5;
          }
          FUN_00359ad8(param_1 + 0x10,puVar4,uVar1);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_1 + 0x10,*(undefined8 *)((long)&param_3->field0_0x0 + 8),
                       *(undefined8 *)((long)&param_3->field0_0x0 + 0x10));
        }
        else {
          uVar7 = *(undefined8 *)((long)&param_3->field0_0x0 + 0x10);
          uVar6 = *puVar5;
          *(undefined8 *)(param_1 + 0x20) = *(undefined8 *)((long)&param_3->field0_0x0 + 0x18);
          *(undefined8 *)(param_1 + 0x18) = uVar7;
          *(undefined8 *)(param_1 + 0x10) = uVar6;
        }
        puVar5 = (undefined8 *)((long)&param_3->field0_0x0 + 0x20);
        bVar3 = *(byte *)((long)&param_3->field0_0x0 + 0x37);
        if (*(char *)(param_1 + 0x3f) < '\0') {
          uVar1 = *(ulong *)((long)&param_3->field0_0x0 + 0x28);
          puVar4 = *(undefined8 **)((long)&param_3->field0_0x0 + 0x20);
          if (-1 < (char)bVar3) {
            uVar1 = (ulong)bVar3;
            puVar4 = puVar5;
          }
          FUN_00359ad8(param_1 + 0x28,puVar4,uVar1);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_1 + 0x28,*(undefined8 *)((long)&param_3->field0_0x0 + 0x20),
                       *(undefined8 *)((long)&param_3->field0_0x0 + 0x28));
        }
        else {
          uVar7 = *(undefined8 *)((long)&param_3->field0_0x0 + 0x28);
          uVar6 = *puVar5;
          *(undefined8 *)(param_1 + 0x38) = *(undefined8 *)((long)&param_3->field0_0x0 + 0x30);
          *(undefined8 *)(param_1 + 0x30) = uVar7;
          *(undefined8 *)(param_1 + 0x28) = uVar6;
        }
        puVar5 = (undefined8 *)((long)&param_3->field0_0x0 + 0x38);
        bVar3 = *(byte *)((long)&param_3->field0_0x0 + 0x4f);
        if (*(char *)(param_1 + 0x57) < '\0') {
          uVar1 = *(ulong *)((long)&param_3->field0_0x0 + 0x40);
          puVar4 = *(undefined8 **)((long)&param_3->field0_0x0 + 0x38);
          if (-1 < (char)bVar3) {
            uVar1 = (ulong)bVar3;
            puVar4 = puVar5;
          }
          FUN_00359ad8(param_1 + 0x40,puVar4,uVar1);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_1 + 0x40,*(undefined8 *)((long)&param_3->field0_0x0 + 0x38),
                       *(undefined8 *)((long)&param_3->field0_0x0 + 0x40));
        }
        else {
          uVar7 = *(undefined8 *)((long)&param_3->field0_0x0 + 0x40);
          uVar6 = *puVar5;
          *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)((long)&param_3->field0_0x0 + 0x48);
          *(undefined8 *)(param_1 + 0x48) = uVar7;
          *(undefined8 *)(param_1 + 0x40) = uVar6;
        }
        puVar5 = (undefined8 *)((long)&param_3->field0_0x0 + 0x50);
        bVar3 = *(byte *)((long)&param_3->field0_0x0 + 0x67);
        if (*(char *)(param_1 + 0x6f) < '\0') {
          uVar1 = *(ulong *)((long)&param_3->field0_0x0 + 0x58);
          puVar4 = *(undefined8 **)((long)&param_3->field0_0x0 + 0x50);
          if (-1 < (char)bVar3) {
            uVar1 = (ulong)bVar3;
            puVar4 = puVar5;
          }
          FUN_00359ad8(param_1 + 0x58,puVar4,uVar1);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_1 + 0x58,*(undefined8 *)((long)&param_3->field0_0x0 + 0x50),
                       *(undefined8 *)((long)&param_3->field0_0x0 + 0x58));
        }
        else {
          uVar7 = *(undefined8 *)((long)&param_3->field0_0x0 + 0x58);
          uVar6 = *puVar5;
          *(undefined8 *)(param_1 + 0x68) = *(undefined8 *)((long)&param_3->field0_0x0 + 0x60);
          *(undefined8 *)(param_1 + 0x60) = uVar7;
          *(undefined8 *)(param_1 + 0x58) = uVar6;
        }
        puVar5 = (undefined8 *)((long)&param_3->field0_0x0 + 0x68);
        bVar3 = *(byte *)((long)&param_3->field0_0x0 + 0x7f);
        if (*(char *)(param_1 + 0x87) < '\0') {
          uVar1 = *(ulong *)((long)&param_3->field0_0x0 + 0x70);
          puVar4 = *(undefined8 **)((long)&param_3->field0_0x0 + 0x68);
          if (-1 < (char)bVar3) {
            uVar1 = (ulong)bVar3;
            puVar4 = puVar5;
          }
          FUN_00359ad8(param_1 + 0x70,puVar4,uVar1);
        }
        else if ((char)bVar3 < '\0') {
          FUN_00359998(param_1 + 0x70,*(undefined8 *)((long)&param_3->field0_0x0 + 0x68),
                       *(undefined8 *)((long)&param_3->field0_0x0 + 0x70));
        }
        else {
          uVar7 = *(undefined8 *)((long)&param_3->field0_0x0 + 0x70);
          uVar6 = *puVar5;
          *(undefined8 *)(param_1 + 0x80) = *(undefined8 *)((long)&param_3->field0_0x0 + 0x78);
          *(undefined8 *)(param_1 + 0x78) = uVar7;
          *(undefined8 *)(param_1 + 0x70) = uVar6;
        }
      }
      uVar6 = *(undefined8 *)((long)&(param_3->field0_0x0).field1.field7_0x7d + 3);
      *(undefined8 *)(param_1 + 0x90) = *(undefined8 *)((long)&param_3->field1_0x85 + 3);
      *(undefined8 *)(param_1 + 0x88) = uVar6;
    }
  }
  else if (cVar2 == '\0') {
    FUN_043da310(param_1 + 8,param_3);
    *(undefined1 *)(param_1 + 0x98) = 1;
  }
  else {
    FUN_042a3344(param_1 + 8);
    *(undefined1 *)(param_1 + 0x98) = 0;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:148170
// ==== 0x43dd3a0  ArcAIServiceConfiguration::.cxx_destruct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIServiceConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x98) == '\0') {
    return;
  }
  FUN_042a3344(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:148185
// ==== 0x43dd3b4  ArcExternalProtocolDialog::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExternalProtocolDialog::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x98) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:148225
// ==== 0x43dd450  ICloudKeychainPresentationDelegate::presentationAnchorForAuthorizationController:  (8 bytes)

/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

ID ICloudKeychainPresentationDelegate::presentationAnchorForAuthorizationController_
             (ID param_1,SEL param_2,ID param_3)

{
  return *(ID *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:148294
// ==== 0x43dd5a4  WebNNMLFeatureProvider::.cxx_destruct  (48 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void WebNNMLFeatureProvider::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149031
// ==== 0x43de5e8  ArcHistoryQueryOptions::.cxx_destruct  (188 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcHistoryQueryOptions::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar2;
  
  plVar2 = *(long **)(param_1 + 0x10);
  if (plVar2 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar2[1];
    plVar2[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar2 + 0x10))(plVar2);
      if (plVar2[2] != 0) {
        LOAcquire();
        lVar1 = plVar2[2];
        plVar2[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043de6a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(plVar2);
        return;
      }
      FUN_00005810(plVar2,0x20);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149286
// ==== 0x43ded20  ArcAIInferenceModel::.cxx_destruct  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIInferenceModel::_cxx_destruct(ID param_1,SEL param_2)

{
  if ((*(char *)(param_1 + 0x28) != '\0') && (*(char *)(param_1 + 0x27) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(ulong *)(param_1 + 0x20) & 0x7fffffffffffffff);
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149333
// ==== 0x43ded78  ArcHSLAMatrix::initWithMatrix:  (132 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcHSLAMatrix::initWithMatrix_(ID param_1,SEL param_2,float *param_3)

{
  code *pcVar1;
  int iVar2;
  ID IVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcHSLAMatrix;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    iVar2 = FUN_00122014(param_3,0x50,4);
    if (iVar2 != 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x43dedfc);
      (*pcVar1)();
    }
    uVar4 = *(undefined8 *)param_3;
    uVar6 = *(undefined8 *)(param_3 + 6);
    uVar5 = *(undefined8 *)(param_3 + 4);
    uVar8 = *(undefined8 *)(param_3 + 10);
    uVar7 = *(undefined8 *)(param_3 + 8);
    uVar10 = *(undefined8 *)(param_3 + 0xe);
    uVar9 = *(undefined8 *)(param_3 + 0xc);
    uVar12 = *(undefined8 *)(param_3 + 0x12);
    uVar11 = *(undefined8 *)(param_3 + 0x10);
    *(undefined8 *)(IVar3 + 0x10) = *(undefined8 *)(param_3 + 2);
    *(undefined8 *)(IVar3 + 8) = uVar4;
    *(undefined8 *)(IVar3 + 0x20) = uVar6;
    *(undefined8 *)(IVar3 + 0x18) = uVar5;
    *(undefined8 *)(IVar3 + 0x30) = uVar8;
    *(undefined8 *)(IVar3 + 0x28) = uVar7;
    *(undefined8 *)(IVar3 + 0x40) = uVar10;
    *(undefined8 *)(IVar3 + 0x38) = uVar9;
    *(undefined8 *)(IVar3 + 0x50) = uVar12;
    *(undefined8 *)(IVar3 + 0x48) = uVar11;
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149389
// ==== 0x43dee04  ArcHSLAMatrix::m0  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcHSLAMatrix::m0(void *param_1,void *param_2,int param_3,char *param_4,int param_5,int param_6
                      )

{
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149401
// ==== 0x43dee0c  ArcHSLAMatrix::m1  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m1(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0xc);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149412
// ==== 0x43dee14  ArcHSLAMatrix::m3  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m3(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x14);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149423
// ==== 0x43dee1c  CursorAccessibilityScaleFactorNotifier::scaleFactor  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float CursorAccessibilityScaleFactorNotifier::scaleFactor(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x18);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149434
// ==== 0x43dee24  ArcHSLAMatrix::m5  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m5(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x1c);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149445
// ==== 0x43dee2c  ArcHSLAMatrix::m6  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m6(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x20);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149456
// ==== 0x43dee34  ArcHSLAMatrix::m7  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m7(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x24);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149467
// ==== 0x43dee3c  ArcHSLAMatrix::m8  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m8(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x28);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149478
// ==== 0x43dee44  ArcHSLAMatrix::m9  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m9(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x2c);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149489
// ==== 0x43dee4c  ArcHSLAMatrix::m10  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m10(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x30);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149500
// ==== 0x43dee54  ArcHSLAMatrix::m11  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m11(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x34);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149511
// ==== 0x43dee5c  ArcHSLAMatrix::m12  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m12(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x38);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149522
// ==== 0x43dee64  ArcHSLAMatrix::m13  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m13(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x3c);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149533
// ==== 0x43dee6c  ArcHSLAMatrix::m14  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m14(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x40);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149544
// ==== 0x43dee74  ArcHSLAMatrix::m15  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m15(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x44);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149555
// ==== 0x43dee7c  ArcHSLAMatrix::m16  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m16(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x48);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149566
// ==== 0x43dee84  ArcHSLAMatrix::m17  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m17(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x4c);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149577
// ==== 0x43dee8c  ArcHSLAMatrix::m18  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m18(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x50);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149588
// ==== 0x43dee94  ArcHSLAMatrix::m19  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m19(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x54);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149599
// ==== 0x43dee9c  ArcHSLAMatrix::hslaMatrix  (36 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcHSLAMatrix::hslaMatrix(HSLAMatrix *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  
  uVar6 = *(undefined8 *)(param_1 + 0x50);
  uVar5 = *(undefined8 *)(param_1 + 0x48);
  uVar2 = *(undefined8 *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  uVar4 = *(undefined8 *)(param_1 + 0x20);
  uVar3 = *(undefined8 *)(param_1 + 0x18);
  __return_storage_ptr__->field2_0x8 = (float)(int)uVar2;
  __return_storage_ptr__->field3_0xc = (float)(int)((ulong)uVar2 >> 0x20);
  __return_storage_ptr__->field0_0x0 = (float)(int)uVar1;
  __return_storage_ptr__->field1_0x4 = (float)(int)((ulong)uVar1 >> 0x20);
  __return_storage_ptr__->field6_0x18 = (float)(int)uVar4;
  __return_storage_ptr__->field7_0x1c = (float)(int)((ulong)uVar4 >> 0x20);
  __return_storage_ptr__->field4_0x10 = (float)(int)uVar3;
  __return_storage_ptr__->field5_0x14 = (float)(int)((ulong)uVar3 >> 0x20);
  uVar2 = *(undefined8 *)(param_1 + 0x30);
  uVar1 = *(undefined8 *)(param_1 + 0x28);
  uVar4 = *(undefined8 *)(param_1 + 0x40);
  uVar3 = *(undefined8 *)(param_1 + 0x38);
  __return_storage_ptr__->field10_0x28 = (float)(int)uVar2;
  __return_storage_ptr__->field11_0x2c = (float)(int)((ulong)uVar2 >> 0x20);
  __return_storage_ptr__->field8_0x20 = (float)(int)uVar1;
  __return_storage_ptr__->field9_0x24 = (float)(int)((ulong)uVar1 >> 0x20);
  __return_storage_ptr__->field14_0x38 = (float)(int)uVar4;
  __return_storage_ptr__->field15_0x3c = (float)(int)((ulong)uVar4 >> 0x20);
  __return_storage_ptr__->field12_0x30 = (float)(int)uVar3;
  __return_storage_ptr__->field13_0x34 = (float)(int)((ulong)uVar3 >> 0x20);
  __return_storage_ptr__->field18_0x48 = (float)(int)uVar6;
  __return_storage_ptr__->field19_0x4c = (float)(int)((ulong)uVar6 >> 0x20);
  __return_storage_ptr__->field16_0x40 = (float)(int)uVar5;
  __return_storage_ptr__->field17_0x44 = (float)(int)((ulong)uVar5 >> 0x20);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149647
// ==== 0x43deec0  ArcHSLAMatrix::setHslaMatrix:  (36 bytes)

/* Function Stack Size: 0x60 bytes */

void ArcHSLAMatrix::setHslaMatrix_(ID param_1,SEL param_2,HSLAMatrix *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  uVar2 = *(undefined8 *)&param_3->field2_0x8;
  uVar1 = *(undefined8 *)param_3;
  uVar4 = *(undefined8 *)&param_3->field6_0x18;
  uVar3 = *(undefined8 *)&param_3->field4_0x10;
  uVar6 = *(undefined8 *)&param_3->field10_0x28;
  uVar5 = *(undefined8 *)&param_3->field8_0x20;
  uVar7 = *(undefined8 *)&param_3->field12_0x30;
  *(undefined8 *)(param_1 + 0x40) = *(undefined8 *)&param_3->field14_0x38;
  *(undefined8 *)(param_1 + 0x38) = uVar7;
  *(undefined8 *)(param_1 + 0x30) = uVar6;
  *(undefined8 *)(param_1 + 0x28) = uVar5;
  *(undefined8 *)(param_1 + 0x20) = uVar4;
  *(undefined8 *)(param_1 + 0x18) = uVar3;
  *(undefined8 *)(param_1 + 0x10) = uVar2;
  *(undefined8 *)(param_1 + 8) = uVar1;
  uVar1 = *(undefined8 *)&param_3->field16_0x40;
  *(undefined8 *)(param_1 + 0x50) = *(undefined8 *)&param_3->field18_0x48;
  *(undefined8 *)(param_1 + 0x48) = uVar1;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149707
// ==== 0x43def08  ArcKeyEventData::initWithEventType:domCode:key:windowsVirtualKeyCode:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x2c bytes */

ID ArcKeyEventData::initWithEventType_domCode_key_windowsVirtualKeyCode_
             (ID param_1,SEL param_2,unsigned_long_long param_3,ID param_4,ID param_5,int param_6)

{
  ID IVar1;
  ID IVar2;
  undefined8 uVar3;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcKeyEventData;
  local_50 = param_1;
  IVar1 = NSObject::init((ID)&local_50,"init");
  if (IVar1 != 0) {
    *(unsigned_long_long *)(IVar1 + 0x10) = param_3;
    IVar2 = _objc_msgSend(param_4,"copy");
    uVar3 = *(undefined8 *)(IVar1 + 0x18);
    *(ID *)(IVar1 + 0x18) = IVar2;
    IVar2 = _objc_msgSend(param_5,"copy",uVar3);
    *(ID *)(IVar1 + 0x20) = IVar2;
    *(int *)(IVar1 + 8) = param_6;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149739
// ==== 0x43defdc  ArcKeyEventData::.cxx_destruct  (48 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcKeyEventData::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149863
// ==== 0x43df298  ArcBackForwardListItem::initWithBackForwardListItem:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBackForwardListItem::initWithBackForwardListItem_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBackForwardListItem;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_043df2f8;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043df2f4;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_043df2f4:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_043df2f8:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149922
// ==== 0x43df3bc  ArcImportSourceProfile::name  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImportSourceProfile::name(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar5 = *(undefined8 **)(param_1 + 8);
    if (*(char *)((long)puVar5 + 0x17) < '\0') {
      FUN_00054420(&local_40,*puVar5,puVar5[1]);
    }
    else {
      lStack_38 = puVar5[1];
      local_40 = (undefined1 *)*puVar5;
      local_30 = puVar5[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar4 = FUN_01d9fb90(ppuVar1,lVar2);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x43df458);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:149971
// ==== 0x43df47c  ArcBackForwardListItem::url  (132 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBackForwardListItem::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SEL extraout_x1;
  long lVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x2f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x18),*(undefined8 *)(lVar3 + 0x20));
      param_2 = extraout_x1;
    }
    else {
      uStack_38 = *(undefined8 *)(lVar3 + 0x20);
      local_40 = *(undefined8 *)(lVar3 + 0x18);
      local_30 = *(ulong *)(lVar3 + 0x28);
    }
    IVar2 = FUN_043abd34(&local_40,param_2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43df500);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150012
// ==== 0x43df524  ArcBackForwardListItem::isValid  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBackForwardListItem::isValid(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x30);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43df53c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150030
// ==== 0x43df53c  ArcWebContentsAIAgent::web_contents_ai_agent  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsAIAgent::web_contents_ai_agent
               (optional<arc::WebContents::AIAgent> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x18) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::WebContents::AIAgent> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x43df57c);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x10);
    pIVar4 = *(Impl **)(param_1 + 8);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150066
// ==== 0x43df57c  ArcSiteSettingInfo::setInfo:  (288 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcSiteSettingInfo::setInfo_(ID param_1,SEL param_2,optional<arc::SiteSettingInfo> *param_3)

{
  char cVar1;
  long lVar2;
  __shared_weak_count *p_Var3;
  code *pcVar4;
  long *plVar5;
  
  cVar1 = *(char *)(param_1 + 0x18);
  if (cVar1 == (char)param_3->field1_0x10) {
    if (cVar1 == '\0') {
      return;
    }
    FUN_04331e20(param_1 + 8,param_3);
    return;
  }
  if (cVar1 == '\0') {
    *(Impl **)(param_1 + 8) = (param_3->field0_0x0).field1.field0_0x0.field0_0x0;
    p_Var3 = (param_3->field0_0x0).field1.field0_0x0.field1_0x8;
    *(__shared_weak_count **)(param_1 + 0x10) = p_Var3;
    if (p_Var3 != (__shared_weak_count *)0x0) {
      *(long *)(p_Var3 + 8) = *(long *)(p_Var3 + 8) + 1;
    }
    *(undefined1 *)(param_1 + 0x18) = 1;
    return;
  }
  plVar5 = *(long **)(param_1 + 0x10);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar5[1];
    plVar5[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar2 = plVar5[2];
        plVar5[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043df5d8;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_043df5d8:
  *(undefined1 *)(param_1 + 0x18) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150127
// ==== 0x43df69c  ArcWebContentsAIAgent::.cxx_destruct  (196 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsAIAgent::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar2;
  
  if ((*(char *)(param_1 + 0x18) != '\0') &&
     (plVar2 = *(long **)(param_1 + 0x10), plVar2 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar2[1];
    plVar2[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar2 + 0x10))(plVar2);
      if (plVar2[2] != 0) {
        LOAcquire();
        lVar1 = plVar2[2];
        plVar2[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043df75c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(plVar2);
        return;
      }
      FUN_00005810(plVar2,0x20);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150171
// ==== 0x43df760  ArcWebContentsAIAgent::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcWebContentsAIAgent::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x18) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150311
// ==== 0x43dfab0  ArcWebContentsMediaSession::nativeMediaSession  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsMediaSession::nativeMediaSession
               (optional<arc::WebContents::MediaSession> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x40) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::WebContents::MediaSession> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x43dfaf0);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x38);
    pIVar4 = *(Impl **)(param_1 + 0x30);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x38);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150469
// ==== 0x43dfd40  ArcBookmarkTreeNode::initWithBookmarkTreeNode:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBookmarkTreeNode::initWithBookmarkTreeNode_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBookmarkTreeNode;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_043dfda0;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043dfd9c;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_043dfd9c:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_043dfda0:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150528
// ==== 0x43dfe80  ArcBookmarkTreeNode::nodeType  (40 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcBookmarkTreeNode::nodeType(ID param_1,SEL param_2)

{
  uint uVar1;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    return 0;
  }
  uVar1 = *(uint *)(*(long *)(param_1 + 8) + 0x28);
  if (4 < uVar1 - 1) {
    uVar1 = 0;
  }
  return (ulong)uVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150548
// ==== 0x43dfea8  ArcBookmarkTreeNode::isFolder  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBookmarkTreeNode::isFolder(ID param_1,SEL param_2)

{
  byte bVar1;
  
  bVar1 = 0;
  if (*(char *)(param_1 + 0x18) != '\0') {
    bVar1 = *(byte *)(*(long *)(param_1 + 8) + 0x2c);
  }
  return bVar1 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150565
// ==== 0x43dfec0  ArcBookmarkTreeNode::isUrl  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBookmarkTreeNode::isUrl(ID param_1,SEL param_2)

{
  byte bVar1;
  
  bVar1 = 0;
  if (*(char *)(param_1 + 0x18) != '\0') {
    bVar1 = *(byte *)(*(long *)(param_1 + 8) + 0x2d);
  }
  return bVar1 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150582
// ==== 0x43dfed8  ArcBookmarkTreeNode::nodeID  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::nodeID(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  cfstringStruct *pcVar2;
  long lVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    pcVar2 = &cf___;
  }
  else {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x5f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x48),*(undefined8 *)(lVar3 + 0x50));
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0x50);
      local_40 = *(undefined1 **)(lVar3 + 0x48);
      local_30 = *(ulong *)(lVar3 + 0x58);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    pcVar2 = (cfstringStruct *)FUN_01c391bc(ppuVar1,lVar3);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return (ID)pcVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150629
// ==== 0x43dffa0  ArcBookmarkTreeNode::uuid  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::uuid(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  cfstringStruct *pcVar2;
  long lVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    pcVar2 = &cf___;
  }
  else {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x47) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x30),*(undefined8 *)(lVar3 + 0x38));
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0x38);
      local_40 = *(undefined1 **)(lVar3 + 0x30);
      local_30 = *(ulong *)(lVar3 + 0x40);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    pcVar2 = (cfstringStruct *)FUN_01c391bc(ppuVar1,lVar3);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return (ID)pcVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150676
// ==== 0x43e0068  ArcBookmarkTreeNode::parentNodeID  (264 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::parentNodeID(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x77) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x60),*(undefined8 *)(lVar4 + 0x68));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x68);
      local_40 = *(undefined1 **)(lVar4 + 0x60);
      local_30 = *(ulong *)(lVar4 + 0x70);
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    if ((long)local_30._7_1_ < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    if (lVar4 != 0) {
      if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x43e0170);
        (*pcVar2)();
      }
      lVar4 = *(long *)(param_1 + 8);
      if (*(char *)(lVar4 + 0x77) < '\0') {
        FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x60),*(undefined8 *)(lVar4 + 0x68));
      }
      else {
        lStack_38 = *(long *)(lVar4 + 0x68);
        local_40 = *(undefined1 **)(lVar4 + 0x60);
        local_30 = *(ulong *)(lVar4 + 0x70);
      }
      ppuVar1 = (undefined1 **)local_40;
      if (-1 < (long)local_30._7_1_) {
        ppuVar1 = &local_40;
      }
      lVar4 = lStack_38;
      if (-1 < (long)local_30) {
        lVar4 = (long)local_30._7_1_;
      }
      IVar3 = FUN_01c391bc(ppuVar1,lVar4);
      if (-1 < (long)local_30) {
        return IVar3;
      }
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      return IVar3;
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150746
// ==== 0x43e0194  ArcBookmarkTreeNode::index  (40 bytes)

/* Function Stack Size: 0x10 bytes */

char * ArcBookmarkTreeNode::index(char *param_1,int param_2)

{
  uint uVar1;
  
  if (param_1[0x18] == '\0') {
    return &DAT_ffffffff;
  }
  uVar1 = *(uint *)(*(long *)(param_1 + 8) + 0x78);
  if ((*(uint *)(*(long *)(param_1 + 8) + 0x80) & 1) == 0) {
    uVar1 = 0xffffffff;
  }
  return (char *)(ulong)uVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150766
// ==== 0x43e01bc  ArcBookmarkTreeNode::url  (240 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  long lVar3;
  undefined8 local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0x9f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x88),*(undefined8 *)(lVar3 + 0x90));
      param_2 = extraout_x1;
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0x90);
      local_40 = *(undefined8 *)(lVar3 + 0x88);
      local_30 = *(ulong *)(lVar3 + 0x98);
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    if ((long)local_30._7_1_ < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      param_2 = extraout_x1_00;
    }
    if (lVar3 != 0) {
      if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x43e02ac);
        (*pcVar1)();
      }
      lVar3 = *(long *)(param_1 + 8);
      if (*(char *)(lVar3 + 0x9f) < '\0') {
        FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0x88),*(undefined8 *)(lVar3 + 0x90));
        param_2 = extraout_x1_01;
      }
      else {
        lStack_38 = *(long *)(lVar3 + 0x90);
        local_40 = *(undefined8 *)(lVar3 + 0x88);
        local_30 = *(ulong *)(lVar3 + 0x98);
      }
      IVar2 = FUN_043abd34(&local_40,param_2);
      if (-1 < (long)local_30) {
        return IVar2;
      }
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      return IVar2;
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150833
// ==== 0x43e02d0  ArcBookmarkTreeNode::title  (264 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::title(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0xb7) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0xa0),*(undefined8 *)(lVar4 + 0xa8));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0xa8);
      local_40 = *(undefined1 **)(lVar4 + 0xa0);
      local_30 = *(long *)(lVar4 + 0xb0);
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    if ((long)local_30._7_1_ < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    if (lVar4 != 0) {
      if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x43e03d8);
        (*pcVar2)();
      }
      lVar4 = *(long *)(param_1 + 8);
      if (*(char *)(lVar4 + 0xb7) < '\0') {
        FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0xa0),*(undefined8 *)(lVar4 + 0xa8));
      }
      else {
        lStack_38 = *(long *)(lVar4 + 0xa8);
        local_40 = *(undefined1 **)(lVar4 + 0xa0);
        local_30 = *(long *)(lVar4 + 0xb0);
      }
      ppuVar1 = (undefined1 **)local_40;
      if (-1 < (long)local_30._7_1_) {
        ppuVar1 = &local_40;
      }
      lVar4 = lStack_38;
      if (-1 < local_30) {
        lVar4 = (long)local_30._7_1_;
      }
      IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
      if (-1 < local_30) {
        return IVar3;
      }
      FUN_00005810(local_40,local_30 << 1);
      return IVar3;
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150903
// ==== 0x43e03fc  ArcBookmarkTreeNode::faviconUrl  (240 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::faviconUrl(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  long lVar3;
  undefined8 local_40;
  long lStack_38;
  undefined8 local_30;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lVar3 = *(long *)(param_1 + 8);
    if (*(char *)(lVar3 + 0xcf) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0xb8),*(undefined8 *)(lVar3 + 0xc0));
      param_2 = extraout_x1;
    }
    else {
      lStack_38 = *(long *)(lVar3 + 0xc0);
      local_40 = *(undefined8 *)(lVar3 + 0xb8);
      local_30 = *(ulong *)(lVar3 + 200);
    }
    lVar3 = lStack_38;
    if (-1 < (long)local_30) {
      lVar3 = (long)local_30._7_1_;
    }
    if ((long)local_30._7_1_ < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      param_2 = extraout_x1_00;
    }
    if (lVar3 != 0) {
      if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x43e04ec);
        (*pcVar1)();
      }
      lVar3 = *(long *)(param_1 + 8);
      if (*(char *)(lVar3 + 0xcf) < '\0') {
        FUN_00060a60(&local_40,*(undefined8 *)(lVar3 + 0xb8),*(undefined8 *)(lVar3 + 0xc0));
        param_2 = extraout_x1_01;
      }
      else {
        lStack_38 = *(long *)(lVar3 + 0xc0);
        local_40 = *(undefined8 *)(lVar3 + 0xb8);
        local_30 = *(ulong *)(lVar3 + 200);
      }
      IVar2 = FUN_043abd34(&local_40,param_2);
      if (-1 < (long)local_30) {
        return IVar2;
      }
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      return IVar2;
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150970
// ==== 0x43e0510  ArcBookmarkTreeNode::dateAdded  (84 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::dateAdded(ID param_1,SEL param_2)

{
  ID IVar1;
  undefined8 uVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = *(undefined8 *)(*(long *)(param_1 + 8) + 0xd0);
  }
  IVar1 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",uVar2);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:150995
// ==== 0x43e0564  ArcBookmarkTreeNode::dateFolderModified  (72 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::dateFolderModified(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((*(char *)(param_1 + 0x18) != '\0') && (*(char *)(*(long *)(param_1 + 8) + 0x2c) != '\0')) {
    IVar1 = NSDate::dateWithTimeIntervalSince1970_
                      ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",
                       *(undefined8 *)(*(long *)(param_1 + 8) + 0xd8));
    return IVar1;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151017
// ==== 0x43e05ac  ArcBookmarkTreeNode::isValid  (48 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBookmarkTreeNode::isValid(ID param_1,SEL param_2)

{
  long lVar1;
  
  if (*(char *)(param_1 + 0x18) == '\0') {
    return 0;
  }
  lVar1 = FUN_042affa0(*(undefined8 *)(param_1 + 8));
  return (bool)(lVar1 != 0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151034
// ==== 0x43e05dc  ArcBookmarkTreeNode::liveChildren  (516 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBookmarkTreeNode::liveChildren(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  ID IVar4;
  ID IVar5;
  code *pcVar6;
  long *plVar7;
  long lVar8;
  const_void__ local_80;
  long lStack_78;
  long local_70;
  ulong local_68;
  
  if (*(char *)(param_1 + 0x18) != '\0') {
    lStack_78 = 0;
    local_80 = (const_void__)0x0;
    local_68 = 0;
    local_70 = 0;
    FUN_042b00f4(&local_80,*(undefined8 *)(param_1 + 8));
    if ((char)local_68 != '\0') {
      IVar4 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
      if ((local_68 & 1) != 0) {
        pvVar1 = local_80;
        for (lVar8 = lStack_78 << 4; lVar8 != 0; lVar8 = lVar8 + -0x10) {
          IVar5 = _objc_alloc(&objc::class_t::ArcBookmarkTreeNode);
          IVar5 = initWithBookmarkTreeNode_(IVar5,"initWithBookmarkTreeNode:",pvVar1);
          NSMutableArray::addObject_(IVar4,"addObject:",IVar5);
          pvVar1 = (const_void__)((long)pvVar1 + 0x10);
        }
        IVar4 = NSMutableArray::copy(IVar4,"copy");
        pvVar1 = local_80;
        if ((char)local_68 == '\0') {
          return IVar4;
        }
        if (local_80 == (const_void__)0x0) {
          return IVar4;
        }
        if (lStack_78 * 0x10 != 0) {
          lVar8 = lStack_78 * -0x10;
          pvVar2 = (const_void__)(lStack_78 * 0x10 + (long)local_80);
          do {
            if ((const_void__)((long)pvVar2 + -0x10) == (const_void__)0x0) goto LAB_043e07dc;
            plVar7 = *(long **)((long)pvVar2 + -8);
            if (plVar7 != (long *)0x0) {
              LOAcquire();
              lVar3 = plVar7[1];
              plVar7[1] = lVar3 + -1;
              LORelease();
              if (lVar3 == 0) {
                (**(code **)(*plVar7 + 0x10))(plVar7);
                if (plVar7[2] != 0) {
                  LOAcquire();
                  lVar3 = plVar7[2];
                  plVar7[2] = lVar3 + -1;
                  LORelease();
                  if (lVar3 != 0) goto LAB_043e0760;
                }
                pcVar6 = *(code **)(*plVar7 + 0x20);
                if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) ||
                   (pcVar6 == FUN_013efb40)) {
                  FUN_00005810(plVar7,0x20);
                }
                else {
                  (*pcVar6)(plVar7);
                }
              }
            }
LAB_043e0760:
            lVar8 = lVar8 + 0x10;
            pvVar2 = (const_void__)((long)pvVar2 + -0x10);
          } while (lVar8 != 0);
        }
        lStack_78 = (long)pvVar1 - (long)local_80 >> 4;
        FUN_00005810(local_80,local_70 << 4);
        return IVar4;
      }
LAB_043e07dc:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x43e07e0);
      (*pcVar6)();
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151129
// ==== 0x43e08e4  ArcTabGroupResult::init  (68 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcTabGroupResult::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_20;
  class_t *pcStack_18;
  
  pcStack_18 = &objc::class_t::ArcTabGroupResult;
  local_20 = param_1;
  IVar1 = NSObject::init((ID)&local_20,"init");
  if (IVar1 != 0) {
    *(undefined1 *)(IVar1 + 8) = 0;
    *(undefined8 *)(IVar1 + 0x20) = 0xffffffffffffffff;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151151
// ==== 0x43e0930  AXAnnouncementSpec::setPolite:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void AXAnnouncementSpec::setPolite_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 8) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151163
// ==== 0x43e0938  ArcTabGroupInfo::setGroupId:  (8 bytes)

/* WARNING: Function: _objc_setProperty_nonatomic_copy replaced with injection:
   _objc_setProperty_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTabGroupInfo::setGroupId_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x10) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151177
// ==== 0x43e0940  AXAnnouncementSpec::.cxx_destruct  (48 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void AXAnnouncementSpec::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151191
// ==== 0x43e0970  ArcTabGroupInfo::init  (132 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcTabGroupInfo::init(ID param_1,SEL param_2)

{
  undefined *puVar1;
  ID IVar2;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcTabGroupInfo;
  local_30 = param_1;
  IVar2 = NSObject::init((ID)&local_30,"init");
  if (IVar2 != 0) {
    *(cfstringStruct **)(IVar2 + 0x10) = &cf___;
    *(undefined8 *)(IVar2 + 0x18) = 0xffffffffffffffff;
    *(cfstringStruct **)(IVar2 + 0x20) = &cf___;
    *(undefined1 *)(IVar2 + 8) = 0;
    puVar1 = PTR____NSArray0__struct_0d824938;
    *(undefined8 *)(IVar2 + 0x28) = 0;
    *(undefined **)(IVar2 + 0x30) = puVar1;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151220
// ==== 0x43e09f4  ArcTabGroupInfo::setExtensionGroupId:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcTabGroupInfo::setExtensionGroupId_(ID param_1,SEL param_2,long_long param_3)

{
  *(long_long *)(param_1 + 0x18) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151232
// ==== 0x43e09fc  ArcTabGroupInfo::setTitle:  (8 bytes)

/* WARNING: Function: _objc_setProperty_nonatomic_copy replaced with injection:
   _objc_setProperty_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTabGroupInfo::setTitle_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x20) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151246
// ==== 0x43e0a04  RTCVideoEncoderSettings::setMode:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void RTCVideoEncoderSettings::setMode_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  *(unsigned_long_long *)(param_1 + 0x28) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151258
// ==== 0x43e0a0c  ArcTabGroupInfo::setTabSessionIds:  (8 bytes)

/* WARNING: Function: _objc_setProperty_nonatomic_copy replaced with injection:
   _objc_setProperty_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTabGroupInfo::setTabSessionIds_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x30) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151272
// ==== 0x43e0a14  ArcTabGroupInfo::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcTabGroupInfo::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151401
// ==== 0x43e0c34  ArcBrowser::initWithBrowser:  (828 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowser::initWithBrowser_(ID param_1,SEL param_2,Browser param_3)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  undefined8 uVar7;
  Impl *pIVar8;
  undefined8 *puVar9;
  long lVar10;
  code *pcVar11;
  long *plVar12;
  long *plVar13;
  undefined1 auVar14 [16];
  undefined1 auStack_50 [8];
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pIVar8 = param_3.field0_0x0.field0_0x0;
  pcStack_38 = &objc::class_t::ArcBrowser;
  local_40 = param_1;
  IVar5 = NSObject::init((ID)&local_40,"init",pIVar8,param_3.field0_0x0.field1_0x8);
  if (IVar5 == 0) {
    return 0;
  }
  if (*(char *)(IVar5 + 0x38) != '\0') {
    plVar12 = *(long **)(IVar5 + 0x30);
    if (plVar12 != (long *)0x0) {
      LOAcquire();
      lVar4 = plVar12[1];
      plVar12[1] = lVar4 + -1;
      LORelease();
      if (lVar4 == 0) {
        (**(code **)(*plVar12 + 0x10))(plVar12);
        if (plVar12[2] != 0) {
          LOAcquire();
          lVar4 = plVar12[2];
          plVar12[2] = lVar4 + -1;
          LORelease();
          if (lVar4 != 0) goto LAB_043e0c90;
        }
        pcVar11 = *(code **)(*plVar12 + 0x20);
        if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
          FUN_00005810(plVar12,0x20);
        }
        else {
          (*pcVar11)(plVar12);
        }
      }
    }
LAB_043e0c90:
    *(undefined1 *)(IVar5 + 0x38) = 0;
  }
  *(undefined8 *)(IVar5 + 0x28) = *(undefined8 *)pIVar8;
  lVar4 = *(long *)(pIVar8 + 8);
  *(long *)(IVar5 + 0x30) = lVar4;
  if (lVar4 != 0) {
    *(long *)(lVar4 + 8) = *(long *)(lVar4 + 8) + 1;
  }
  *(undefined1 *)(IVar5 + 0x38) = 1;
  IVar6 = _objc_alloc(&objc::class_t::ArcBrowserSidePanel);
  if ((*(byte *)(IVar5 + 0x38) & 1) == 0) goto LAB_043e0f68;
  FUN_042c8400(auStack_50,*(undefined8 *)(*(long *)(IVar5 + 0x28) + 0x1f0));
  IVar6 = ArcBrowserSidePanel::initWithSidePanel_(IVar6,"initWithSidePanel:",auStack_50);
  plVar12 = local_48;
  *(ID *)(IVar5 + 8) = IVar6;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar4 = local_48[1];
    local_48[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      plVar13 = local_48 + 2;
      (**(code **)(*local_48 + 0x10))(local_48);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar4 = plVar12[2];
        plVar12[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_043e0d20;
      }
      pcVar11 = *(code **)(*plVar12 + 0x20);
      if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
        FUN_00005810(plVar12,0x20);
      }
      else {
        (*pcVar11)(plVar12);
      }
    }
  }
LAB_043e0d20:
  auVar14 = _objc_alloc(&objc::class_t::ArcBrowserCreateParams);
  uVar7 = auVar14._8_8_;
  if ((*(byte *)(IVar5 + 0x38) & 1) != 0) {
    puVar9 = *(undefined8 **)(*(long *)(IVar5 + 0x28) + 0x200);
    if (puVar9 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x43e1018);
      (*pcVar11)();
    }
    lVar4 = puVar9[1];
    if (lVar4 != 0) {
      uVar7 = *puVar9;
      lVar10 = *(long *)(lVar4 + 8);
      while (lVar10 != -1) {
        lVar1 = lVar10 + 1;
        lVar2 = *(long *)(lVar4 + 8);
        bVar3 = lVar2 == lVar10;
        lVar10 = lVar2;
        if (bVar3) {
          *(long *)(lVar4 + 8) = lVar1;
          FUN_043334c8(auStack_50);
          IVar6 = _objc_msgSend(auVar14._0_8_,"initWithBrowserCreateParams:",auStack_50);
          *(ID *)(IVar5 + 0x10) = IVar6;
          if (local_48 != (long *)0x0) {
            LOAcquire();
            lVar4 = local_48[1];
            local_48[1] = lVar4 + -1;
            LORelease();
            if (lVar4 == 0) {
              (**(code **)(*local_48 + 0x10))(local_48);
              if (local_48[2] != 0) {
                LOAcquire();
                lVar4 = local_48[2];
                local_48[2] = lVar4 + -1;
                LORelease();
                if (lVar4 != 0) {
                  return IVar5;
                }
              }
              pcVar11 = *(code **)(*local_48 + 0x20);
              if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) ||
                 (pcVar11 == FUN_013efb40)) {
                FUN_00005810(local_48,0x20);
              }
              else {
                (*pcVar11)(local_48);
              }
            }
          }
          return IVar5;
        }
      }
    }
                    /* WARNING: Subroutine does not return */
    FUN_050d342c(auVar14._0_8_,uVar7);
  }
LAB_043e0f68:
                    /* WARNING: Does not return */
  pcVar11 = (code *)SoftwareBreakpoint(1,0x43e0f6c);
  (*pcVar11)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151636
// ==== 0x43e1190  ArcBrowser::tryToCloseSkipBeforeUnloadHandlers:callback:  (224 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

void ArcBrowser::tryToCloseSkipBeforeUnloadHandlers_callback_
               (ID param_1,SEL param_2,bool param_3,ID param_4,undefined4 param_5)

{
  code *pcVar1;
  ulong local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43e126c);
    (*pcVar1)();
  }
  local_48 = &LAB_043e3394;
  local_40 = &PTR_FUN_0df6b548;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  FUN_042fa5c8(&local_60,*(undefined8 *)(param_1 + 0x28),param_3,local_58);
  if ((local_60 & 1) == 0) {
    FUN_04b35898();
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151679
// ==== 0x43e1284  ArcBrowser::setDelegate:  (536 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  undefined8 uVar2;
  code *pcVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;
  long *plVar7;
  
  if (param_3 == 0) {
    plVar7 = *(long **)(param_1 + 0x20);
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 *)(param_1 + 0x20) = 0;
    if (plVar7 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar4 = plVar7[1];
    plVar7[1] = lVar4 + -1;
    LORelease();
    if (lVar4 != 0) {
      return;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7);
    if (plVar7[2] != 0) {
      LOAcquire();
      lVar4 = plVar7[2];
      plVar7[2] = lVar4 + -1;
      LORelease();
      if (lVar4 != 0) {
        return;
      }
    }
    pcVar3 = *(code **)(*plVar7 + 0x20);
    if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
      (*pcVar3)(plVar7);
      return;
    }
    FUN_00005810(plVar7,0x20);
    return;
  }
  if (*(long *)(param_1 + 0x18) != 0) {
    *(ID *)(*(long *)(param_1 + 0x18) + 8) = param_3;
    return;
  }
  puVar5 = (undefined8 *)FUN_00008790(0x30);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = &PTR_viewWillStartLiveResize_0da87760;
  if (puVar5 + 3 == (undefined8 *)0x0) goto LAB_043e1498;
  puVar5[3] = &PTR_FUN_0da87798;
  _objc_initWeak(puVar5 + 4,param_3);
  _objc_initWeak(puVar5 + 5,param_1);
  plVar7 = *(long **)(param_1 + 0x20);
  *(undefined8 **)(param_1 + 0x18) = puVar5 + 3;
  *(undefined8 **)(param_1 + 0x20) = puVar5;
  if (plVar7 != (long *)0x0) {
    LOAcquire();
    lVar4 = plVar7[1];
    plVar7[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar4 = plVar7[2];
        plVar7[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_043e13c0;
      }
      pcVar3 = *(code **)(*plVar7 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar3)(plVar7);
      }
    }
  }
LAB_043e13c0:
  uVar6 = (ulong)*(byte *)(param_1 + 0x38);
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    uVar2 = *(undefined8 *)(param_1 + 0x18);
    if (*(long *)(param_1 + 0x20) != 0) {
      puVar1 = (ulong *)(*(long *)(param_1 + 0x20) + 0x10);
      uVar6 = *puVar1;
      *puVar1 = uVar6 + 1;
    }
    FUN_04332a34(uVar6,param_1 + 0x28,uVar2);
    return;
  }
LAB_043e1498:
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x43e149c);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151809
// ==== 0x43e14d4  ArcBrowser::setWindow:  (120 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::setWindow_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 uVar2;
  ID local_28;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    _os_retain(param_3);
    uVar2 = *(undefined8 *)(param_1 + 0x28);
    local_28 = param_3;
    _os_retain(param_3);
    FUN_042fa9fc(uVar2,&local_28);
    _os_release(local_28);
    _os_release(param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e154c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151838
// ==== 0x43e154c  ArcBrowser::createWebContents:  (324 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowser::createWebContents_(ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 local_40;
  long *local_38;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43e1690);
    (*pcVar4)();
  }
  local_40 = 0;
  local_38 = (long *)0x0;
  if (param_3 == 0) {
    local_58 = 0;
    uStack_50 = 0;
    local_48 = 0;
  }
  else {
    _objc_msgSend(param_3,"nativeWebContentsConfiguration",&local_58);
  }
  FUN_042faa84(&local_40,*(undefined8 *)(param_1 + 0x28),&local_58);
  IVar3 = _objc_alloc(&objc::class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",&local_40);
  plVar1 = local_38;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_38[1];
    local_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:151908
// ==== 0x43e1734  ArcBrowser::takeOwnershipOfWebContents:  (408 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowser::takeOwnershipOfWebContents_(ID param_1,SEL param_2,ID param_3)

{
  ID IVar1;
  undefined8 uVar2;
  long lVar3;
  code *pcVar4;
  undefined8 local_50;
  long *local_48;
  ulong local_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x38) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43e18c0);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_50 = 0;
    local_48 = (long *)0x0;
    local_40 = 0;
    IVar1 = 0;
  }
  else {
    IVar1 = _objc_msgSend(param_3,"web_contents",&local_50);
    if ((local_40 & 1) != 0) {
      lVar3 = *(long *)(param_1 + 0x28);
      if (((*(long *)(lVar3 + 0xf0) == 0) || (*(char *)(*(long *)(lVar3 + 0xf0) + 4) != '\0')) ||
         (*(long *)(lVar3 + 0xf8) == 0)) {
        local_38 = FUN_04b37104(0x25,"browser deinitialized",0x15,0x1dc,
                                "../../arc/browser/browser_impl.cc");
      }
      else {
        if ((*(long *)(lVar3 + 0xf0) == 0) || (*(char *)(*(long *)(lVar3 + 0xf0) + 4) != '\0')) {
          uVar2 = 0;
        }
        else {
          uVar2 = *(undefined8 *)(lVar3 + 0xf8);
        }
        FUN_0437b014(&local_38,local_50,uVar2);
      }
      if ((local_38 & 1) == 0) {
        FUN_04b35898();
      }
      if (((char)local_40 != '\0') && (local_48 != (long *)0x0)) {
        LOAcquire();
        lVar3 = local_48[1];
        local_48[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          (**(code **)(*local_48 + 0x10))(local_48);
          if (local_48[2] != 0) {
            LOAcquire();
            lVar3 = local_48[2];
            local_48[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) {
              return;
            }
          }
          pcVar4 = *(code **)(*local_48 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(local_48,0x20);
          }
          else {
            (*pcVar4)(local_48);
          }
        }
      }
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00(IVar1);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:153246
// ==== 0x43e320c  ArcBrowser::.cxx_destruct  (376 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowser::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x38) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x30), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_043e3240;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_043e3240:
  plVar3 = *(long **)(param_1 + 0x20);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e3258;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_043e3258:
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154337
// ==== 0x43e6224  ArcBrowserApplicationDebugProperty::initWithDebugProperty:  (264 bytes)

/* Function Stack Size: 0x40 bytes */

ID ArcBrowserApplicationDebugProperty::initWithDebugProperty_
             (ID param_1,SEL param_2,DebugProperty *param_3)

{
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar1;
  byte bVar2;
  DebugProperty *pDVar3;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar4;
  ID IVar5;
  char *pcVar6;
  unsigned_long_long uVar7;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcBrowserApplicationDebugProperty;
  local_30 = param_1;
  IVar5 = NSObject::init((ID)&local_30,"init");
  if (IVar5 != 0) {
    if ((DebugProperty *)(IVar5 + 8) != param_3) {
      bVar2 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
      if (*(char *)(IVar5 + 0x1f) < '\0') {
        uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
        pDVar3 = (DebugProperty *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
        if (-1 < (char)bVar2) {
          uVar7 = (ulong)bVar2;
          pDVar3 = param_3;
        }
        FUN_00359ad8(IVar5 + 8,pDVar3,uVar7);
      }
      else if ((char)bVar2 < '\0') {
        FUN_00359998(IVar5 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                     (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
      }
      else {
        uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
        pcVar6 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
        *(AnonymousBitField1 *)(IVar5 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
        *(unsigned_long_long *)(IVar5 + 0x10) = uVar7;
        (((DebugProperty *)(IVar5 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar6;
      }
    }
    pbVar1 = &param_3->field1_0x18;
    if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(IVar5 + 0x20) != pbVar1
       ) {
      bVar2 = *(byte *)((long)&(param_3->field1_0x18).field0_0x0 + 0x17);
      if (*(char *)(IVar5 + 0x37) < '\0') {
        uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
        pbVar4 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
                 (param_3->field1_0x18).field0_0x0.field1.field0_0x0;
        if (-1 < (char)bVar2) {
          uVar7 = (ulong)bVar2;
          pbVar4 = pbVar1;
        }
        FUN_00359ad8(IVar5 + 0x20,pbVar4,uVar7);
      }
      else if ((char)bVar2 < '\0') {
        FUN_00359998(IVar5 + 0x20,(param_3->field1_0x18).field0_0x0.field1.field0_0x0,
                     (param_3->field1_0x18).field0_0x0.field1.field1_0x8);
      }
      else {
        uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
        pcVar6 = (pbVar1->field0_0x0).field1.field0_0x0;
        *(AnonymousBitField1 *)(IVar5 + 0x30) = (param_3->field1_0x18).field0_0x0.field1.field2_0x10
        ;
        *(unsigned_long_long *)(IVar5 + 0x28) = uVar7;
        *(char **)(IVar5 + 0x20) = pcVar6;
      }
    }
  }
  return IVar5;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154414
// ==== 0x43e632c  ArcNetworkLoggingConfiguration::filePath  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcNetworkLoggingConfiguration::filePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x1f);
  plVar1 = (long *)*(long *)(param_1 + 8);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 8);
  }
  lVar2 = *(long *)(param_1 + 0x10);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154440
// ==== 0x43e6350  ArcExtensionCommand::commandName  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionCommand::commandName(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x37);
  plVar1 = (long *)*(long *)(param_1 + 0x20);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x20);
  }
  lVar2 = *(long *)(param_1 + 0x28);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154466
// ==== 0x43e6374  ArcBrowserApplicationDebugProperty::property  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplicationDebugProperty::property
               (DebugProperty *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (*(char *)(param_1 + 0x1f) < '\0') {
    FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10)
                );
  }
  else {
    pcVar1 = *(char **)(param_1 + 8);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x18);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00060a60(&__return_storage_ptr__->field1_0x18,*(undefined8 *)(param_1 + 0x20),
                 *(undefined8 *)(param_1 + 0x28));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x20);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x28);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x30);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154504
// ==== 0x43e6408  ArcBrowserApplicationDebugProperty::setProperty:  (220 bytes)

/* Function Stack Size: 0x40 bytes */

void ArcBrowserApplicationDebugProperty::setProperty_(ID param_1,SEL param_2,DebugProperty *param_3)

{
  undefined8 *puVar1;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar2;
  byte bVar3;
  DebugProperty *pDVar4;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar5;
  char *pcVar6;
  unsigned_long_long uVar7;
  
  if ((DebugProperty *)(param_1 + 8) != param_3) {
    bVar3 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pDVar4 = (DebugProperty *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar7 = (ulong)bVar3;
        pDVar4 = param_3;
      }
      FUN_00359ad8(param_1 + 8,pDVar4,uVar7);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(param_1 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar7 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pcVar6 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x10) = uVar7;
      (((DebugProperty *)(param_1 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar6;
    }
  }
  puVar1 = (undefined8 *)(param_1 + 0x20);
  pbVar2 = &param_3->field1_0x18;
  if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)puVar1 != pbVar2) {
    bVar3 = *(byte *)((long)&(param_3->field1_0x18).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x37) < '\0') {
      uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
      pbVar5 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field1_0x18).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar7 = (ulong)bVar3;
        pbVar5 = pbVar2;
      }
      FUN_00359ad8(puVar1,pbVar5,uVar7);
      return;
    }
    if ((char)bVar3 < '\0') {
      FUN_00359998(puVar1,(param_3->field1_0x18).field0_0x0.field1.field0_0x0,
                   (param_3->field1_0x18).field0_0x0.field1.field1_0x8);
      return;
    }
    uVar7 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
    pcVar6 = (pbVar2->field0_0x0).field1.field0_0x0;
    *(AnonymousBitField1 *)(param_1 + 0x30) = (param_3->field1_0x18).field0_0x0.field1.field2_0x10;
    *(unsigned_long_long *)(param_1 + 0x28) = uVar7;
    *puVar1 = pcVar6;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154572
// ==== 0x43e64e4  ArcBrowserApplicationDebugProperty::.cxx_destruct  (84 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplicationDebugProperty::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  if (-1 < *(char *)(param_1 + 0x1f)) {
    return;
  }
  FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154590
// ==== 0x43e6538  ArcBrowserApplication::init  (16 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::init(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = init_(param_1,"init:",0);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154604
// ==== 0x43e6548  ArcBrowserApplication::init:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserApplication::init_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  undefined8 *puVar3;
  ID IVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  code *pcVar7;
  long *plVar8;
  undefined1 auVar9 [16];
  undefined1 auStack_60 [8];
  long *local_58;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcBrowserApplication;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  puVar3 = (undefined8 *)FUN_00008790(0x28);
  puVar3[1] = 0;
  puVar3[2] = 0;
  *puVar3 = &PTR_viewWillStartLiveResize_0da87850;
  if (puVar3 + 3 == (undefined8 *)0x0) goto LAB_043e6884;
  auVar9 = FUN_042b17e8();
  *(undefined1 (*) [16])(puVar3 + 3) = auVar9;
  plVar8 = *(long **)(IVar2 + 0x28);
  *(undefined8 **)(IVar2 + 0x20) = puVar3 + 3;
  *(undefined8 **)(IVar2 + 0x28) = puVar3;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar8[1];
    plVar8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar1 = plVar8[2];
        plVar8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e65dc;
      }
      pcVar7 = *(code **)(*plVar8 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar7)(plVar8);
      }
    }
  }
LAB_043e65dc:
  IVar4 = _objc_alloc(&objc::class_t::ArcLocalState);
  FUN_042c8400(auStack_60,*(undefined8 *)(**(long **)(IVar2 + 0x20) + 200));
  IVar4 = ArcLocalState::initWithLocalState_(IVar4,"initWithLocalState:",auStack_60);
  *(ID *)(IVar2 + 8) = IVar4;
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e663c;
      }
      pcVar7 = *(code **)(*local_58 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar7)(local_58);
      }
    }
  }
LAB_043e663c:
  if (param_3 == 0) goto LAB_043e66bc;
  puVar5 = (undefined8 *)FUN_00008790(0x28);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = &PTR_viewWillStartLiveResize_0da87888;
  puVar3 = puVar5 + 3;
  if (puVar3 == (undefined8 *)0x0) {
LAB_043e6884:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x43e6888);
    (*pcVar7)();
  }
  puVar5[3] = &PTR_FUN_0da878c0;
  _objc_initWeak(puVar5 + 4,param_3);
  plVar8 = *(long **)(IVar2 + 0x38);
  *(undefined8 **)(IVar2 + 0x30) = puVar3;
  *(undefined8 **)(IVar2 + 0x38) = puVar5;
  if (plVar8 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar8[1];
    plVar8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar8 + 0x10))(plVar8);
      if (plVar8[2] != 0) {
        LOAcquire();
        lVar1 = plVar8[2];
        plVar8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043e6698;
      }
      pcVar7 = *(code **)(*plVar8 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(plVar8,0x20);
      }
      else {
        (*pcVar7)(plVar8);
      }
    }
LAB_043e6698:
    puVar3 = *(undefined8 **)(IVar2 + 0x30);
    puVar5 = *(undefined8 **)(IVar2 + 0x38);
  }
  uVar6 = *(undefined8 *)(IVar2 + 0x20);
  if (puVar5 != (undefined8 *)0x0) {
    puVar5[2] = puVar5[2] + 1;
  }
  FUN_04332148(uVar6,puVar3,puVar5);
LAB_043e66bc:
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154750
// ==== 0x43e692c  ArcBrowserApplication::setNotificationCenter:  (124 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setNotificationCenter_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  if (param_3 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_0440a6e8(param_3);
  }
  if ((DAT_0e4a2f10 & 1) == 0) {
    iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f10);
    if (iVar1 != 0) {
      DAT_0e4a2f08 = 0;
      thunk_FUN_0c44b760(&DAT_0e4a2f10,uVar2);
    }
  }
  FUN_0432cd94(&DAT_0e4a2f08,uVar2);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154778
// ==== 0x43e69a8  ArcBrowserApplication::setHistogramSamplesDelegate:  (440 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setHistogramSamplesDelegate_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined8 local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_042b7ca8(**(undefined8 **)(param_1 + 0x20),local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_042b7d30(**(undefined8 **)(param_1 + 0x20),local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
  }
  else {
    _objc_initWeak(auStack_60,param_3);
    puVar2 = *(undefined8 **)(param_1 + 0x20);
    _objc_copyWeak(auStack_68,auStack_60);
    uVar1 = FUN_00008790(8);
    _objc_moveWeak(uVar1,auStack_68);
    _objc_destroyWeak(auStack_68);
    local_48 = FUN_043eb534;
    local_40 = &PTR_FUN_0df6b688;
    local_58[0] = uVar1;
    FUN_042b7ca8(*puVar2,local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    puVar2 = *(undefined8 **)(param_1 + 0x20);
    _objc_copyWeak(auStack_70,auStack_60);
    uVar1 = FUN_00008790(8);
    _objc_moveWeak(uVar1,auStack_70);
    _objc_destroyWeak(auStack_70);
    local_48 = FUN_043eb604;
    local_40 = &PTR_FUN_0df6b6a8;
    local_58[0] = uVar1;
    FUN_042b7d30(*puVar2,local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    _objc_destroyWeak(auStack_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154848
// ==== 0x43e6b88  ArcBrowserApplication::setBreadcrumbCallback:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setBreadcrumbCallback_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    uVar1 = *puVar2;
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
  }
  else {
    local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
    *local_58[0] = param_3;
    uVar1 = *puVar2;
    local_48 = FUN_043eb670;
    local_40 = &PTR_FUN_0df6b6c8;
  }
  FUN_042b7e3c(uVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154892
// ==== 0x43e6c84  ArcBrowserApplication::setDumpWithoutCrashingCallback:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setDumpWithoutCrashingCallback_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    uVar1 = *puVar2;
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
  }
  else {
    local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
    *local_58[0] = param_3;
    uVar1 = *puVar2;
    local_48 = FUN_043eb6c8;
    local_40 = &PTR_FUN_0df6b6e8;
  }
  FUN_042b7ea0(uVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154936
// ==== 0x43e6d80  ArcBrowserApplication::setAccessibilityObserver:  (292 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setAccessibilityObserver_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 uVar1;
  undefined8 *puVar2;
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined8 local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_48 = FUN_04048340;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_042b7db8(**(undefined8 **)(param_1 + 0x20),local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
  }
  else {
    _objc_initWeak(auStack_60,param_3);
    puVar2 = *(undefined8 **)(param_1 + 0x20);
    _objc_copyWeak(auStack_68,auStack_60);
    uVar1 = FUN_00008790(8);
    _objc_moveWeak(uVar1,auStack_68);
    _objc_destroyWeak(auStack_68);
    local_48 = FUN_043eb730;
    local_40 = &PTR_FUN_0df6b708;
    local_58[0] = uVar1;
    FUN_042b7db8(*puVar2,local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58[0]);
    }
    _objc_destroyWeak(auStack_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:154987
// ==== 0x43e6ec0  ArcBrowserApplication::run  (32 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcBrowserApplication::run(ID param_1,SEL param_2)

{
  int iVar1;
  
  iVar1 = FUN_042b1ba8(**(undefined8 **)(param_1 + 0x20));
  return (long)iVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155001
// ==== 0x43e6ee0  ArcBrowserApplication::tryToTerminate:  (212 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

unsigned_long_long
ArcBrowserApplication::tryToTerminate_(ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  code *pcVar1;
  uint uVar2;
  undefined8 *puVar3;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar3 = *(undefined8 **)(param_1 + 0x20);
  local_48 = &LAB_04044e30;
  local_40 = &PTR_FUN_0df6b728;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  uVar2 = FUN_042b2430(*puVar3,local_58);
  if (2 < uVar2) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e6fb0);
    (*pcVar1)();
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return (ulong)uVar2;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155043
// ==== 0x43e6fc8  ArcBrowserApplication::initialize:  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

long_long ArcBrowserApplication::initialize_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined1 auStack_f0 [104];
  undefined1 auStack_88 [104];
  
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  FUN_043ec404(auStack_f0,param_3);
  uVar3 = *puVar2;
  FUN_0433260c(auStack_88,auStack_f0);
  iVar1 = FUN_042b0850(uVar3,auStack_88);
  FUN_04332524(auStack_88);
  FUN_04332524(auStack_f0);
  return (long)iVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155069
// ==== 0x43e7048  ArcBrowserApplication::createBrowser:withDelegate:forWindow:  (900 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcBrowserApplication::createBrowser_withDelegate_forWindow_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  bool bVar1;
  long *plVar2;
  long lVar3;
  ID IVar4;
  __shared_weak_count *p_Var5;
  code *pcVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  long *plVar9;
  Browser BVar10;
  undefined8 local_b0;
  long *plStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined8 local_88;
  long *local_80;
  ulong local_78;
  undefined8 local_70;
  long *plStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  
  local_70 = 0;
  plStack_68 = (long *)0x0;
  if (param_3 == 0) {
    local_88 = 0;
    local_80 = (long *)0x0;
    local_78 = 0;
    IVar4 = param_5;
LAB_043e73c8:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00(IVar4);
  }
  puVar7 = *(undefined8 **)(param_1 + 0x20);
  p_Var5 = (__shared_weak_count *)param_4;
  IVar4 = _objc_msgSend(param_3,"browser_context",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043e73c8;
  if (param_5 == 0) {
    local_a0 = 0;
    uStack_98 = 0;
    local_90 = 0;
    uVar8 = *puVar7;
LAB_043e711c:
    bVar1 = false;
    uStack_58 = uStack_98;
    local_60 = local_a0;
    local_50 = local_90;
  }
  else {
    FUN_01b356b8(&local_a0,param_5);
    uVar8 = *puVar7;
    if (-1 < (long)local_90) goto LAB_043e711c;
    FUN_00060a60(&local_60,local_a0,uStack_98);
    bVar1 = true;
  }
  FUN_042b32d0(&local_70,uVar8,&local_88,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if (bVar1) {
    FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
  }
  if (((char)local_78 != '\0') && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_80[1];
    local_80[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar3 = local_80[2];
        local_80[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043e7180;
      }
      pcVar6 = *(code **)(*local_80 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar6)(local_80);
      }
    }
  }
LAB_043e7180:
  IVar4 = _objc_alloc(&objc::class_t::ArcBrowser);
  plStack_a8 = plStack_68;
  local_b0 = local_70;
  if (plStack_68 != (long *)0x0) {
    plStack_68[1] = plStack_68[1] + 1;
  }
  BVar10.field0_0x0.field1_0x8 = p_Var5;
  BVar10.field0_0x0.field0_0x0 = (Impl *)&local_b0;
  IVar4 = ArcBrowser::initWithBrowser_(IVar4,"initWithBrowser:",BVar10);
  plVar2 = plStack_a8;
  if (plStack_a8 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_a8[1];
    plStack_a8[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar9 = plStack_a8 + 2;
      (**(code **)(*plStack_a8 + 0x10))(plStack_a8);
      if (*plVar9 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043e71d4;
      }
      pcVar6 = *(code **)(*plVar2 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar6)(plVar2);
      }
    }
  }
LAB_043e71d4:
  ArcBrowser::setDelegate_(IVar4,"setDelegate:",param_4);
  plVar2 = plStack_68;
  if (plStack_68 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_68[1];
    plStack_68[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar9 = plStack_68 + 2;
      (**(code **)(*plStack_68 + 0x10))(plStack_68);
      if (*plVar9 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) {
          return IVar4;
        }
      }
      pcVar6 = *(code **)(*plVar2 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar6)(plVar2);
      }
    }
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155235
// ==== 0x43e7528  ArcBrowserApplication::createPopupBrowser:withDelegate:  (720 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::createPopupBrowser_withDelegate_
             (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *p_Var4;
  code *pcVar5;
  undefined8 *puVar6;
  long *plVar7;
  Browser BVar8;
  undefined8 local_80;
  long *plStack_78;
  undefined8 local_68;
  long *local_60;
  ulong local_58;
  undefined8 local_50;
  long *plStack_48;
  
  local_50 = 0;
  plStack_48 = (long *)0x0;
  if (param_3 == 0) {
    local_68 = 0;
    local_60 = (long *)0x0;
    local_58 = 0;
    IVar3 = param_4;
LAB_043e77f4:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00(IVar3);
  }
  puVar6 = *(undefined8 **)(param_1 + 0x20);
  p_Var4 = (__shared_weak_count *)param_4;
  IVar3 = _objc_msgSend(param_3,"browser_context",&local_68);
  if ((local_58 & 1) == 0) goto LAB_043e77f4;
  FUN_042b3804(&local_50,*puVar6,&local_68);
  if (((char)local_58 != '\0') && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_60[1];
    local_60[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar2 = local_60[2];
        local_60[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e75b4;
      }
      pcVar5 = *(code **)(*local_60 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar5)(local_60);
      }
    }
  }
LAB_043e75b4:
  IVar3 = _objc_alloc(&objc::class_t::ArcBrowser);
  plStack_78 = plStack_48;
  local_80 = local_50;
  if (plStack_48 != (long *)0x0) {
    plStack_48[1] = plStack_48[1] + 1;
  }
  BVar8.field0_0x0.field1_0x8 = p_Var4;
  BVar8.field0_0x0.field0_0x0 = (Impl *)&local_80;
  IVar3 = ArcBrowser::initWithBrowser_(IVar3,"initWithBrowser:",BVar8);
  plVar1 = plStack_78;
  if (plStack_78 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_78[1];
    plStack_78[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar7 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7608;
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar5)(plVar1);
      }
    }
  }
LAB_043e7608:
  ArcBrowser::setDelegate_(IVar3,"setDelegate:",param_4);
  plVar1 = plStack_48;
  if (plStack_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_48[1];
    plStack_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar7 = plStack_48 + 2;
      (**(code **)(*plStack_48 + 0x10))(plStack_48);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar5 = *(code **)(*plVar1 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar5)(plVar1);
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155369
// ==== 0x43e793c  ArcBrowserApplication::createAIService:  (260 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserApplication::createAIService_(ID param_1,SEL param_2,ID *param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_40 [8];
  long *local_38;
  
  IVar2 = _objc_alloc(&objc::class_t::ArcAIService);
  FUN_042b3ae0(auStack_40,**(undefined8 **)(param_1 + 0x20));
  IVar2 = ArcAIService::initWithAIService_(IVar2,"initWithAIService:",auStack_40);
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155415
// ==== 0x43e7ae4  ArcBrowserApplication::version  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::version(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  undefined5 local_38;
  undefined3 uStack_33;
  undefined5 uStack_30;
  char local_2b [3];
  undefined7 local_28;
  char cStack_21;
  
  cStack_21 = '\r';
  if (local_2b <= "156.0.8078.48" || "156.0.8078.48" < &local_38) {
    local_38 = 0x302e363531;
    uStack_33 = 0x30382e;
    uStack_30 = 0x38342e3837;
    local_2b[0] = '\0';
    IVar2 = FUN_01c391bc(&local_38,0xd);
    if (cStack_21 < '\0') {
      FUN_00005810(CONCAT35(uStack_33,local_38),CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e7b94);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155452
// ==== 0x43e7b94  ArcBrowserApplication::tracingController  (460 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::tracingController(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *in_x3;
  code *pcVar4;
  long *plVar5;
  TracingController TVar6;
  undefined8 local_50;
  long *plStack_48;
  undefined8 local_40;
  long *plStack_38;
  
  if (*(ID *)(param_1 + 0x10) != 0) {
    return *(ID *)(param_1 + 0x10);
  }
  local_40 = 0;
  plStack_38 = (long *)0x0;
  FUN_042b7738(&local_40,**(undefined8 **)(param_1 + 0x20));
  IVar3 = _objc_alloc(&objc::class_t::ArcTracingController);
  plStack_48 = plStack_38;
  local_50 = local_40;
  if (plStack_38 != (long *)0x0) {
    plStack_38[1] = plStack_38[1] + 1;
  }
  TVar6.field0_0x0.field1_0x8 = in_x3;
  TVar6.field0_0x0.field0_0x0 = (Impl *)&local_50;
  IVar3 = ArcTracingController::init_(IVar3,"init:",TVar6);
  plVar1 = plStack_48;
  *(ID *)(param_1 + 0x10) = IVar3;
  if (plStack_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_48[1];
    plStack_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = plStack_48 + 2;
      (**(code **)(*plStack_48 + 0x10))(plStack_48);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7c28;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043e7c28:
  plVar1 = plStack_38;
  if (plStack_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_38[1];
    plStack_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = plStack_38 + 2;
      (**(code **)(*plStack_38 + 0x10))(plStack_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7c40;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043e7c40:
  return *(ID *)(param_1 + 0x10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155546
// ==== 0x43e7ea4  ArcBrowserApplication::networkActivityMonitor  (460 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::networkActivityMonitor(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *in_x3;
  code *pcVar4;
  long *plVar5;
  NetworkActivityMonitor NVar6;
  undefined8 local_50;
  long *plStack_48;
  undefined8 local_40;
  long *plStack_38;
  
  if (*(ID *)(param_1 + 0x18) != 0) {
    return *(ID *)(param_1 + 0x18);
  }
  local_40 = 0;
  plStack_38 = (long *)0x0;
  FUN_042b785c(&local_40,**(undefined8 **)(param_1 + 0x20));
  IVar3 = _objc_alloc(&objc::class_t::ArcNetworkActivityMonitor);
  plStack_48 = plStack_38;
  local_50 = local_40;
  if (plStack_38 != (long *)0x0) {
    plStack_38[1] = plStack_38[1] + 1;
  }
  NVar6.field0_0x0.field1_0x8 = in_x3;
  NVar6.field0_0x0.field0_0x0 = (Impl *)&local_50;
  IVar3 = ArcNetworkActivityMonitor::init_(IVar3,"init:",NVar6);
  plVar1 = plStack_48;
  *(ID *)(param_1 + 0x18) = IVar3;
  if (plStack_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_48[1];
    plStack_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = plStack_48 + 2;
      (**(code **)(*plStack_48 + 0x10))(plStack_48);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7f38;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043e7f38:
  plVar1 = plStack_38;
  if (plStack_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = plStack_38[1];
    plStack_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = plStack_38 + 2;
      (**(code **)(*plStack_38 + 0x10))(plStack_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e7f50;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043e7f50:
  return *(ID *)(param_1 + 0x18);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155640
// ==== 0x43e81b4  ArcBrowserApplication::enableAccessibilityForScreenReader  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::enableAccessibilityForScreenReader(ID param_1,SEL param_2)

{
  FUN_042b85e8(**(undefined8 **)(param_1 + 0x20));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155652
// ==== 0x43e81c0  ArcBrowserApplication::ensureBasicAccessibilityEnabled  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::ensureBasicAccessibilityEnabled(ID param_1,SEL param_2)

{
  FUN_042b86c0(**(undefined8 **)(param_1 + 0x20));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155664
// ==== 0x43e81cc  ArcBrowserApplication::disableAccessibility  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::disableAccessibility(ID param_1,SEL param_2)

{
  FUN_042b8798(**(undefined8 **)(param_1 + 0x20));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155676
// ==== 0x43e81d8  ArcBrowserApplication::isFeatureEnabled:  (28 bytes)

/* Function Stack Size: 0x18 bytes */

bool ArcBrowserApplication::isFeatureEnabled_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  bool bVar1;
  
  if ((int)param_3 != 0x2d) {
    bVar1 = FUN_020cf42c(param_3);
    return bVar1;
  }
  bVar1 = FUN_00014cc0(&PTR_s_ArcMacCoalescedResizing_0d41f79e_1_0e304a40);
  return bVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155694
// ==== 0x43e81f4  ArcBrowserApplication::setFeatureEnabled:enabled:  (124 bytes)

/* Function Stack Size: 0x1c bytes */

void ArcBrowserApplication::setFeatureEnabled_enabled_
               (ID param_1,SEL param_2,unsigned_long_long param_3,bool param_4)

{
  int iVar1;
  
  if (((DAT_0e4acbd0 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4acbd0), iVar1 != 0)) {
    FUN_044d8594(&DAT_0e4acb80,param_3);
    thunk_FUN_0c44b760(&DAT_0e4acbd0);
  }
  FUN_044d9734(&DAT_0e4acb80,param_3,param_4);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155713
// ==== 0x43e8270  ArcBrowserApplication::setStringFeatureValue:value:  (136 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserApplication::setStringFeatureValue_value_
               (ID param_1,SEL param_2,unsigned_long_long param_3,ID param_4)

{
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if (param_4 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_4);
  }
  FUN_042b88c8(param_3,&local_38);
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155743
// ==== 0x43e82f8  ArcBrowserApplication::setIntFeatureValue:value:  (112 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcBrowserApplication::setIntFeatureValue_value_
               (ID param_1,SEL param_2,unsigned_long_long param_3,long_long param_4)

{
  int iVar1;
  
  if (((DAT_0e4acbd0 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4acbd0), iVar1 != 0)) {
    FUN_044d8594(&DAT_0e4acb80,param_3);
    thunk_FUN_0c44b760(&DAT_0e4acbd0);
  }
  FUN_044d9f3c(&DAT_0e4acb80,param_3,param_4);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155762
// ==== 0x43e8368  ArcBrowserApplication::createBrowserContext:isHidden:type:completion:error:  (412 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x34 bytes */

bool ArcBrowserApplication::createBrowserContext_isHidden_type_completion_error_
               (ID param_1,SEL param_2,ID param_3,bool param_4,unsigned_long_long param_5,ID param_6
               ,undefined4 param_7,ID *param_8)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  SEL extraout_x1;
  uint *puVar4;
  undefined8 *puVar5;
  uint *local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  uint *local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar5 = *(undefined8 **)(param_1 + 0x20);
  if ((param_5 != 0) && (param_5 != 1)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e8504);
    (*pcVar1)();
  }
  local_80 = (uint *)0x0;
  if (param_3 == 0) {
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
  }
  else {
    FUN_01b356b8(&local_98,param_3);
    param_2 = extraout_x1;
  }
  local_78[0] = (ID *)FUN_00008790(8,param_2);
  *local_78[0] = param_6;
  local_68 = &LAB_043eb7a0;
  local_60 = &PTR_FUN_0df6b748;
  FUN_042b3d74(&local_80,*puVar5,param_5,param_4,&local_98,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_88 < 0) {
    FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
  }
  local_a0 = local_80;
  puVar4 = local_80;
  if (((ulong)local_80 & 1) == 0) {
    uVar2 = *local_80;
    *local_80 = uVar2 + 1;
    puVar4 = (uint *)(ulong)uVar2;
  }
  bVar3 = FUN_04402548(puVar4,&local_a0,param_7);
  if (((ulong)local_a0 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_80 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_58) {
    return bVar3;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155840
// ==== 0x43e8550  ArcBrowserApplication::getBrowserContext:type:  (408 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::getBrowserContext_type_
             (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  undefined8 *puVar5;
  long *plVar6;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  puVar5 = *(undefined8 **)(param_1 + 0x20);
  if ((param_4 != 0) && (param_4 != 1)) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(0x550b,0x43e86e4);
    (*pcVar4)();
  }
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  FUN_042b4410(&local_48,*puVar5,param_4,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((char)local_38 == '\0') {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&objc::class_t::ArcBrowserContext);
    if ((local_38 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    IVar3 = ArcBrowserContext::initWithBrowserContext_(IVar3,"initWithBrowserContext:",&local_48);
    plVar1 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar2 = local_40[1];
      local_40[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        plVar6 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
        if (*plVar6 != 0) {
          LOAcquire();
          lVar2 = plVar1[2];
          plVar1[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) {
            return IVar3;
          }
        }
        pcVar4 = *(code **)(*plVar1 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(plVar1,0x20);
        }
        else {
          (*pcVar4)(plVar1);
        }
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155927
// ==== 0x43e8794  ArcBrowserApplication::loadBrowserContext:type:completion:  (320 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcBrowserApplication::loadBrowserContext_type_completion_
               (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4,ID param_5,
               undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 *puVar2;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if ((param_4 != 0) && (param_4 != 1)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e88d4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  local_58 = &LAB_043eb7a0;
  local_50 = &PTR_FUN_0df6b768;
  FUN_042b5c28(*puVar2,param_4,&local_80,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:155985
// ==== 0x43e8900  ArcBrowserApplication::availableBrowserContextAttributes  (404 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043e89c8) */
/* WARNING: Removing unreachable block (ram,0x043e89e8) */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::availableBrowserContextAttributes(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  long lVar4;
  const_void__ pvVar5;
  const_void__ local_68;
  long local_60;
  long local_58;
  
  IVar2 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
  FUN_042b61fc(&local_68,**(undefined8 **)(param_1 + 0x20));
  pvVar5 = local_68;
  for (lVar4 = local_60 * 0x38; lVar4 != 0; lVar4 = lVar4 + -0x38) {
    IVar3 = _objc_alloc(&objc::class_t::ArcBrowserContextAttributes);
    IVar3 = ArcBrowserContextAttributes::initWithBrowserContextAttributes_
                      (IVar3,"initWithBrowserContextAttributes:",pvVar5);
    NSMutableArray::addObject_(IVar2,"addObject:",IVar3);
    pvVar5 = (const_void__)((long)pvVar5 + 0x38);
  }
  if (local_68 != (const_void__)0x0) {
    local_60 = local_60 * 0x38;
    if (local_60 != 0) {
      lVar4 = 0;
      pvVar5 = local_68;
      do {
        if ((qword *)((long)pvVar5 + local_60) == &segment_command_00000020.vmaddr) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x43e8aec);
          (*pcVar1)();
        }
        pvVar5 = (const_void__)((long)pvVar5 + -0x38);
        lVar4 = lVar4 + 0x38;
      } while (local_60 != lVar4);
    }
    local_60 = 0;
    FUN_00005810(local_68,local_58 * 0x38);
  }
  IVar2 = NSMutableArray::copy(IVar2,"copy");
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156038
// ==== 0x43e8b58  ArcBrowserApplication::deleteBrowserContext:error:  (224 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool ArcBrowserApplication::deleteBrowserContext_error_
               (ID param_1,SEL param_2,ID param_3,ID *param_4)

{
  uint uVar1;
  bool bVar2;
  uint *puVar3;
  undefined8 *puVar4;
  uint *local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  uint *local_38;
  
  local_38 = (uint *)0x0;
  puVar4 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
  }
  FUN_042b6394(&local_38,*puVar4,&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  local_58 = local_38;
  puVar3 = local_38;
  if (((ulong)local_38 & 1) == 0) {
    uVar1 = *local_38;
    *local_38 = uVar1 + 1;
    puVar3 = (uint *)(ulong)uVar1;
  }
  bVar2 = FUN_04402548(puVar3,&local_58,param_4);
  if (((ulong)local_58 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_38 & 1) == 0) {
    FUN_04b35898();
  }
  return bVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156090
// ==== 0x43e8c5c  ArcBrowserApplication::availableImportSourceProfiles:  (216 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::availableImportSourceProfiles_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 uVar1;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  uVar1 = **(undefined8 **)(param_1 + 0x20);
  local_48 = FUN_043eb86c;
  local_40 = &PTR_FUN_0df6b788;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  FUN_042b8950(uVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156125
// ==== 0x43e8d34  ArcBrowserApplication::availableImportSources:  (196 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::availableImportSources_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 *puVar1;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar1 = *(undefined8 **)(param_1 + 0x20);
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  local_48 = FUN_043eb948;
  local_40 = &PTR_FUN_0df6b7a8;
  FUN_042b8d7c(*puVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156160
// ==== 0x43e8e0c  ArcBrowserApplication::loadImportSourceProfiles:completion:  (204 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcBrowserApplication::loadImportSourceProfiles_completion_
               (ID param_1,SEL param_2,long_long param_3,ID param_4,undefined4 param_5)

{
  undefined8 *puVar1;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar1 = *(undefined8 **)(param_1 + 0x20);
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  local_48 = &LAB_043eba34;
  local_40 = &PTR_FUN_0df6b7c8;
  FUN_042b8f50(*puVar1,param_3,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156195
// ==== 0x43e8eec  ArcBrowserApplication::beginImportSequence  (68 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::beginImportSequence(ID param_1,SEL param_2)

{
  long lVar1;
  undefined8 *puVar2;
  
  lVar1 = FUN_043210c8();
  if (*(long *)(lVar1 + 0x140) == 0) {
    puVar2 = (undefined8 *)FUN_00008790(0x38);
    puVar2[1] = 0;
    *puVar2 = 0;
    puVar2[3] = 0;
    puVar2[2] = 0;
    *(undefined4 *)(puVar2 + 4) = 0;
    puVar2[5] = 0;
    puVar2[6] = 0;
    *(undefined8 **)(lVar1 + 0x140) = puVar2;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156221
// ==== 0x43e8f30  ArcBrowserApplication::endImportSequence  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::endImportSequence(ID param_1,SEL param_2)

{
  FUN_043210c8();
  FUN_04323710();
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156234
// ==== 0x43e8f44  ArcBrowserApplication::debugProperties  (408 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::debugProperties(ID param_1,SEL param_2)

{
  ID IVar1;
  ID IVar2;
  long lVar3;
  char *pcVar4;
  __rep local_b0;
  __rep local_98;
  long local_80;
  long local_78;
  long *local_68;
  
  IVar1 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
  FUN_042b66bc(&local_80,**(undefined8 **)(param_1 + 0x20));
  lVar3 = local_78 * 0x30;
  if (lVar3 != 0) {
    pcVar4 = (char *)(local_80 + 0x17);
    do {
      IVar2 = _objc_alloc(&objc::class_t::ArcBrowserApplicationDebugProperty);
      if (*pcVar4 < '\0') {
        FUN_00060a60(&local_b0,*(undefined8 *)(pcVar4 + -0x17),*(undefined8 *)(pcVar4 + -0xf));
      }
      else {
        local_b0.field1.field1_0x8 = *(undefined8 *)(pcVar4 + -0xf);
        local_b0.field1.field0_0x0 = (char *)*(undefined8 *)(pcVar4 + -0x17);
        local_b0.field1.field2_0x10 = *(AnonymousBitField1 *)(pcVar4 + -7);
      }
      if (pcVar4[0x18] < '\0') {
        FUN_00060a60(&local_98,*(undefined8 *)(pcVar4 + 1),*(undefined8 *)(pcVar4 + 9));
      }
      else {
        local_98.field1.field1_0x8 = *(undefined8 *)(pcVar4 + 9);
        local_98.field1.field0_0x0 = (char *)*(undefined8 *)(pcVar4 + 1);
        local_98.field1.field2_0x10 =
             (AnonymousBitField1)((AnonymousBitField1 *)(pcVar4 + 0x11))->bitField0_63;
      }
      IVar2 = ArcBrowserApplicationDebugProperty::initWithDebugProperty_
                        (IVar2,"initWithDebugProperty:",(DebugProperty *)&local_b0.field0);
      NSMutableArray::addObject_(IVar1,"addObject:",IVar2);
      if ((long)local_98.field1.field2_0x10 < 0) {
        FUN_00005810(local_98.field1.field0_0x0,
                     (ulong)local_98.field1.field2_0x10 & 0x7fffffffffffffff);
      }
      if ((long)local_b0.field1.field2_0x10 < 0) {
        FUN_00005810(local_b0.field1.field0_0x0,
                     (ulong)local_b0.field1.field2_0x10 & 0x7fffffffffffffff);
      }
      pcVar4 = pcVar4 + 0x30;
      lVar3 = lVar3 + -0x30;
    } while (lVar3 != 0);
  }
  local_68 = &local_80;
  FUN_043da8c8(&local_68);
  IVar1 = NSMutableArray::copy(IVar1,"copy");
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156299
// ==== 0x43e9138  ArcBrowserApplication::getGPULogMessages  (384 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043e9208) */
/* WARNING: Removing unreachable block (ram,0x043e9228) */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::getGPULogMessages(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  ID self;
  ID IVar3;
  long lVar4;
  const_void__ pvVar5;
  const_void__ local_68;
  long local_60;
  long local_58;
  
  self = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
  local_68 = (const_void__)0x0;
  local_60 = 0;
  local_58 = 0;
  FUN_0836a724(&local_68,**(undefined8 **)(param_1 + 0x20));
  pvVar5 = local_68;
  for (lVar1 = local_60 * 0x38; lVar1 != 0; lVar1 = lVar1 + -0x38) {
    IVar3 = _objc_alloc(&objc::class_t::ArcGPULogMessage);
    IVar3 = ArcGPULogMessage::initWithGPULogMessage_(IVar3,"initWithGPULogMessage:",pvVar5);
    NSMutableArray::addObject_(self,"addObject:",IVar3);
    pvVar5 = (const_void__)((long)pvVar5 + 0x38);
  }
  if (local_68 != (const_void__)0x0) {
    lVar1 = local_60 * 0x38;
    if (lVar1 != 0) {
      lVar4 = 0;
      pvVar5 = local_68;
      do {
        if ((qword *)((long)pvVar5 + lVar1) == &segment_command_00000020.vmaddr) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x43e9310);
          (*pcVar2)();
        }
        pvVar5 = (const_void__)((long)pvVar5 + -0x38);
        lVar4 = lVar4 + 0x38;
      } while (lVar1 != lVar4);
    }
    local_60 = 0;
    FUN_00005810(local_68,local_58 * 0x38);
  }
  return self;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156354
// ==== 0x43e937c  ArcBrowserApplication::getHistogramsJSON  (112 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::getHistogramsJSON(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  ID IVar2;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  FUN_08385340(&local_38,0);
  pppuVar1 = (undefined8 ***)local_38;
  if (-1 < (long)cStack_21) {
    pppuVar1 = &local_38;
  }
  if (-1 < cStack_21) {
    lStack_30 = (long)cStack_21;
  }
  IVar2 = FUN_01c391bc(pppuVar1,lStack_30);
  if (cStack_21 < '\0') {
    FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156387
// ==== 0x43e9410  ArcBrowserApplication::isEncryptionAvailable  (228 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserApplication::isEncryptionAvailable(ID param_1,SEL param_2)

{
  ulong uVar1;
  long *plVar2;
  size_t sVar3;
  char cVar4;
  void *pvVar5;
  int iVar6;
  uint uVar7;
  long lVar8;
  ulong uVar9;
  long *plVar10;
  
  lVar8 = *(long *)(**(long **)(param_1 + 0x20) + 0x338);
  uVar7 = 0;
  if (lVar8 != 0) {
    uVar9 = (ulong)*(char *)(lVar8 + 0x3f);
    uVar1 = uVar9;
    if ((long)uVar9 < 0) {
      uVar1 = *(ulong *)(lVar8 + 0x30);
    }
    if (uVar1 != 0) {
      plVar10 = *(long **)(lVar8 + 0x18);
      if (plVar10 != (long *)0x0) {
        pvVar5 = *(void **)(lVar8 + 0x28);
        uVar1 = *(ulong *)(lVar8 + 0x30);
        if (-1 < *(char *)(lVar8 + 0x3f)) {
          pvVar5 = (void *)(lVar8 + 0x28);
          uVar1 = uVar9;
        }
        do {
          while( true ) {
            cVar4 = *(char *)((long)plVar10 + 0x37);
            plVar2 = (long *)plVar10[4];
            if (-1 < (long)cVar4) {
              plVar2 = plVar10 + 4;
            }
            uVar9 = plVar10[5];
            if (-1 < cVar4) {
              uVar9 = (long)cVar4;
            }
            sVar3 = uVar9;
            if (uVar1 <= uVar9) {
              sVar3 = uVar1;
            }
            iVar6 = _memcmp(pvVar5,plVar2,sVar3);
            if (iVar6 != 0) break;
            if (uVar1 == uVar9) {
              if (plVar10 != (long *)(lVar8 + 0x18)) {
                uVar7 = (uint)*(byte *)(plVar10 + 0xb);
                goto LAB_043e94c0;
              }
              goto LAB_043e94a4;
            }
            if (uVar1 < uVar9) goto LAB_043e94dc;
LAB_043e949c:
            plVar10 = (long *)plVar10[1];
            if (plVar10 == (long *)0x0) goto LAB_043e94a4;
          }
          if (-1 < iVar6) goto LAB_043e949c;
LAB_043e94dc:
          plVar10 = (long *)*plVar10;
        } while (plVar10 != (long *)0x0);
      }
    }
LAB_043e94a4:
    uVar7 = 0;
  }
LAB_043e94c0:
  return uVar7 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156465
// ==== 0x43e94f4  ArcBrowserApplication::getBase64EncodedEncryptionKey  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::getBase64EncodedEncryptionKey(ID param_1,SEL param_2)

{
  ulong uVar1;
  undefined1 **ppuVar2;
  ID IVar3;
  uint uVar4;
  undefined1 *local_40;
  ulong uStack_38;
  undefined8 local_30;
  ulong local_28;
  
  uStack_38 = 0;
  local_40 = (undefined1 *)0x0;
  local_28 = 0;
  local_30 = 0;
  FUN_04523400(&local_40,*(undefined8 *)(**(long **)(param_1 + 0x20) + 0x338));
  if ((char)local_28 == '\0') {
    IVar3 = 0;
  }
  else {
    uVar4 = (uint)(char)local_30._7_1_;
    uVar1 = uStack_38;
    if (-1 < (int)uVar4) {
      uVar1 = (ulong)local_30._7_1_;
    }
    if (uVar1 == 0) {
      IVar3 = 0;
    }
    else {
      ppuVar2 = (undefined1 **)local_40;
      if (-1 < (int)uVar4) {
        ppuVar2 = &local_40;
      }
      IVar3 = FUN_01c391bc(ppuVar2);
      uVar4 = (uint)local_30._7_1_;
      if ((local_28 & 1) == 0) {
        return IVar3;
      }
    }
    if ((uVar4 >> 7 & 1) != 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156520
// ==== 0x43e95d4  ArcBrowserApplication::getEnterpriseDeviceID  (172 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::getEnterpriseDeviceID(ID param_1,SEL param_2)

{
  ulong uVar1;
  undefined1 **ppuVar2;
  ID IVar3;
  uint uVar4;
  undefined1 *local_40;
  ulong uStack_38;
  undefined8 local_30;
  ulong local_28;
  
  uStack_38 = 0;
  local_40 = (undefined1 *)0x0;
  local_28 = 0;
  local_30 = 0;
  FUN_043322c4(&local_40,*(undefined8 *)(param_1 + 0x20));
  if ((char)local_28 == '\0') {
    IVar3 = 0;
  }
  else {
    uVar4 = (uint)(char)local_30._7_1_;
    uVar1 = uStack_38;
    if (-1 < (int)uVar4) {
      uVar1 = (ulong)local_30._7_1_;
    }
    if (uVar1 == 0) {
      IVar3 = 0;
    }
    else {
      ppuVar2 = (undefined1 **)local_40;
      if (-1 < (int)uVar4) {
        ppuVar2 = &local_40;
      }
      IVar3 = FUN_01c391bc(ppuVar2);
      uVar4 = (uint)local_30._7_1_;
      if ((local_28 & 1) == 0) {
        return IVar3;
      }
    }
    if ((uVar4 >> 7 & 1) != 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156575
// ==== 0x43e96ac  ArcBrowserApplication::setFreeMemoryTargetOnCriticalMemoryPressure:  (40 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setFreeMemoryTargetOnCriticalMemoryPressure_
               (ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if (((-1 < (long)param_3) && (param_3 >> 0x35 == 0)) && (param_3 >> 0x2b == 0)) {
    DAT_0e2ef310 = param_3 << 0x14;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e96d4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156595
// ==== 0x43e96d4  ArcBrowserApplication::getNonDefaultFlags:  (192 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::getNonDefaultFlags_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  undefined8 *puVar1;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar1 = *(undefined8 **)(param_1 + 0x20);
  local_48 = FUN_043ebe34;
  local_40 = &PTR_FUN_0df6b7e8;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  FUN_042bdd14(*puVar1,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156630
// ==== 0x43e97a8  ArcBrowserApplication::setAllFlagsToDefault:  (160 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setAllFlagsToDefault_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  ID *pIVar1;
  undefined4 *puVar2;
  
  pIVar1 = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *pIVar1 = param_3;
  puVar2 = (undefined4 *)FUN_00008790(0x48);
  *puVar2 = 1;
  *(undefined1 **)(puVar2 + 2) = &LAB_042ae510;
  *(code **)(puVar2 + 4) = FUN_041a82e0;
  *(code **)(puVar2 + 6) = FUN_00b77080;
  *(ID **)(puVar2 + 10) = pIVar1;
  *(undefined1 **)(puVar2 + 0xe) = &LAB_043ebf18;
  *(undefined ***)(puVar2 + 0x10) = &PTR_FUN_0df6b808;
  FUN_02e7bb1c(0,puVar2);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156659
// ==== 0x43e985c  ArcBrowserApplication::setOnWebContentsCloseDebugLogCallback:  (312 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setOnWebContentsCloseDebugLogCallback_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  ID *pIVar1;
  ID *local_58;
  undefined8 local_50;
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_48 = FUN_040490c0;
    local_40 = (undefined **)&DAT_0c4b3500;
    FUN_0436ec80(&local_58,param_2,0,param_4);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58);
    }
  }
  else {
    pIVar1 = (ID *)FUN_00008790(8);
    *pIVar1 = param_3;
    local_50 = 0;
    local_48 = (code *)&LAB_043ebf28;
    local_40 = &PTR_FUN_0df6b828;
    local_58 = (ID *)FUN_00008790(8);
    *local_58 = param_3;
    FUN_0436ec80(&local_58);
    if ((code *)local_40[1] != (code *)0x0) {
      (*(code *)local_40[1])(local_58);
    }
    if (pIVar1 != (ID *)0x0) {
      FUN_00005810(pIVar1,8);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156710
// ==== 0x43e99a8  ArcBrowserApplication::loadUrl:withDelegate:  (268 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043e9a7c) */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::loadUrl_withDelegate_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined8 *puVar1;
  ID IVar2;
  undefined8 *puVar3;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  local_48 = 0;
  uStack_40 = 0;
  local_38 = 0;
  puVar3 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    uStack_68 = 0;
    local_70 = 0;
    uStack_58 = 0;
    uStack_60 = 0;
    uStack_88 = 0;
    local_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    uStack_98 = 0;
    uStack_a0 = 0;
    uStack_c8 = 0;
    local_d0 = 0;
    uStack_b8 = 0;
    uStack_c0 = 0;
    uStack_e8 = 0;
    local_f0 = 0;
    uStack_d8 = 0;
    uStack_e0 = 0;
  }
  else {
    _objc_msgSend(param_3,"config",&local_f0);
  }
  puVar1 = (undefined8 *)FUN_00008790(0x10);
  *puVar1 = &PTR_FUN_0da87ed8;
  puVar1[1] = param_4;
  FUN_042bdb68(&local_48,*puVar3,&local_f0,puVar1);
  FUN_0836de40(&local_f0);
  IVar2 = _objc_alloc(&objc::class_t::ArcURLLoadCancellationToken);
  IVar2 = ArcURLLoadCancellationToken::initWithToken_(IVar2,"initWithToken:",&local_48);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156788
// ==== 0x43e9ae8  ArcBrowserApplication::cancelUrlLoad:  (136 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::cancelUrlLoad_(ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  plVar1 = *(long **)(param_1 + 0x20);
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    _objc_msgSend(param_3,"token",&local_38);
  }
  FUN_0434f448(*(undefined8 *)(*plVar1 + 0x448),&local_38);
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156819
// ==== 0x43e9b70  ArcBrowserApplication::setBlockLists:  (516 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setBlockLists_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  long lVar6;
  long local_88;
  long local_80;
  ID IStack_78;
  undefined *local_70;
  undefined8 uStack_68;
  undefined1 *local_60;
  undefined *puStack_58;
  long *local_50;
  long local_48;
  long local_40;
  ID IStack_38;
  
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  IVar4 = _objc_msgSend(param_3,"count");
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_48 = FUN_00008790(IVar4 * 0x18);
    local_40 = 0;
    IStack_38 = IVar4;
  }
  local_70 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_68 = 0xd0800000;
  local_60 = &LAB_043d763c;
  puStack_58 = &DAT_0df6b218;
  local_50 = &local_48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_70);
  IStack_78 = IStack_38;
  local_80 = local_40;
  local_88 = local_48;
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  FUN_04332374(&local_88);
  lVar2 = local_88;
  if (local_88 != 0) {
    if (local_80 * 0x18 != 0) {
      lVar6 = local_80 * -0x18;
      puVar1 = (undefined8 *)(local_80 * 0x18 + local_88);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_043e9e88;
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_80 = (lVar2 - local_88 >> 3) * -0x5555555555555555;
    FUN_00005810(local_88,IStack_78 * 0x18);
  }
  lVar2 = local_48;
  if (local_48 != 0) {
    if (local_40 * 0x18 != 0) {
      lVar6 = local_40 * -0x18;
      puVar1 = (undefined8 *)(local_40 * 0x18 + local_48);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) {
LAB_043e9e88:
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x43e9e8c);
          (*pcVar3)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_40 = (lVar2 - local_48 >> 3) * -0x5555555555555555;
    FUN_00005810(local_48,IStack_38 * 0x18);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:156917
// ==== 0x43e9e9c  ArcBrowserApplication::setExcludedDomains:  (516 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setExcludedDomains_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  long lVar6;
  long local_88;
  long local_80;
  ID IStack_78;
  undefined *local_70;
  undefined8 uStack_68;
  undefined1 *local_60;
  undefined *puStack_58;
  long *local_50;
  long local_48;
  long local_40;
  ID IStack_38;
  
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  IVar4 = _objc_msgSend(param_3,"count");
  if (IVar4 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar4) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_48 = FUN_00008790(IVar4 * 0x18);
    local_40 = 0;
    IStack_38 = IVar4;
  }
  local_70 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_68 = 0xd0800000;
  local_60 = &LAB_043ea1c8;
  puStack_58 = &DAT_0df6b218;
  local_50 = &local_48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_70);
  IStack_78 = IStack_38;
  local_80 = local_40;
  local_88 = local_48;
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  FUN_0433244c(&local_88);
  lVar2 = local_88;
  if (local_88 != 0) {
    if (local_80 * 0x18 != 0) {
      lVar6 = local_80 * -0x18;
      puVar1 = (undefined8 *)(local_80 * 0x18 + local_88);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) goto LAB_043ea1b4;
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_80 = (lVar2 - local_88 >> 3) * -0x5555555555555555;
    FUN_00005810(local_88,IStack_78 * 0x18);
  }
  lVar2 = local_48;
  if (local_48 != 0) {
    if (local_40 * 0x18 != 0) {
      lVar6 = local_40 * -0x18;
      puVar1 = (undefined8 *)(local_40 * 0x18 + local_48);
      do {
        puVar5 = puVar1 + -3;
        if (puVar5 == (undefined8 *)0x0) {
LAB_043ea1b4:
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x43ea1b8);
          (*pcVar3)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar5,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar1 = puVar5;
      } while (lVar6 != 0);
    }
    local_40 = (lVar2 - local_48 >> 3) * -0x5555555555555555;
    FUN_00005810(local_48,IStack_38 * 0x18);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157015
// ==== 0x43ea33c  ArcBrowserApplication::setArtifactAllowedScriptSources:  (584 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserApplication::setArtifactAllowedScriptSources_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  int iVar4;
  ID IVar5;
  undefined8 *puVar6;
  long lVar7;
  long local_88;
  long local_80;
  ID IStack_78;
  undefined *local_70;
  undefined8 uStack_68;
  undefined1 *local_60;
  undefined *puStack_58;
  long *local_50;
  long local_48;
  long local_40;
  ID IStack_38;
  
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  IVar5 = _objc_msgSend(param_3,"count");
  if (IVar5 != 0) {
    if (0xaaaaaaaaaaaaaaa < IVar5) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    local_48 = FUN_00008790(IVar5 * 0x18);
    local_40 = 0;
    IStack_38 = IVar5;
  }
  local_70 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_68 = 0xd0800000;
  local_60 = &LAB_043ea6ac;
  puStack_58 = &DAT_0df6b218;
  local_50 = &local_48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_70);
  if (((DAT_0e4a2fb0 & 1) == 0) && (iVar4 = thunk_FUN_0c44b610(&DAT_0e4a2fb0), iVar4 != 0)) {
    FUN_043d0318(&DAT_0e4a2f78);
    thunk_FUN_0c44b760(&DAT_0e4a2fb0);
  }
  IStack_78 = IStack_38;
  local_80 = local_40;
  local_88 = local_48;
  local_48 = 0;
  local_40 = 0;
  IStack_38 = 0;
  FUN_043d06c0(&DAT_0e4a2f78,&local_88);
  lVar2 = local_88;
  if (local_88 != 0) {
    if (local_80 * 0x18 != 0) {
      lVar7 = local_80 * -0x18;
      puVar1 = (undefined8 *)(local_80 * 0x18 + local_88);
      do {
        puVar6 = puVar1 + -3;
        if (puVar6 == (undefined8 *)0x0) goto LAB_043ea698;
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar6,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar7 = lVar7 + 0x18;
        puVar1 = puVar6;
      } while (lVar7 != 0);
    }
    local_80 = (lVar2 - local_88 >> 3) * -0x5555555555555555;
    FUN_00005810(local_88,IStack_78 * 0x18);
  }
  lVar2 = local_48;
  if (local_48 != 0) {
    if (local_40 * 0x18 != 0) {
      lVar7 = local_40 * -0x18;
      puVar1 = (undefined8 *)(local_40 * 0x18 + local_48);
      do {
        puVar6 = puVar1 + -3;
        if (puVar6 == (undefined8 *)0x0) {
LAB_043ea698:
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x43ea69c);
          (*pcVar3)();
        }
        if (*(char *)((long)puVar1 + -1) < '\0') {
          FUN_00005810(*puVar6,puVar1[-1] & 0x7fffffffffffffff);
        }
        lVar7 = lVar7 + 0x18;
        puVar1 = puVar6;
      } while (lVar7 != 0);
    }
    local_40 = (lVar2 - local_48 >> 3) * -0x5555555555555555;
    FUN_00005810(local_48,IStack_38 * 0x18);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157118
// ==== 0x43ea75c  ArcBrowserApplication::frameForDisplayId:error:  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autorelease replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::frameForDisplayId_error_
             (ID param_1,SEL param_2,long_long param_3,ID *param_4)

{
  ID IVar1;
  ID IVar2;
  undefined8 local_38;
  undefined8 local_30;
  uint local_28;
  
  local_38 = 0;
  local_30 = 0;
  local_28 = 0;
  FUN_042bdc48(&local_38,**(undefined8 **)(param_1 + 0x20),param_3);
  if ((local_28 & 1) == 0) {
    IVar1 = NSError::errorWithDomain_code_userInfo_
                      ((ID)&_OBJC_CLASS___NSError,"errorWithDomain:code:userInfo:",
                       &cf_company_thebrowser_Arc,3,0);
    IVar2 = 0;
    *param_4 = IVar1;
  }
  else {
    IVar2 = NSValue::valueWithRect_
                      ((ID)&_OBJC_CLASS___NSValue,"valueWithRect:",(double)(int)local_38,
                       (double)local_38._4_4_,(double)(int)local_30,(double)local_30._4_4_);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157156
// ==== 0x43ea810  ArcBrowserApplication::restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:  (532 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

void ArcBrowserApplication::restoreBrowser_withSessionID_browserCreatedCallback_finishedCallback_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,uint param_6,ID param_7,
               undefined4 param_8)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 *puVar4;
  undefined4 local_98;
  undefined1 local_94;
  undefined8 local_90;
  long *local_88;
  ulong local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  if (param_3 == 0) {
    local_90 = 0;
    local_88 = (long *)0x0;
    local_80 = 0;
    IVar2 = (ulong)param_6;
  }
  else {
    puVar4 = *(undefined8 **)(param_1 + 0x20);
    IVar2 = _objc_msgSend(param_3,"browser_context",&local_90);
    if ((local_80 & 1) != 0) {
      IVar2 = _objc_msgSend(param_4,"sessionId");
      local_98 = (undefined4)IVar2;
      local_94 = (undefined1)(IVar2 >> 0x20);
      if ((IVar2 >> 0x20 & 1) != 0) {
        local_78[0] = (ID *)FUN_00008790(0x10);
        *local_78[0] = param_5;
        local_78[0][1] = (ulong)param_6;
        local_68 = &LAB_043ec004;
        local_60 = &PTR_FUN_0df6b848;
        FUN_042bdee4(*puVar4,&local_90,&local_98,local_78);
        if ((code *)local_60[1] != (code *)0x0) {
          (*(code *)local_60[1])(local_78[0]);
        }
        if (((char)local_80 != '\0') && (local_88 != (long *)0x0)) {
          LOAcquire();
          lVar1 = local_88[1];
          local_88[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*local_88 + 0x10))(local_88);
            if (local_88[2] != 0) {
              LOAcquire();
              lVar1 = local_88[2];
              local_88[2] = lVar1 + -1;
              LORelease();
              if (lVar1 != 0) goto LAB_043ea93c;
            }
            pcVar3 = *(code **)(*local_88 + 0x20);
            if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40))
            {
              FUN_00005810(local_88,0x20);
            }
            else {
              (*pcVar3)(local_88);
            }
          }
        }
LAB_043ea93c:
        if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
          ___stack_chk_fail(0);
        }
        return;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00(IVar2);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157244
// ==== 0x43eaae8  ArcBrowserApplication::quit  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::quit(ID param_1,SEL param_2)

{
  FUN_042b2370(**(undefined8 **)(param_1 + 0x20));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157256
// ==== 0x43eaaf4  ArcWebContentsMediaSession::bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<(anonymous_namespace)::WebContentsMediaSessionDelegateBridge>
ArcWebContentsMediaSession::bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<(anonymous_namespace)::WebContentsMediaSessionDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<(anonymous_namespace)::WebContentsMediaSessionDelegateBridge> *)
           (param_1 + 0x20);
  if (*(long *)(param_1 + 0x28) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x28) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157277
// ==== 0x43eab14  ArcWebContentsMediaSession::setBridge:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcWebContentsMediaSession::setBridge_
          (ArcWebContentsMediaSession *this,ID param_1,SEL param_2,
          shared_ptr<(anonymous_namespace)::WebContentsMediaSessionDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  WebContentsMediaSessionDelegateBridge *pWVar2;
  long *plVar3;
  
  pWVar2 = param_3.field0_0x0;
  if (pWVar2 != (WebContentsMediaSessionDelegateBridge *)0x0) {
    *(long *)(pWVar2 + 8) = *(long *)(pWVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0x28);
  *(SEL *)(this + 0x20) = param_2;
  *(WebContentsMediaSessionDelegateBridge **)(this + 0x28) = pWVar2;
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_1 = extraout_x1;
        if (lVar1 != 0) goto LAB_043eab54;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_1 = extraout_x1_00;
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar3);
        param_1 = extraout_x1_01;
      }
    }
  }
LAB_043eab54:
  if (pWVar2 != (WebContentsMediaSessionDelegateBridge *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(pWVar2 + 8);
    *(long *)(pWVar2 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)pWVar2 + 0x10))(pWVar2,param_1);
      if (*(long *)(pWVar2 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(pWVar2 + 0x10);
        *(long *)(pWVar2 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(long *)pWVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043eac8c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(pWVar2);
        return;
      }
      FUN_00005810(pWVar2,0x20);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157362
// ==== 0x43eac90  ArcBrowserApplication::browserApplicationDelegate  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<(anonymous_namespace)::BrowserApplicationDelegateBridge>
ArcBrowserApplication::browserApplicationDelegate(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<(anonymous_namespace)::BrowserApplicationDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<(anonymous_namespace)::BrowserApplicationDelegateBridge> *)(param_1 + 0x30);
  if (*(long *)(param_1 + 0x38) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x38) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157382
// ==== 0x43eacb0  ArcBrowserApplication::setBrowserApplicationDelegate:  (380 bytes)

/* Function Stack Size: 0x20 bytes */

void __thiscall
ArcBrowserApplication::setBrowserApplicationDelegate_
          (ArcBrowserApplication *this,ID param_1,SEL param_2,
          shared_ptr<(anonymous_namespace)::BrowserApplicationDelegateBridge> param_3)

{
  long lVar1;
  ID extraout_x1;
  ID extraout_x1_00;
  code *UNRECOVERED_JUMPTABLE;
  ID extraout_x1_01;
  BrowserApplicationDelegateBridge *pBVar2;
  long *plVar3;
  
  pBVar2 = param_3.field0_0x0;
  if (pBVar2 != (BrowserApplicationDelegateBridge *)0x0) {
    *(long *)(pBVar2 + 8) = *(long *)(pBVar2 + 8) + 1;
  }
  plVar3 = *(long **)(this + 0x38);
  *(SEL *)(this + 0x30) = param_2;
  *(BrowserApplicationDelegateBridge **)(this + 0x38) = pBVar2;
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_1 = extraout_x1;
        if (lVar1 != 0) goto LAB_043eacf0;
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar3 + 0x20);
      if (((UNRECOVERED_JUMPTABLE == FUN_00f01bc0) || (UNRECOVERED_JUMPTABLE == FUN_00f01bc0)) ||
         (UNRECOVERED_JUMPTABLE == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_1 = extraout_x1_00;
      }
      else {
        (*UNRECOVERED_JUMPTABLE)(plVar3);
        param_1 = extraout_x1_01;
      }
    }
  }
LAB_043eacf0:
  if (pBVar2 != (BrowserApplicationDelegateBridge *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(pBVar2 + 8);
    *(long *)(pBVar2 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)pBVar2 + 0x10))(pBVar2,param_1);
      if (*(long *)(pBVar2 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(pBVar2 + 0x10);
        *(long *)(pBVar2 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*(long *)pBVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x043eae28. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(pBVar2);
        return;
      }
      FUN_00005810(pBVar2,0x20);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157467
// ==== 0x43eae2c  ICloudKeychainPresentationDelegate::setWindow:  (12 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x18 bytes */

void ICloudKeychainPresentationDelegate::setWindow_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 8) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157480
// ==== 0x43eae38  ArcBrowserApplication::.cxx_destruct  (380 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  plVar3 = *(long **)(param_1 + 0x38);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_043eae64;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_043eae64:
  plVar3 = *(long **)(param_1 + 0x28);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043eae7c;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_043eae7c:
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:157555
// ==== 0x43eafb4  ArcBrowserApplication::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158004
// ==== 0x43ec404  FUN_043ec404  (1360 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_043ec404(undefined8 *param_1,ID param_2)

{
  int iVar1;
  ulong uVar2;
  bool bVar3;
  ulong uVar4;
  code *pcVar5;
  ID IVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  undefined4 uVar9;
  long lVar10;
  ulong uVar11;
  long lVar12;
  undefined8 *puVar13;
  size_t sVar14;
  long lVar15;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined *local_90;
  undefined8 uStack_88;
  undefined1 *local_80;
  undefined *puStack_78;
  undefined8 **local_70;
  undefined8 *local_68;
  ulong local_60;
  ulong local_58;
  
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[0xc] = 0;
  param_1[0xb] = 0;
  param_1[10] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[7] = 0;
  param_1[6] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  param_1[3] = 0;
  param_1[2] = 0;
  *(undefined1 *)(param_1 + 0xc) = 1;
  IVar6 = _objc_msgSend(param_2,"argc");
  *(int *)param_1 = (int)IVar6;
  local_68 = (undefined8 *)0x0;
  local_60 = 0;
  local_58 = 0;
  IVar6 = _objc_msgSend(param_2,"argc");
  uVar2 = local_58;
  uVar4 = local_60;
  iVar1 = (int)IVar6 + 1;
  if (local_58 < (ulong)(long)iVar1) {
    uVar11 = (ulong)iVar1;
    if (iVar1 < 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380();
    }
    puVar7 = (undefined8 *)FUN_00008790(uVar11 * 0x18);
    puVar8 = local_68;
    sVar14 = uVar4 * 0x18;
    if (sVar14 != 0) {
      _memcpy(puVar7,local_68,sVar14);
    }
    local_60 = ((long)sVar14 >> 3) * -0x5555555555555555;
    local_68 = puVar7;
    local_58 = uVar11;
    if (puVar8 != (undefined8 *)0x0) {
      FUN_00005810(puVar8,uVar2 * 0x18);
    }
  }
  IVar6 = _objc_msgSend(param_2,"argv");
  local_90 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_88 = 0xd0800000;
  local_80 = &LAB_043ea6ac;
  puStack_78 = &DAT_0df6b218;
  local_70 = &local_68;
  _objc_msgSend(IVar6,"enumerateObjectsUsingBlock:",&local_90);
  uVar4 = local_60;
  if (local_60 == local_58) {
    uVar2 = local_60 + 1;
    if (0xaaaaaaaaaaaaaaa < uVar2) {
                    /* WARNING: Subroutine does not return */
      FUN_04047380(0);
    }
    uVar11 = local_60 * 2;
    if (uVar11 < uVar2 || uVar11 - uVar2 == 0) {
      uVar11 = uVar2;
    }
    if (0x555555555555554 < local_60) {
      uVar11 = 0xaaaaaaaaaaaaaaa;
    }
    if (uVar11 == 0) {
      puVar8 = (undefined8 *)0x0;
    }
    else {
      if (0xaaaaaaaaaaaaaaa < uVar11) {
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      puVar8 = (undefined8 *)FUN_00008790(uVar11 * 0x18);
    }
    uVar2 = local_60;
    puVar7 = puVar8 + uVar4 * 3;
    if (puVar7 == (undefined8 *)0x0) goto LAB_043ec9f4;
    *puVar7 = 0;
    puVar7[2] = 0;
    puVar7 = puVar7 + local_60 * -3;
    if (local_60 * 0x18 != 0) {
      _memcpy(puVar7,local_68,local_60 * 0x18);
    }
    puVar13 = local_68;
    local_60 = ((long)(uVar2 * 0x18 + 0x18) >> 3) * -0x5555555555555555;
    bVar3 = local_68 != (undefined8 *)0x0;
    local_68 = puVar7;
    if (bVar3) {
      lVar12 = local_58 * 0x18;
      local_58 = uVar11;
      FUN_00005810(puVar13,lVar12);
      uVar11 = local_58;
    }
  }
  else {
    puVar8 = local_68 + local_60 * 3;
    if (puVar8 == (undefined8 *)0x0) goto LAB_043ec9f4;
    *puVar8 = 0;
    puVar8[2] = 0;
    puVar8 = local_68;
    uVar11 = local_58;
  }
  local_58 = uVar11;
  lVar12 = (long)puVar8 + ((uVar4 * 0x18 + 0x18) - (long)local_68);
  local_60 = (lVar12 >> 3) * -0x5555555555555555;
  if (lVar12 == 0) {
LAB_043ec9f4:
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43ec9f8);
    (*pcVar5)();
  }
  lVar12 = param_1[1];
  if (lVar12 != 0) {
    lVar10 = param_1[2] * 0x18;
    if (lVar10 != 0) {
      lVar15 = param_1[2] * -0x18;
      puVar8 = (undefined8 *)(lVar10 + lVar12);
      do {
        puVar7 = puVar8 + -3;
        if (puVar7 == (undefined8 *)0x0) goto LAB_043ec9f4;
        if (*(char *)((long)puVar8 + -1) < '\0') {
          FUN_00005810(*puVar7,puVar8[-1] & 0x7fffffffffffffff);
        }
        lVar15 = lVar15 + 0x18;
        puVar8 = puVar7;
      } while (lVar15 != 0);
    }
    FUN_00005810(lVar12,param_1[3] * 0x18);
  }
  param_1[1] = local_68;
  param_1[2] = local_60;
  param_1[3] = local_58;
  local_60 = 0;
  local_58 = 0;
  local_68 = (undefined8 *)0x0;
  IVar6 = _objc_msgSend(param_2,"profilePath");
  if (IVar6 == 0) {
    local_a8 = 0;
    uStack_a0 = 0;
    local_98 = 0;
  }
  else {
    FUN_01b356b8(&local_a8,IVar6);
  }
  if (*(char *)((long)param_1 + 0x37) < '\0') {
    FUN_00005810(param_1[4],param_1[6] & 0x7fffffffffffffff);
  }
  param_1[5] = uStack_a0;
  param_1[4] = local_a8;
  param_1[6] = local_98;
  IVar6 = _objc_msgSend(param_2,"applicationID");
  if (IVar6 == 0) {
    uVar9 = 0;
  }
  else {
    if (IVar6 != 1) goto LAB_043ec9e4;
    uVar9 = 1;
  }
  *(undefined4 *)(param_1 + 7) = uVar9;
  IVar6 = _objc_msgSend(param_2,"isProduction");
  *(char *)(param_1 + 0xb) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"sessionRestorationPolicy");
  if (IVar6 == 0) {
    uVar9 = 0;
  }
  else {
    if (IVar6 != 1) {
LAB_043ec9e4:
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(0x550b,0x43ec9e8);
      (*pcVar5)();
    }
    uVar9 = 1;
  }
  *(undefined4 *)((long)param_1 + 0x5c) = uVar9;
  IVar6 = _objc_msgSend(param_2,"shouldLoadAllProfilesAtStartup");
  *(char *)(param_1 + 0xc) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"quitRunLoopOnTermination");
  *(char *)((long)param_1 + 0x61) = (char)IVar6;
  IVar6 = _objc_msgSend(param_2,"userDefaultsSuiteName");
  if (IVar6 != 0) {
    IVar6 = _objc_msgSend(param_2,"userDefaultsSuiteName");
    if (IVar6 == 0) {
      local_a8 = 0;
      uStack_a0 = 0;
      local_98 = 0;
    }
    else {
      FUN_01b356b8(&local_a8,IVar6);
    }
    if (*(char *)((long)param_1 + 0x57) < '\0') {
      FUN_00005810(param_1[8],param_1[10] & 0x7fffffffffffffff);
    }
    param_1[9] = uStack_a0;
    param_1[8] = local_a8;
    param_1[10] = local_98;
  }
  puVar8 = local_68;
  if (local_68 != (undefined8 *)0x0) {
    if (local_60 * 0x18 != 0) {
      lVar12 = local_60 * -0x18;
      puVar7 = local_68 + local_60 * 3;
      do {
        puVar13 = puVar7 + -3;
        if (puVar13 == (undefined8 *)0x0) goto LAB_043ec9f4;
        if (*(char *)((long)puVar7 + -1) < '\0') {
          FUN_00005810(*puVar13,puVar7[-1] & 0x7fffffffffffffff);
        }
        lVar12 = lVar12 + 0x18;
        puVar7 = puVar13;
      } while (lVar12 != 0);
    }
    local_60 = ((long)puVar8 - (long)local_68 >> 3) * -0x5555555555555555;
    FUN_00005810(local_68,local_58 * 0x18);
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158259
// ==== 0x43eca0c  ArcBrowserApplicationConfiguration::init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x48 bytes */

ID ArcBrowserApplicationConfiguration::
   init_argv_profilePath_applicationID_isProduction_sessionRestorationPolicy_shouldLoadAllProfilesAtStartup_quitRunLoopOnTermination_userDefaultsSuiteName_
             (ID param_1,SEL param_2,int param_3,ID param_4,ID param_5,unsigned_long_long param_6,
             bool param_7,unsigned_long_long param_8,bool param_9,bool param_10,ID param_11)

{
  ID IVar1;
  undefined4 in_stack_0000000c;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcBrowserApplicationConfiguration;
  local_70 = param_1;
  IVar1 = NSObject::init((ID)&local_70,"init");
  if (IVar1 != 0) {
    *(int *)(IVar1 + 0xc) = param_3;
    *(ID *)(IVar1 + 0x10) = param_4;
    *(ID *)(IVar1 + 0x18) = param_5;
    *(char *)(IVar1 + 8) = (char)param_7;
    *(unsigned_long_long *)(IVar1 + 0x20) = param_6;
    *(unsigned_long_long *)(IVar1 + 0x28) = param_8;
    *(undefined1 *)(IVar1 + 9) = (undefined1)param_9;
    *(undefined1 *)(IVar1 + 10) = param_9._1_1_;
    *(ulong *)(IVar1 + 0x30) = CONCAT44(in_stack_0000000c,param_10);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158295
// ==== 0x43ecb04  MacScreenMonitor::isScreenLocked  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool MacScreenMonitor::isScreenLocked(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 9);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158306
// ==== 0x43ecb0c  ArcCpuInfo::hasBMI2  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasBMI2(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158317
// ==== 0x43ecb14  ArcBrowserApplicationConfiguration::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplicationConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158537
// ==== 0x43ecf8c  objc::Categories::ArcSearchEngine_Extensions_::isEqual:  (552 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcSearchEngine_Extensions_::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  bool bVar4;
  long local_60;
  long *local_58;
  byte local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  if (param_3 == 0) {
    return 0;
  }
  IVar2 = _objc_msgSend((ID)&class_t::ArcSearchEngine,"class");
  IVar2 = _objc_msgSend(param_3,"isKindOfClass:",IVar2);
  if ((int)IVar2 == 0) {
    return 0;
  }
  _objc_msgSend(param_1,"search_engine_template_url",&local_48);
  _objc_msgSend(param_3,"search_engine_template_url",&local_60);
  if ((local_50 & local_38) == 0) {
    bVar4 = (bool)(local_38 == local_50);
    if (local_50 == 0) goto LAB_043ed088;
  }
  else {
    if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x43ed1b4);
      (*pcVar3)();
    }
    bVar4 = (bool)(local_48 == local_60);
  }
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043ed088;
      }
      pcVar3 = *(code **)(*local_58 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar3)(local_58);
      }
    }
  }
LAB_043ed088:
  if (local_38 == 0) {
    return bVar4;
  }
  if (local_40 == (long *)0x0) {
    return bVar4;
  }
  LOAcquire();
  lVar1 = local_40[1];
  local_40[1] = lVar1 + -1;
  LORelease();
  if (lVar1 != 0) {
    return bVar4;
  }
  (**(code **)(*local_40 + 0x10))(local_40);
  if (local_40[2] != 0) {
    LOAcquire();
    lVar1 = local_40[2];
    local_40[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return bVar4;
    }
  }
  pcVar3 = *(code **)(*local_40 + 0x20);
  if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
    (*pcVar3)(local_40);
    return bVar4;
  }
  FUN_00005810(local_40,0x20);
  return bVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158639
// ==== 0x43ed260  objc::Categories::ArcSearchEngine_Extensions_::hash  (256 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long objc::Categories::ArcSearchEngine_Extensions_::hash(ID param_1,SEL param_2)

{
  long lVar1;
  unsigned_long_long uVar2;
  code *pcVar3;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  undefined1 local_31;
  
  _objc_msgSend(param_1,"search_engine_template_url",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  local_31 = 0;
  uVar2 = FUN_04340180(&local_31,local_50);
  if ((local_40 != 0) && (local_48 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return uVar2;
        }
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
  return uVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158774
// ==== 0x43ed470  ArcCommandBarRankingModel::directory  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingModel::directory(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x43ed4a0);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158844
// ==== 0x43ed510  ArcCommandBarRankingModel::.cxx_destruct  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingModel::_cxx_destruct(ID param_1,SEL param_2)

{
  if ((*(char *)(param_1 + 0x28) != '\0') && (*(char *)(param_1 + 0x1f) < '\0')) {
    FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158859
// ==== 0x43ed538  ArcAIInferenceModel::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIInferenceModel::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x28) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:158898
// ==== 0x43ed5d8  objc::Categories::ArcBrowserContext_Extensions_::isEqual:  (552 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  bool bVar4;
  long local_60;
  long *local_58;
  byte local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  if (param_3 == 0) {
    return 0;
  }
  IVar2 = _objc_msgSend((ID)&class_t::ArcBrowserContext,"class");
  IVar2 = _objc_msgSend(param_3,"isKindOfClass:",IVar2);
  if ((int)IVar2 == 0) {
    return 0;
  }
  _objc_msgSend(param_1,"browser_context",&local_48);
  _objc_msgSend(param_3,"browser_context",&local_60);
  if ((local_50 & local_38) == 0) {
    bVar4 = (bool)(local_38 == local_50);
    if (local_50 == 0) goto LAB_043ed6d4;
  }
  else {
    if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x43ed800);
      (*pcVar3)();
    }
    bVar4 = (bool)(local_48 == local_60);
  }
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043ed6d4;
      }
      pcVar3 = *(code **)(*local_58 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar3)(local_58);
      }
    }
  }
LAB_043ed6d4:
  if (local_38 == 0) {
    return bVar4;
  }
  if (local_40 == (long *)0x0) {
    return bVar4;
  }
  LOAcquire();
  lVar1 = local_40[1];
  local_40[1] = lVar1 + -1;
  LORelease();
  if (lVar1 != 0) {
    return bVar4;
  }
  (**(code **)(*local_40 + 0x10))(local_40);
  if (local_40[2] != 0) {
    LOAcquire();
    lVar1 = local_40[2];
    local_40[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return bVar4;
    }
  }
  pcVar3 = *(code **)(*local_40 + 0x20);
  if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
    (*pcVar3)(local_40);
    return bVar4;
  }
  FUN_00005810(local_40,0x20);
  return bVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159000
// ==== 0x43ed8ac  objc::Categories::ArcBrowserContext_Extensions_::hash  (252 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long objc::Categories::ArcBrowserContext_Extensions_::hash(ID param_1,SEL param_2)

{
  long lVar1;
  unsigned_long_long uVar2;
  code *pcVar3;
  undefined8 local_50;
  long *local_48;
  byte local_40;
  undefined8 local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  local_38 = local_50;
  uVar2 = FUN_00018f3c(&local_38,8);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return uVar2;
        }
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
  return uVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159051
// ==== 0x43ed9a8  objc::Categories::ArcBrowserContext_Extensions_::setDelegate:  (1116 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::setDelegate_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  long *plVar3;
  long *extraout_x1;
  long *extraout_x1_00;
  long *extraout_x1_01;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_58 [8];
  long *local_50;
  byte local_48;
  
  IVar2 = _objc_msgSend(param_1,"contextDelegate");
  if (param_3 == 0) {
    if (extraout_x1 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar1 = extraout_x1[1];
    extraout_x1[1] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
    (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
    if (extraout_x1[2] != 0) {
      LOAcquire();
      lVar1 = extraout_x1[2];
      extraout_x1[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
    }
    pcVar4 = *(code **)(*extraout_x1 + 0x20);
    local_50 = extraout_x1;
    if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40))
    goto LAB_043edda4;
    goto LAB_043eda70;
  }
  if (extraout_x1 != (long *)0x0) {
    LOAcquire();
    lVar1 = extraout_x1[1];
    extraout_x1[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*extraout_x1 + 0x10))(extraout_x1);
      if (extraout_x1[2] != 0) {
        LOAcquire();
        lVar1 = extraout_x1[2];
        extraout_x1[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043eda04;
      }
      pcVar4 = *(code **)(*extraout_x1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(extraout_x1,0x20);
      }
      else {
        (*pcVar4)(extraout_x1);
      }
    }
  }
LAB_043eda04:
  if (IVar2 == 0) {
    plVar3 = (long *)FUN_00008790(0x30);
    plVar5 = plVar3 + 1;
    *plVar5 = 0;
    plVar3[2] = 0;
    *plVar3 = (long)&PTR_viewWillStartLiveResize_0da879b0;
    if (plVar3 + 3 == (long *)0x0) goto LAB_043ede00;
    plVar3[3] = (long)&PTR_FUN_0da879e8;
    _objc_initWeak(plVar3 + 4,param_3);
    _objc_initWeak(plVar3 + 5,param_1);
    if (param_1 == 0) {
      if (plVar3 != (long *)0x0) {
        LOAcquire();
        lVar1 = *plVar5;
        *plVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*plVar3 + 0x10))(plVar3);
          if (plVar3[2] != 0) {
            LOAcquire();
            lVar1 = plVar3[2];
            plVar3[2] = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_043edc34;
          }
          pcVar4 = *(code **)(*plVar3 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(plVar3,0x20);
          }
          else {
            (*pcVar4)(plVar3);
          }
        }
      }
    }
    else {
      _objc_msgSend(param_1,"setContextDelegate:",plVar3 + 3,plVar3);
    }
LAB_043edc34:
    _objc_msgSend(param_1,"browser_context",auStack_58);
    if ((local_48 & 1) == 0) {
LAB_043ede00:
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x43ede04);
      (*pcVar4)();
    }
    IVar2 = _objc_msgSend(param_1,"contextDelegate");
    if (extraout_x1_01 == (long *)0x0) {
      FUN_04333868(auStack_58,IVar2,0);
    }
    else {
      lVar1 = extraout_x1_01[2];
      extraout_x1_01[2] = lVar1 + 1;
      FUN_04333868(lVar1,auStack_58,IVar2,extraout_x1_01);
      LOAcquire();
      lVar1 = extraout_x1_01[1];
      extraout_x1_01[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*extraout_x1_01 + 0x10))(extraout_x1_01);
        if (extraout_x1_01[2] != 0) {
          LOAcquire();
          lVar1 = extraout_x1_01[2];
          extraout_x1_01[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) goto LAB_043edba4;
        }
        pcVar4 = *(code **)(*extraout_x1_01 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(extraout_x1_01,0x20);
        }
        else {
          (*pcVar4)(extraout_x1_01);
        }
      }
    }
LAB_043edba4:
    if (local_48 == 0) {
      return;
    }
  }
  else {
    IVar2 = _objc_msgSend(param_1,"contextDelegate");
    *(ID *)(IVar2 + 8) = param_3;
    local_50 = extraout_x1_00;
  }
  if (local_50 == (long *)0x0) {
    return;
  }
  LOAcquire();
  lVar1 = local_50[1];
  local_50[1] = lVar1 + -1;
  LORelease();
  if (lVar1 != 0) {
    return;
  }
  (**(code **)(*local_50 + 0x10))(local_50);
  if (local_50[2] != 0) {
    LOAcquire();
    lVar1 = local_50[2];
    local_50[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
  }
  pcVar4 = *(code **)(*local_50 + 0x20);
  if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
LAB_043edda4:
    (*pcVar4)(local_50);
    return;
  }
LAB_043eda70:
  FUN_00005810(local_50,0x20);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159244
// ==== 0x43edeb0  objc::Categories::ArcBrowserContext_Extensions_::loadUrl:withDelegate:  (468 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043edfc0) */
/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::loadUrl_withDelegate_
             (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  undefined8 *puVar2;
  ID IVar3;
  code *pcVar4;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  local_58 = 0;
  uStack_50 = 0;
  local_48 = 0;
  _objc_msgSend(param_1,"browser_context",&local_70);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43ee084);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    uStack_88 = 0;
    local_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    uStack_98 = 0;
    uStack_a0 = 0;
    uStack_c8 = 0;
    local_d0 = 0;
    uStack_b8 = 0;
    uStack_c0 = 0;
    uStack_e8 = 0;
    local_f0 = 0;
    uStack_d8 = 0;
    uStack_e0 = 0;
    uStack_108 = 0;
    local_110 = 0;
    uStack_f8 = 0;
    uStack_100 = 0;
  }
  else {
    _objc_msgSend(param_3,"config",&local_110);
  }
  puVar2 = (undefined8 *)FUN_00008790(0x10);
  *puVar2 = &PTR_FUN_0da87ed8;
  puVar2[1] = param_4;
  FUN_042c8c30(&local_58,local_70,&local_110,puVar2);
  FUN_0836de40(&local_110);
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043edf98;
      }
      pcVar4 = *(code **)(*local_68 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar4)(local_68);
      }
    }
  }
LAB_043edf98:
  IVar3 = _objc_alloc(&class_t::ArcURLLoadCancellationToken);
  IVar3 = ArcURLLoadCancellationToken::initWithToken_(IVar3,"initWithToken:",&local_58);
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159356
// ==== 0x43ee15c  objc::Categories::ArcBrowserContext_Extensions_::cancelUrlLoad:  (388 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::cancelUrlLoad_
               (ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  code *pcVar2;
  long lVar3;
  void *local_60;
  long local_58;
  ulong local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43ee2e0);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_60 = (void *)0x0;
    local_58 = 0;
    local_50 = 0;
  }
  else {
    _objc_msgSend(param_3,"token",&local_60);
  }
  lVar3 = *(long *)(local_48 + 0x288);
  if ((lVar3 != 0) &&
     (((-1 < (long)local_50 || (local_58 != 0x24)) ||
      (iVar1 = _memcmp(local_60,&DAT_0c9d6718,0x24), iVar1 != 0)))) {
    FUN_0434f448(lVar3,&local_60);
  }
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_40[1];
    local_40[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar3 = local_40[2];
        local_40[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159428
// ==== 0x43ee38c  objc::Categories::ArcBrowserContext_Extensions_::getChangedPermissionsForOrigin:  (732 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::getChangedPermissionsForOrigin_
             (ID param_1,SEL param_2,ID param_3)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  code *pcVar7;
  long *plVar8;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  const_void__ local_78;
  long local_70;
  long local_68;
  
  local_78 = (const_void__)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
LAB_043ee638:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x43ee63c);
    (*pcVar7)();
  }
  FUN_043abdf8(local_a8,param_3);
  FUN_042c8da8(&local_78,local_90,local_a8);
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_88[1];
    local_88[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar3 = local_88[2];
        local_88[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043ee434;
      }
      pcVar7 = *(code **)(*local_88 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar7)(local_88);
      }
    }
  }
LAB_043ee434:
  IVar5 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
  pvVar1 = local_78;
  for (lVar3 = local_70 << 4; lVar3 != 0; lVar3 = lVar3 + -0x10) {
    IVar6 = _objc_alloc(&class_t::ArcSiteSettingInfo);
    IVar6 = ArcSiteSettingInfo::initWithSiteSettingInfo_(IVar6,"initWithSiteSettingInfo:",pvVar1);
    NSMutableArray::addObject_(IVar5,"addObject:",IVar6);
    pvVar1 = (const_void__)((long)pvVar1 + 0x10);
  }
  IVar5 = NSMutableArray::copy(IVar5,"copy");
  pvVar1 = local_78;
  if (local_78 != (const_void__)0x0) {
    if (local_70 * 0x10 != 0) {
      lVar3 = local_70 * -0x10;
      pvVar2 = (const_void__)(local_70 * 0x10 + (long)local_78);
      do {
        if ((const_void__)((long)pvVar2 + -0x10) == (const_void__)0x0) goto LAB_043ee638;
        plVar8 = *(long **)((long)pvVar2 + -8);
        if (plVar8 != (long *)0x0) {
          LOAcquire();
          lVar4 = plVar8[1];
          plVar8[1] = lVar4 + -1;
          LORelease();
          if (lVar4 == 0) {
            (**(code **)(*plVar8 + 0x10))(plVar8);
            if (plVar8[2] != 0) {
              LOAcquire();
              lVar4 = plVar8[2];
              plVar8[2] = lVar4 + -1;
              LORelease();
              if (lVar4 != 0) goto LAB_043ee55c;
            }
            pcVar7 = *(code **)(*plVar8 + 0x20);
            if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40))
            {
              FUN_00005810(plVar8,0x20);
            }
            else {
              (*pcVar7)(plVar8);
            }
          }
        }
LAB_043ee55c:
        lVar3 = lVar3 + 0x10;
        pvVar2 = (const_void__)((long)pvVar2 + -0x10);
      } while (lVar3 != 0);
    }
    local_70 = (long)pvVar1 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return IVar5;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159549
// ==== 0x43ee808  objc::Categories::ArcBrowserContext_Extensions_::getContentSettingForOrigin:withContentSettingType:  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::
   getContentSettingForOrigin_withContentSettingType_
             (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4)

{
  ulong *puVar1;
  long *plVar2;
  long lVar3;
  ID IVar4;
  ulong *puVar5;
  code *pcVar6;
  ulong uVar7;
  ulong uVar8;
  long *plVar9;
  undefined8 local_78 [2];
  undefined7 local_68;
  char cStack_61;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context",&local_60);
  if ((local_50 & 1) == 0) goto LAB_043eea74;
  FUN_043abdf8(local_78,param_3);
  uVar7 = 0x25;
  puVar5 = &DAT_0c9d7d08;
  do {
    uVar8 = uVar7 >> 1;
    puVar1 = puVar5 + uVar8 * 2 + 2;
    uVar7 = uVar7 + ~(uVar7 >> 1);
    if (param_4 <= puVar5[uVar8 * 2]) {
      puVar1 = puVar5;
      uVar7 = uVar8;
    }
    puVar5 = puVar1;
  } while (uVar7 != 0);
  if ((puVar1 == &DAT_0c9d7f58) || (param_4 < *puVar1)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x43eeb40);
    (*pcVar6)();
  }
  FUN_042c9600(&local_48,local_60,local_78,(int)puVar1[1]);
  if (cStack_61 < '\0') {
    FUN_00005810(local_78[0],CONCAT17(cStack_61,local_68) & 0x7fffffffffffffff);
  }
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_58[1];
    local_58[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar3 = local_58[2];
        local_58[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043ee8f8;
      }
      pcVar6 = *(code **)(*local_58 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar6)(local_58);
      }
    }
  }
LAB_043ee8f8:
  if ((local_38 & 1) == 0) {
    IVar4 = 0;
  }
  else {
    IVar4 = _objc_alloc(&class_t::ArcSiteSettingInfo);
    if ((local_38 & 1) == 0) {
LAB_043eea74:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x43eea78);
      (*pcVar6)();
    }
    IVar4 = ArcSiteSettingInfo::initWithSiteSettingInfo_(IVar4,"initWithSiteSettingInfo:",&local_48)
    ;
    plVar2 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar3 = local_40[1];
      local_40[1] = lVar3 + -1;
      LORelease();
      if (lVar3 == 0) {
        plVar9 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
        if (*plVar9 != 0) {
          LOAcquire();
          lVar3 = plVar2[2];
          plVar2[2] = lVar3 + -1;
          LORelease();
          if (lVar3 != 0) {
            return IVar4;
          }
        }
        pcVar6 = *(code **)(*plVar2 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar2,0x20);
        }
        else {
          (*pcVar6)(plVar2);
        }
      }
    }
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159676
// ==== 0x43eeb48  objc::Categories::ArcBrowserContext_Extensions_::setPermissionsForOrigin:withPermissionTypes:withSiteSetting:  (748 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setPermissionsForOrigin_withPermissionTypes_withSiteSetting_
               (ID param_1,SEL param_2,ID param_3,ID param_4,unsigned_long_long param_5)

{
  ulong uVar1;
  bool bVar2;
  undefined4 *puVar3;
  ulong uVar4;
  code *pcVar5;
  ID IVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  long lVar9;
  size_t sVar10;
  ulong uVar11;
  undefined4 *puVar12;
  long lVar13;
  undefined4 *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined8 local_90 [2];
  undefined7 local_80;
  char cStack_79;
  undefined1 auStack_78 [8];
  long *local_70;
  byte local_68;
  
  _objc_msgSend(param_1,"browser_context",auStack_78);
  if ((local_68 & 1) != 0) {
    FUN_043abdf8(local_90,param_3);
    local_a8 = (undefined4 *)0x0;
    local_a0 = 0;
    local_98 = 0;
    puVar12 = &DAT_0c9d7f60;
    lVar13 = 0x1d0;
    do {
      IVar6 = _objc_msgSend(param_4,"containsIndex:",*(undefined8 *)(puVar12 + -2));
      uVar4 = local_a0;
      if ((int)IVar6 != 0) {
        if (local_a0 == local_98) {
          uVar1 = local_a0 + 1;
          if (uVar1 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_040416c0();
          }
          uVar11 = local_a0 * 2;
          if (uVar11 < uVar1 || uVar11 - uVar1 == 0) {
            uVar11 = uVar1;
          }
          if (0x1ffffffffffffffe < local_a0) {
            uVar11 = 0x3fffffffffffffff;
          }
          if (uVar11 == 0) {
            puVar7 = (undefined4 *)0x0;
          }
          else {
            if (uVar11 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            puVar7 = (undefined4 *)FUN_00008790(uVar11 << 2);
          }
          puVar8 = puVar7 + uVar4;
          if (puVar8 == (undefined4 *)0x0) goto LAB_043eec3c;
          *puVar8 = *puVar12;
          puVar8 = puVar8 + -local_a0;
          sVar10 = local_a0 * 4;
          _memcpy(puVar8,local_a8,sVar10);
          puVar3 = local_a8;
          local_a0 = (long)(sVar10 + 4) >> 2;
          bVar2 = local_a8 != (undefined4 *)0x0;
          local_a8 = puVar8;
          if (bVar2) {
            lVar9 = local_98 << 2;
            local_98 = uVar11;
            FUN_00005810(puVar3,lVar9);
            uVar11 = local_98;
          }
        }
        else {
          if (local_a8 + local_a0 == (undefined4 *)0x0) goto LAB_043eec3c;
          local_a8[local_a0] = *puVar12;
          puVar7 = local_a8;
          uVar11 = local_98;
        }
        local_98 = uVar11;
        lVar9 = (long)puVar7 + ((uVar4 * 4 + 4) - (long)local_a8);
        local_a0 = lVar9 >> 2;
        if (lVar9 == 0) goto LAB_043eec3c;
      }
      puVar12 = puVar12 + 4;
      lVar13 = lVar13 + -0x10;
    } while (lVar13 != 0);
    if (4 < param_5) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(0x550b,0x43eef60);
      (*pcVar5)();
    }
    FUN_04333920(auStack_78,local_90,&local_a8,param_5);
    if (local_a8 != (undefined4 *)0x0) {
      if ((local_a0 & 0x3fffffffffffffff) != 0) {
        lVar13 = local_a0 * -4;
        puVar12 = local_a8 + local_a0;
        do {
          puVar12 = puVar12 + -1;
          if (puVar12 == (undefined4 *)0x0) goto LAB_043eec3c;
          lVar13 = lVar13 + 4;
        } while (lVar13 != 0);
      }
      local_a0 = 0;
      FUN_00005810(local_a8,local_98 << 2);
    }
    if (cStack_79 < '\0') {
      FUN_00005810(local_90[0],CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
    }
    if ((local_68 != 0) && (local_70 != (long *)0x0)) {
      LOAcquire();
      lVar13 = local_70[1];
      local_70[1] = lVar13 + -1;
      LORelease();
      if (lVar13 == 0) {
        (**(code **)(*local_70 + 0x10))(local_70);
        if (local_70[2] != 0) {
          LOAcquire();
          lVar13 = local_70[2];
          local_70[2] = lVar13 + -1;
          LORelease();
          if (lVar13 != 0) {
            return;
          }
        }
        pcVar5 = *(code **)(*local_70 + 0x20);
        if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
          FUN_00005810(local_70,0x20);
        }
        else {
          (*pcVar5)(local_70);
        }
      }
    }
    return;
  }
LAB_043eec3c:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x43eec40);
  (*pcVar5)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159832
// ==== 0x43eef70  objc::Categories::ArcBrowserContext_Extensions_::setContentSettingsForOrigin:withContentSettingTypes:withSiteSetting:  (596 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setContentSettingsForOrigin_withContentSettingTypes_withSiteSetting_
               (ID param_1,SEL param_2,ID param_3,ID param_4,unsigned_long_long param_5)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  long lVar5;
  long local_d0;
  long local_c8;
  long local_c0;
  undefined8 local_b8 [2];
  undefined7 local_a8;
  char cStack_a1;
  undefined1 auStack_a0 [8];
  long *local_98;
  byte local_90;
  long *local_88;
  undefined4 *puStack_80;
  undefined1 *local_78;
  undefined4 *local_70;
  undefined1 *puStack_68;
  long *local_60;
  long local_58;
  
  _objc_msgSend(param_1,"browser_context",auStack_a0);
  if ((local_90 & 1) != 0) {
    FUN_043abdf8(local_b8,param_3);
    local_d0 = 0;
    local_c8 = 0;
    local_c0 = 0;
    puVar4 = &DAT_0c9d7d10;
    lVar5 = 0x250;
    do {
      IVar2 = _objc_msgSend(param_4,"containsIndex:",*(undefined8 *)(puVar4 + -2));
      if ((int)IVar2 != 0) {
        local_58 = local_d0 + local_c8 * 4;
        local_88 = &local_58;
        puStack_80 = puVar4;
        local_78 = (undefined1 *)&local_d0;
        local_70 = puVar4;
        puStack_68 = (undefined1 *)&local_d0;
        local_60 = &local_58;
        FUN_042da3c8(local_c8 != local_c0,&local_70,&local_88);
        local_c8 = local_58 - local_d0 >> 2;
        if (local_58 - local_d0 == 0) goto LAB_043ef070;
      }
      puVar4 = puVar4 + 4;
      lVar5 = lVar5 + -0x10;
    } while (lVar5 != 0);
    if (4 < param_5) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43ef2fc);
      (*pcVar1)();
    }
    FUN_043339fc(auStack_a0,local_b8,&local_d0,param_5);
    if (local_d0 != 0) {
      if (local_c8 * 4 != 0) {
        lVar5 = local_c8 * -4;
        lVar3 = local_c8 * 4 + local_d0;
        do {
          lVar3 = lVar3 + -4;
          if (lVar3 == 0) goto LAB_043ef070;
          lVar5 = lVar5 + 4;
        } while (lVar5 != 0);
      }
      local_c8 = 0;
      FUN_00005810(local_d0,local_c0 << 2);
    }
    if (cStack_a1 < '\0') {
      FUN_00005810(local_b8[0],CONCAT17(cStack_a1,local_a8) & 0x7fffffffffffffff);
    }
    if ((local_90 != 0) && (local_98 != (long *)0x0)) {
      LOAcquire();
      lVar5 = local_98[1];
      local_98[1] = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*local_98 + 0x10))(local_98);
        if (local_98[2] != 0) {
          LOAcquire();
          lVar5 = local_98[2];
          local_98[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) {
            return;
          }
        }
        pcVar1 = *(code **)(*local_98 + 0x20);
        if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
          FUN_00005810(local_98,0x20);
        }
        else {
          (*pcVar1)(local_98);
        }
      }
    }
    return;
  }
LAB_043ef070:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43ef074);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:159946
// ==== 0x43ef2fc  objc::Categories::ArcBrowserContext_Extensions_::getImportedBookmarksTree:  (408 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getImportedBookmarksTree_
               (ID param_1,SEL param_2,ID param_3,uint param_4)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_70,param_3,(ulong)param_4);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43ef490);
    (*pcVar2)();
  }
  local_48 = FUN_043f834c;
  local_40 = &PTR_FUN_0df6b948;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_3;
  FUN_042c9d24(local_70,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043ef3d4;
      }
      pcVar2 = *(code **)(*local_68 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar2)(local_68);
      }
    }
  }
LAB_043ef3d4:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160014
// ==== 0x43ef494  objc::Categories::ArcBrowserContext_Extensions_::clearBookmarks  (540 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::clearBookmarks(ID param_1,SEL param_2)

{
  ID self;
  long lVar1;
  code *pcVar2;
  undefined1 auStack_80 [8];
  long *local_78;
  byte local_70;
  long local_68;
  long *local_60;
  byte local_58;
  undefined1 auStack_50 [32];
  
  self = _objc_msgSend(param_1,"getBookmarksTree");
  if (self == 0) {
    return;
  }
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43ef6ac);
    (*pcVar2)();
  }
  _objc_msgSend(self,"node",auStack_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (((*(long *)(local_68 + 0x88) != 0) && (lVar1 = FUN_00881874(), lVar1 != 0)) &&
     (*(char *)(lVar1 + 0x21) != '\0')) {
    FUN_00029e40(auStack_50,"ClearBookmarks",
                 "../../arc/browser/browser_context/browser_context_impl.cc",0x2c7);
    FUN_09f4e2b8(lVar1,auStack_50);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043ef564;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043ef564:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_60 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar2)(local_60);
      }
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160109
// ==== 0x43ef75c  objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree  (476 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context",&local_60);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43ef934);
    (*pcVar4)();
  }
  FUN_042c9f90(&local_48,local_60);
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_58[1];
    local_58[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar2 = local_58[2];
        local_58[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043ef7bc;
      }
      pcVar4 = *(code **)(*local_58 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar4)(local_58);
      }
    }
  }
LAB_043ef7bc:
  if ((char)local_38 == '\0') {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&class_t::ArcBookmarkTreeNode);
    if ((local_38 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    IVar3 = ArcBookmarkTreeNode::initWithBookmarkTreeNode_
                      (IVar3,"initWithBookmarkTreeNode:",&local_48);
    plVar1 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar2 = local_40[1];
      local_40[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        plVar5 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
        if (*plVar5 != 0) {
          LOAcquire();
          lVar2 = plVar1[2];
          plVar1[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) {
            return IVar3;
          }
        }
        pcVar4 = *(code **)(*plVar1 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(plVar1,0x20);
        }
        else {
          (*pcVar4)(plVar1);
        }
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160206
// ==== 0x43ef9e4  objc::Categories::ArcBrowserContext_Extensions_::addBookmark:withUrl:withParent:withIndexInParent:withUuid:  (1084 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     addBookmark_withUrl_withParent_withIndexInParent_withUuid_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               )

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  long *plVar4;
  long *plVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  ulong local_e8;
  undefined1 auStack_e0 [8];
  long *local_d8;
  byte local_d0;
  undefined8 local_c8 [2];
  undefined7 local_b8;
  char cStack_b1;
  undefined8 local_b0;
  undefined8 uStack_a8;
  long local_a0;
  undefined8 local_98;
  long *local_90;
  byte local_88;
  ulong local_80;
  long *local_78;
  ulong local_70;
  bool local_68;
  
  if (param_5 == 0) {
    return 0;
  }
  _objc_msgSend(param_5,"node",&local_80);
  plVar4 = local_78;
  if ((char)local_70 == '\0') {
    return 0;
  }
  if (local_78 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      plVar5 = local_78 + 2;
      (**(code **)(*local_78 + 0x10))(local_78);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043efb64;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_043efb64:
  if (param_7 == 0) {
    uVar6 = 0;
    plVar4 = (long *)0x0;
    local_e8 = 0;
    uVar8 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_7);
    uVar8 = local_70 >> 0x38;
    local_e8 = local_70 & 0xffffffffffffff;
    plVar4 = local_78;
    uVar6 = local_80;
  }
  _objc_msgSend(param_1,"browser_context",&local_98);
  if ((local_88 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43efe1c);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_083b1228(&local_b0,param_3);
  }
  FUN_043abdf8(local_c8,param_4);
  _objc_msgSend(param_5,"node",auStack_e0);
  if ((local_d0 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_7 == 0) {
    local_80 = local_80 & 0xffffffffffffff00;
    uVar7 = uVar6;
  }
  else {
    local_70 = local_e8 | uVar8 << 0x38;
    uVar8 = 0;
    local_e8 = 0;
    uVar7 = 0;
    local_80 = uVar6;
    local_78 = plVar4;
  }
  local_68 = param_7 != 0;
  bVar2 = FUN_042ca218(local_98,&local_b0,local_c8,auStack_e0,param_6,&local_80);
  if ((local_68 != false) && ((long)local_70 < 0)) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if ((local_d0 != 0) && (local_d8 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_d8[1];
    local_d8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_d8 + 0x10))(local_d8);
      if (local_d8[2] != 0) {
        LOAcquire();
        lVar1 = local_d8[2];
        local_d8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043efc80;
      }
      pcVar3 = *(code **)(*local_d8 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_d8,0x20);
      }
      else {
        (*pcVar3)(local_d8);
      }
    }
  }
LAB_043efc80:
  if (cStack_b1 < '\0') {
    FUN_00005810(local_c8[0],CONCAT17(cStack_b1,local_b8) & 0x7fffffffffffffff);
  }
  if (local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 << 1);
  }
  if ((local_88 != 0) && (local_90 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_90[1];
    local_90[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_90 + 0x10))(local_90);
      if (local_90[2] != 0) {
        LOAcquire();
        lVar1 = local_90[2];
        local_90[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043efcd0;
      }
      pcVar3 = *(code **)(*local_90 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_90,0x20);
      }
      else {
        (*pcVar3)(local_90);
      }
    }
  }
LAB_043efcd0:
  if (param_7 == 0) {
    return bVar2;
  }
  if ((uint)uVar8 >> 7 != 0) {
    FUN_00005810(uVar7,local_e8 | (uVar8 & 0x7f) << 0x38);
    return bVar2;
  }
  return bVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160392
// ==== 0x43eff2c  objc::Categories::ArcBrowserContext_Extensions_::renameBookmark:withTitle:  (824 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::renameBookmark_withTitle_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  code *pcVar4;
  bool bVar5;
  long *plVar6;
  undefined8 local_88;
  long *local_80;
  byte local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  long local_60;
  long local_58;
  long *local_50;
  byte local_48;
  
  if ((param_3 == 0) || (_objc_msgSend(param_3,"node",&local_58), plVar1 = local_50, local_48 == 0))
  {
    return 0;
  }
  if (local_50 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar6 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043effd8;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043effd8:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f0260);
    (*pcVar4)();
  }
  if (param_4 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    FUN_083b1228(&local_70,param_4);
  }
  _objc_msgSend(param_3,"node",&local_88);
  if ((local_78 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (*(long *)(local_58 + 0x88) == 0) {
LAB_043f004c:
    bVar5 = 0;
  }
  else {
    lVar2 = FUN_00881874();
    bVar5 = 0;
    if (lVar2 != 0) {
      if ((*(char *)(lVar2 + 0x21) == '\0') || (lVar3 = FUN_042affa0(local_88), lVar3 == 0))
      goto LAB_043f004c;
      FUN_09f4f5c4(lVar2,lVar3,&local_70,0);
      bVar5 = 1;
    }
  }
  if ((local_78 != 0) && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_80[1];
    local_80[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar2 = local_80[2];
        local_80[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0070;
      }
      pcVar4 = *(code **)(*local_80 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar4)(local_80);
      }
    }
  }
LAB_043f0070:
  if (local_60 < 0) {
    FUN_00005810(local_70,local_60 << 1);
  }
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return bVar5;
        }
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
  return bVar5;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160536
// ==== 0x43f0330  objc::Categories::ArcBrowserContext_Extensions_::editBookmark:withNewURL:  (736 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::editBookmark_withNewURL_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  bool bVar3;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_88 [8];
  long *local_80;
  byte local_78;
  undefined8 local_70 [2];
  undefined7 local_60;
  char cStack_59;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  
  if ((param_3 == 0) || (_objc_msgSend(param_3,"node",&local_58), plVar1 = local_50, local_48 == 0))
  {
    return 0;
  }
  if (local_50 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f03d0;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043f03d0:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f060c);
    (*pcVar4)();
  }
  FUN_043abdf8(local_70,param_4);
  _objc_msgSend(param_3,"node",auStack_88);
  if ((local_78 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  bVar3 = FUN_042ca078(local_58,local_70,auStack_88);
  if ((local_78 != 0) && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_80[1];
    local_80[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar2 = local_80[2];
        local_80[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0444;
      }
      pcVar4 = *(code **)(*local_80 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar4)(local_80);
      }
    }
  }
LAB_043f0444:
  if (cStack_59 < '\0') {
    FUN_00005810(local_70[0],CONCAT17(cStack_59,local_60) & 0x7fffffffffffffff);
  }
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return bVar3;
        }
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
  return bVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160659
// ==== 0x43f06dc  objc::Categories::ArcBrowserContext_Extensions_::addBookmarkFolder:withParent:withIndexInParent:withUuid:  (992 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     addBookmarkFolder_withParent_withIndexInParent_withUuid_
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5,ID param_6)

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  long *plVar7;
  ulong uVar8;
  long *plVar9;
  undefined1 auStack_c8 [8];
  long *local_c0;
  byte local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  long local_a0;
  undefined8 local_98;
  long *local_90;
  byte local_88;
  ulong local_80;
  long *local_78;
  ulong local_70;
  bool local_68;
  
  if (param_4 == 0) {
    return 0;
  }
  _objc_msgSend(param_4,"node",&local_80);
  plVar9 = local_78;
  if ((char)local_70 == '\0') {
    return 0;
  }
  if (local_78 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      plVar7 = local_78 + 2;
      (**(code **)(*local_78 + 0x10))(local_78);
      if (*plVar7 != 0) {
        LOAcquire();
        lVar1 = plVar9[2];
        plVar9[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f07c4;
      }
      pcVar3 = *(code **)(*plVar9 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar9,0x20);
      }
      else {
        (*pcVar3)(plVar9);
      }
    }
  }
LAB_043f07c4:
  if (param_6 == 0) {
    uVar4 = 0;
    plVar9 = (long *)0x0;
    uVar6 = 0;
    uVar8 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_6);
    uVar8 = local_70 >> 0x38;
    uVar6 = local_70 & 0xffffffffffffff;
    uVar4 = local_80;
    plVar9 = local_78;
  }
  _objc_msgSend(param_1,"browser_context",&local_98);
  if ((local_88 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f0ab8);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_083b1228(&local_b0,param_3);
  }
  _objc_msgSend(param_4,"node",auStack_c8);
  if ((local_b8 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_6 == 0) {
    local_80 = local_80 & 0xffffffffffffff00;
    uVar5 = uVar4;
  }
  else {
    local_70 = uVar6 | uVar8 << 0x38;
    uVar8 = 0;
    uVar6 = 0;
    uVar5 = 0;
    local_80 = uVar4;
    local_78 = plVar9;
  }
  local_68 = param_6 != 0;
  bVar2 = FUN_042ca500(local_98,&local_b0,auStack_c8,param_5,&local_80);
  if ((local_68 != false) && ((long)local_70 < 0)) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if ((local_b8 != 0) && (local_c0 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_c0[1];
    local_c0[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_c0 + 0x10))(local_c0);
      if (local_c0[2] != 0) {
        LOAcquire();
        lVar1 = local_c0[2];
        local_c0[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f08c4;
      }
      pcVar3 = *(code **)(*local_c0 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_c0,0x20);
      }
      else {
        (*pcVar3)(local_c0);
      }
    }
  }
LAB_043f08c4:
  if (local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 << 1);
  }
  if ((local_88 != 0) && (local_90 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_90[1];
    local_90[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_90 + 0x10))(local_90);
      if (local_90[2] != 0) {
        LOAcquire();
        lVar1 = local_90[2];
        local_90[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f08fc;
      }
      pcVar3 = *(code **)(*local_90 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_90,0x20);
      }
      else {
        (*pcVar3)(local_90);
      }
    }
  }
LAB_043f08fc:
  if (param_6 == 0) {
    return bVar2;
  }
  if ((uint)uVar8 >> 7 != 0) {
    FUN_00005810(uVar5,uVar6 | (uVar8 & 0x7f) << 0x38);
    return bVar2;
  }
  return bVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160837
// ==== 0x43f0ba8  objc::Categories::ArcBrowserContext_Extensions_::removeBookmark:withParent:  (900 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::removeBookmark_withParent_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long *plVar1;
  long lVar2;
  bool bVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_88;
  long *local_80;
  ulong local_78;
  undefined1 auStack_70 [8];
  long *local_68;
  byte local_60;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  
  if ((param_3 == 0) || (_objc_msgSend(param_3,"node",&local_58), plVar1 = local_50, local_48 == 0))
  {
    return 0;
  }
  if (local_50 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_50 + 2;
      (**(code **)(*local_50 + 0x10))(local_50);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0c48;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_043f0c48:
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f0f20);
    (*pcVar4)();
  }
  _objc_msgSend(param_3,"node",auStack_70);
  if ((local_60 & 1) == 0) {
LAB_043f0f28:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (param_4 == 0) {
    local_88 = 0;
    local_80 = (long *)0x0;
    local_78 = 0;
    goto LAB_043f0f28;
  }
  _objc_msgSend(param_4,"node",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043f0f28;
  bVar3 = FUN_042ca6d4(local_58,auStack_70,&local_88);
  if (((char)local_78 != '\0') && (local_80 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_80[1];
    local_80[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_80 + 0x10))(local_80);
      if (local_80[2] != 0) {
        LOAcquire();
        lVar2 = local_80[2];
        local_80[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0ccc;
      }
      pcVar4 = *(code **)(*local_80 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_80,0x20);
      }
      else {
        (*pcVar4)(local_80);
      }
    }
  }
LAB_043f0ccc:
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_68[1];
    local_68[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar2 = local_68[2];
        local_68[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f0cec;
      }
      pcVar4 = *(code **)(*local_68 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar4)(local_68);
      }
    }
  }
LAB_043f0cec:
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return bVar3;
        }
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
  return bVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:160989
// ==== 0x43f1080  objc::Categories::ArcBrowserContext_Extensions_::moveBookmark:newParent:atIndex:  (1296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::moveBookmark_newParent_atIndex_
               (ID param_1,SEL param_2,ID param_3,ID param_4,long_long param_5)

{
  bool bVar1;
  long *plVar2;
  long lVar3;
  bool bVar4;
  code *pcVar5;
  long *plVar6;
  undefined8 local_a8;
  long *local_a0;
  ulong local_98;
  undefined8 local_90;
  long *plStack_88;
  undefined8 local_80;
  long *plStack_78;
  byte local_70;
  undefined1 auStack_68 [8];
  long *local_60;
  byte local_58;
  
  if (param_3 == 0) {
    return 0;
  }
  _objc_msgSend(param_3,"node",auStack_68);
  if (local_58 == 0) {
    return 0;
  }
  if ((param_4 == 0) ||
     (_objc_msgSend(param_4,"node",&local_80), plVar2 = plStack_78, local_70 == 0)) {
    bVar1 = true;
  }
  else {
    if (plStack_78 != (long *)0x0) {
      LOAcquire();
      lVar3 = plStack_78[1];
      plStack_78[1] = lVar3 + -1;
      LORelease();
      if (lVar3 == 0) {
        plVar6 = plStack_78 + 2;
        (**(code **)(*plStack_78 + 0x10))(plStack_78);
        if (*plVar6 != 0) {
          LOAcquire();
          lVar3 = plVar2[2];
          plVar2[2] = lVar3 + -1;
          LORelease();
          if (lVar3 != 0) goto LAB_043f1148;
        }
        pcVar5 = *(code **)(*plVar2 + 0x20);
        if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
          FUN_00005810(plVar2,0x20);
        }
        else {
          (*pcVar5)(plVar2);
        }
      }
    }
LAB_043f1148:
    bVar1 = false;
  }
  plVar2 = local_60;
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_60[1];
    local_60[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = local_60 + 2;
      (**(code **)(*local_60 + 0x10))(local_60);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto joined_r0x043f1174;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
joined_r0x043f1174:
  if (bVar1) {
    return 0;
  }
  _objc_msgSend(param_1,"browser_context",auStack_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43f1584);
    (*pcVar5)();
  }
  _objc_msgSend(param_3,"node",&local_80);
  if ((local_70 & 1) == 0) {
LAB_043f158c:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  plStack_88 = plStack_78;
  local_90 = local_80;
  local_80 = 0;
  plStack_78 = (long *)0x0;
  if (param_4 == 0) {
    local_a8 = 0;
    local_a0 = (long *)0x0;
    local_98 = 0;
    goto LAB_043f158c;
  }
  _objc_msgSend(param_4,"node",&local_a8);
  if ((local_98 & 1) == 0) goto LAB_043f158c;
  bVar4 = FUN_04333ad8(auStack_68,&local_90,&local_a8,param_5);
  if (((char)local_98 != '\0') && (local_a0 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_a0[1];
    local_a0[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_a0 + 0x10))(local_a0);
      if (local_a0[2] != 0) {
        LOAcquire();
        lVar3 = local_a0[2];
        local_a0[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f120c;
      }
      pcVar5 = *(code **)(*local_a0 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_a0,0x20);
      }
      else {
        (*pcVar5)(local_a0);
      }
    }
  }
LAB_043f120c:
  plVar2 = plStack_88;
  if (plStack_88 != (long *)0x0) {
    LOAcquire();
    lVar3 = plStack_88[1];
    plStack_88[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = plStack_88 + 2;
      (**(code **)(*plStack_88 + 0x10))(plStack_88);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f1224;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
LAB_043f1224:
  plVar2 = plStack_78;
  if ((local_70 != 0) && (plStack_78 != (long *)0x0)) {
    LOAcquire();
    lVar3 = plStack_78[1];
    plStack_78[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      plVar6 = plStack_78 + 2;
      (**(code **)(*plStack_78 + 0x10))(plStack_78);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar3 = plVar2[2];
        plVar2[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043f1244;
      }
      pcVar5 = *(code **)(*plVar2 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar5)(plVar2);
      }
    }
  }
LAB_043f1244:
  if (local_58 == 0) {
    return bVar4;
  }
  if (local_60 == (long *)0x0) {
    return bVar4;
  }
  LOAcquire();
  lVar3 = local_60[1];
  local_60[1] = lVar3 + -1;
  LORelease();
  if (lVar3 != 0) {
    return bVar4;
  }
  (**(code **)(*local_60 + 0x10))(local_60);
  if (local_60[2] != 0) {
    LOAcquire();
    lVar3 = local_60[2];
    local_60[2] = lVar3 + -1;
    LORelease();
    if (lVar3 != 0) {
      return bVar4;
    }
  }
  pcVar5 = *(code **)(*local_60 + 0x20);
  if (((pcVar5 != FUN_00f01bc0) && (pcVar5 != FUN_00f01bc0)) && (pcVar5 != FUN_013efb40)) {
    (*pcVar5)(local_60);
    return bVar4;
  }
  FUN_00005810(local_60,0x20);
  return bVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161219
// ==== 0x43f1780  objc::Categories::ArcBrowserContext_Extensions_::isBookmarked:  (316 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::isBookmarked_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  undefined8 local_60 [2];
  undefined7 local_50;
  char cStack_49;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f18bc);
    (*pcVar3)();
  }
  FUN_043abdf8(local_60,param_3);
  bVar2 = FUN_042ca760(local_48,local_60);
  if (cStack_49 < '\0') {
    FUN_00005810(local_60[0],CONCAT17(cStack_49,local_50) & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return bVar2;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar3)(local_40);
      }
    }
  }
  return bVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161279
// ==== 0x43f1968  objc::Categories::ArcBrowserContext_Extensions_::addBookmarkModelObserver:  (1456 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::addBookmarkModelObserver_
               (ID param_1,SEL param_2,ID param_3)

{
  ulong uVar1;
  bool bVar2;
  long lVar3;
  long *plVar4;
  ulong uVar5;
  code *pcVar6;
  ulong uVar7;
  long *plVar8;
  long *plVar9;
  long *plVar10;
  long lVar11;
  size_t sVar12;
  long *plVar13;
  long *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined1 auStack_90 [8];
  long *local_88;
  byte local_80;
  long *local_78;
  ulong local_70;
  ulong local_68;
  
  local_78 = (long *)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"bookmarkObservers",&local_78);
  plVar10 = local_78;
  for (lVar11 = local_70 << 4; lVar11 != 0; lVar11 = lVar11 + -0x10) {
    if (*(ID *)(*plVar10 + 8) == param_3) goto LAB_043f1d08;
    plVar10 = plVar10 + 2;
  }
  plVar4 = (long *)FUN_00008790(0x28);
  plVar8 = plVar4 + 1;
  *plVar8 = 0;
  plVar4[2] = 0;
  *plVar4 = (long)&PTR_viewWillStartLiveResize_0da87a98;
  plVar10 = plVar4 + 3;
  if (plVar10 == (long *)0x0) goto LAB_043f1bd8;
  plVar4[3] = (long)&PTR_FUN_0da87ad0;
  _objc_initWeak(plVar4 + 4,param_3);
  _objc_msgSend(param_1,"browser_context",auStack_90);
  uVar5 = (ulong)local_80;
  if ((local_80 & 1) == 0) goto LAB_043f1bd8;
  if (plVar4 != (long *)0x0) {
    uVar5 = plVar4[2];
    plVar4[2] = uVar5 + 1;
  }
  FUN_04333c0c(uVar5,auStack_90,plVar10,plVar4);
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar11 = local_88[1];
    local_88[1] = lVar11 + -1;
    LORelease();
    if (lVar11 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar11 = local_88[2];
        local_88[2] = lVar11 + -1;
        LORelease();
        if (lVar11 != 0) goto LAB_043f1a94;
      }
      pcVar6 = *(code **)(*local_88 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar6)(local_88);
      }
    }
  }
LAB_043f1a94:
  uVar5 = local_70;
  if (local_70 == local_68) {
    uVar1 = local_70 + 1;
    if (uVar1 >> 0x3c != 0) goto LAB_043f2028;
    uVar7 = local_70 * 2;
    if (uVar7 < uVar1 || uVar7 - uVar1 == 0) {
      uVar7 = uVar1;
    }
    if (0x7fffffffffffffe < local_70) {
      uVar7 = 0xfffffffffffffff;
    }
    if (uVar7 == 0) {
      lVar11 = 0;
    }
    else {
      if (uVar7 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      lVar11 = FUN_00008790(uVar7 << 4);
    }
    plVar13 = (long *)(lVar11 + uVar5 * 0x10);
    if (plVar13 == (long *)0x0) goto LAB_043f1bd8;
    *plVar13 = (long)plVar10;
    plVar13[1] = (long)plVar4;
    if (plVar4 != (long *)0x0) {
      *plVar8 = *plVar8 + 1;
    }
    plVar9 = plVar13 + local_70 * -2;
    sVar12 = local_70 * 0x10;
    _memcpy(plVar9,local_78,sVar12);
    plVar10 = local_78;
    local_70 = (long)(sVar12 + 0x10) >> 4;
    bVar2 = local_78 != (long *)0x0;
    local_78 = plVar9;
    if (bVar2) {
      lVar11 = local_68 << 4;
      local_68 = uVar7;
      FUN_00005810(plVar10,lVar11);
      uVar7 = local_68;
    }
  }
  else {
    plVar13 = local_78 + local_70 * 2;
    if (plVar13 == (long *)0x0) goto LAB_043f1bd8;
    *plVar13 = (long)plVar10;
    plVar13[1] = (long)plVar4;
    uVar7 = local_68;
    if (plVar4 != (long *)0x0) {
      *plVar8 = *plVar8 + 1;
    }
  }
  local_68 = uVar7;
  plVar10 = local_78;
  lVar11 = (long)plVar13 + (0x10 - (long)local_78);
  uVar5 = lVar11 >> 4;
  local_70 = uVar5;
  if (lVar11 == 0) {
LAB_043f1bd8:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x43f1bdc);
    (*pcVar6)();
  }
  local_a8 = (long *)0x0;
  local_a0 = 0;
  local_98 = 0;
  if (uVar5 >> 0x3c != 0) {
LAB_043f2028:
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  local_a8 = (long *)FUN_00008790();
  plVar9 = local_a8;
  do {
    local_98 = uVar5;
    if (plVar9 == (long *)0x0) goto LAB_043f1bd8;
    *plVar9 = *plVar10;
    lVar11 = plVar10[1];
    plVar9[1] = lVar11;
    if (lVar11 != 0) {
      *(long *)(lVar11 + 8) = *(long *)(lVar11 + 8) + 1;
    }
    plVar9 = plVar9 + 2;
    bVar2 = plVar10 != plVar13;
    plVar10 = plVar10 + 2;
  } while (bVar2);
  local_a0 = (long)plVar9 - (long)local_a8 >> 4;
  _objc_msgSend(param_1,"setBookmarkObservers:",&local_a8);
  plVar10 = local_a8;
  if (local_a8 != (long *)0x0) {
    if ((local_a0 & 0xfffffffffffffff) != 0) {
      lVar11 = local_a0 * -0x10;
      plVar13 = local_a8 + local_a0 * 2;
      do {
        if (plVar13 + -2 == (long *)0x0) goto LAB_043f1bd8;
        plVar9 = (long *)plVar13[-1];
        if (plVar9 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar9[1];
          plVar9[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar9 + 0x10))(plVar9);
            if (plVar9[2] != 0) {
              LOAcquire();
              lVar3 = plVar9[2];
              plVar9[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f1be8;
            }
            pcVar6 = *(code **)(*plVar9 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar9,0x20);
            }
            else {
              (*pcVar6)(plVar9);
            }
          }
        }
LAB_043f1be8:
        lVar11 = lVar11 + 0x10;
        plVar13 = plVar13 + -2;
      } while (lVar11 != 0);
    }
    local_a0 = (long)plVar10 - (long)local_a8 >> 4;
    FUN_00005810(local_a8,local_98 << 4);
  }
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar11 = *plVar8;
    *plVar8 = lVar11 + -1;
    LORelease();
    if (lVar11 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar11 = plVar4[2];
        plVar4[2] = lVar11 + -1;
        LORelease();
        if (lVar11 != 0) goto LAB_043f1d08;
      }
      pcVar6 = *(code **)(*plVar4 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar6)(plVar4);
      }
    }
  }
LAB_043f1d08:
  plVar10 = local_78;
  if (local_78 != (long *)0x0) {
    if ((local_70 & 0xfffffffffffffff) != 0) {
      lVar11 = local_70 * -0x10;
      plVar4 = local_78 + local_70 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f1bd8;
        plVar8 = (long *)plVar4[-1];
        if (plVar8 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar8[1];
          plVar8[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar8 + 0x10))(plVar8);
            if (plVar8[2] != 0) {
              LOAcquire();
              lVar3 = plVar8[2];
              plVar8[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f1da0;
            }
            pcVar6 = *(code **)(*plVar8 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar8,0x20);
            }
            else {
              (*pcVar6)(plVar8);
            }
          }
        }
LAB_043f1da0:
        lVar11 = lVar11 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar11 != 0);
    }
    local_70 = (long)plVar10 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161559
// ==== 0x43f21a8  objc::Categories::ArcBrowserContext_Extensions_::removeBookmarkModelObserver:  (1460 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::removeBookmarkModelObserver_
               (ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  ulong uVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  long *plVar9;
  long lVar10;
  long *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined1 auStack_90 [8];
  long *local_88;
  byte local_80;
  long *local_78;
  ulong local_70;
  long local_68;
  
  local_78 = (long *)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"bookmarkObservers",&local_78);
  lVar10 = local_70 << 4;
  plVar9 = local_78;
  if (lVar10 != 0) {
    plVar4 = local_78;
    do {
      plVar9 = plVar4;
      if (*(ID *)(*plVar4 + 8) == param_3) break;
      plVar4 = plVar4 + 2;
      lVar10 = lVar10 + -0x10;
      plVar9 = local_78 + local_70 * 2;
    } while (lVar10 != 0);
  }
  if (plVar9 == local_78 + local_70 * 2) goto LAB_043f2620;
  _objc_msgSend(param_1,"browser_context",auStack_90);
  uVar5 = (ulong)local_80;
  if ((local_80 & 1) == 0) goto LAB_043f256c;
  lVar10 = *plVar9;
  if (plVar9[1] != 0) {
    puVar1 = (ulong *)(plVar9[1] + 0x10);
    uVar5 = *puVar1;
    *puVar1 = uVar5 + 1;
  }
  FUN_04333cc4(uVar5,auStack_90,lVar10);
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar10 = local_88[1];
    local_88[1] = lVar10 + -1;
    LORelease();
    if (lVar10 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar10 = local_88[2];
        local_88[2] = lVar10 + -1;
        LORelease();
        if (lVar10 != 0) goto LAB_043f22a8;
      }
      pcVar6 = *(code **)(*local_88 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar6)(local_88);
      }
    }
  }
LAB_043f22a8:
  plVar4 = local_78 + local_70 * 2;
  if (plVar9 == plVar4) {
LAB_043f256c:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x43f2570);
    (*pcVar6)();
  }
  plVar7 = plVar9;
  if ((long)plVar9 + (0x10 - (long)local_78) != local_70 * 0x10) {
    do {
      lVar3 = plVar9[3];
      lVar10 = plVar9[2];
      plVar9[3] = 0;
      plVar9[2] = 0;
      plVar7 = (long *)plVar9[1];
      plVar9[1] = lVar3;
      *plVar9 = lVar10;
      if (plVar7 != (long *)0x0) {
        LOAcquire();
        lVar10 = plVar7[1];
        plVar7[1] = lVar10 + -1;
        LORelease();
        if (lVar10 == 0) {
          (**(code **)(*plVar7 + 0x10))(plVar7);
          if (plVar7[2] != 0) {
            LOAcquire();
            lVar10 = plVar7[2];
            plVar7[2] = lVar10 + -1;
            LORelease();
            if (lVar10 != 0) goto LAB_043f2348;
          }
          pcVar6 = *(code **)(*plVar7 + 0x20);
          if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
            FUN_00005810(plVar7,0x20);
          }
          else {
            (*pcVar6)(plVar7);
          }
        }
      }
LAB_043f2348:
      plVar7 = plVar9 + 2;
      plVar2 = plVar9 + 4;
      plVar9 = plVar7;
    } while (plVar2 != plVar4);
  }
  plVar4 = local_78 + local_70 * 2;
  plVar9 = local_78;
joined_r0x043f2478:
  local_78 = plVar9;
  if (plVar7 != plVar4) {
    plVar2 = plVar4 + -2;
    if (plVar2 == (long *)0x0) goto LAB_043f256c;
    plVar8 = (long *)plVar4[-1];
    plVar4 = plVar2;
    if (plVar8 != (long *)0x0) {
      LOAcquire();
      lVar10 = plVar8[1];
      plVar8[1] = lVar10 + -1;
      LORelease();
      if (lVar10 == 0) {
        (**(code **)(*plVar8 + 0x10))(plVar8);
        if (plVar8[2] != 0) {
          LOAcquire();
          lVar10 = plVar8[2];
          plVar8[2] = lVar10 + -1;
          LORelease();
          plVar9 = local_78;
          if (lVar10 != 0) goto joined_r0x043f2478;
        }
        pcVar6 = *(code **)(*plVar8 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar8,0x20);
          plVar9 = local_78;
        }
        else {
          (*pcVar6)(plVar8);
          plVar9 = local_78;
        }
      }
    }
    goto joined_r0x043f2478;
  }
  local_a0 = 0;
  local_98 = 0;
  local_a8 = (long *)0x0;
  uVar5 = (long)plVar7 - (long)plVar9 >> 4;
  local_70 = uVar5;
  if ((long)plVar7 - (long)plVar9 != 0) {
    if (uVar5 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    local_a8 = (long *)FUN_00008790();
    local_a0 = 0;
    plVar4 = local_a8;
    do {
      local_98 = uVar5;
      if (plVar4 == (long *)0x0) goto LAB_043f256c;
      *plVar4 = *plVar9;
      lVar10 = plVar9[1];
      plVar4[1] = lVar10;
      if (lVar10 != 0) {
        *(long *)(lVar10 + 8) = *(long *)(lVar10 + 8) + 1;
      }
      plVar9 = plVar9 + 2;
      plVar4 = plVar4 + 2;
    } while (plVar9 != plVar7);
    local_a0 = (long)plVar4 - (long)local_a8 >> 4;
  }
  _objc_msgSend(param_1,"setBookmarkObservers:",&local_a8);
  plVar9 = local_a8;
  if (local_a8 != (long *)0x0) {
    if ((local_a0 & 0xfffffffffffffff) != 0) {
      lVar10 = local_a0 * -0x10;
      plVar4 = local_a8 + local_a0 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f256c;
        plVar7 = (long *)plVar4[-1];
        if (plVar7 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar7[1];
          plVar7[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar7 + 0x10))(plVar7);
            if (plVar7[2] != 0) {
              LOAcquire();
              lVar3 = plVar7[2];
              plVar7[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f257c;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_043f257c:
        lVar10 = lVar10 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar10 != 0);
    }
    local_a0 = (long)plVar9 - (long)local_a8 >> 4;
    FUN_00005810(local_a8,local_98 << 4);
  }
LAB_043f2620:
  plVar9 = local_78;
  if (local_78 != (long *)0x0) {
    if ((local_70 & 0xfffffffffffffff) != 0) {
      lVar10 = local_70 * -0x10;
      plVar4 = local_78 + local_70 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f256c;
        plVar7 = (long *)plVar4[-1];
        if (plVar7 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar7[1];
          plVar7[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar7 + 0x10))(plVar7);
            if (plVar7[2] != 0) {
              LOAcquire();
              lVar3 = plVar7[2];
              plVar7[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f26b0;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_043f26b0:
        lVar10 = lVar10 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar10 != 0);
    }
    local_70 = (long)plVar9 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161837
// ==== 0x43f2954  objc::Categories::ArcBrowserContext_Extensions_::deleteAllCookies:  (408 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::deleteAllCookies_
               (ID param_1,SEL param_2,ID param_3,uint param_4)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_70,param_3,(ulong)param_4);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2ae8);
    (*pcVar2)();
  }
  local_48 = &LAB_043f88c8;
  local_40 = &PTR_FUN_0df6b968;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_3;
  FUN_042cb3b4(local_70,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f2a2c;
      }
      pcVar2 = *(code **)(*local_68 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar2)(local_68);
      }
    }
  }
LAB_043f2a2c:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161905
// ==== 0x43f2aec  objc::Categories::ArcBrowserContext_Extensions_::getAllCookies:  (444 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAllCookies_
               (ID param_1,SEL param_2,ID param_3,uint param_4)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_88;
  undefined8 uStack_80;
  ulong local_78;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_70,param_3,(ulong)param_4);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2ca4);
    (*pcVar2)();
  }
  local_88 = 0;
  uStack_80 = 0;
  local_78 = 0;
  local_48 = FUN_043e5ef8;
  local_40 = &PTR_FUN_0df6b988;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_3;
  FUN_042cba00(local_70,&local_88,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((long)local_78 < 0) {
    FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
  }
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f2be8;
      }
      pcVar2 = *(code **)(*local_68 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar2)(local_68);
      }
    }
  }
LAB_043f2be8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:161982
// ==== 0x43f2ca8  objc::Categories::ArcBrowserContext_Extensions_::getAllCookies:completion:  (476 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAllCookies_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80,param_3,param_4,(ulong)param_5);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f2e80);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  local_58 = FUN_043e5ef8;
  local_50 = &PTR_FUN_0df6b9a8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_042cba00(local_80,local_98,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f2db8;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f2db8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162057
// ==== 0x43f2f30  objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage:key:completion:  (568 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getLocalStorage_key_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f3164);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  if (param_4 == 0) {
    local_b0 = 0;
    uStack_a8 = 0;
    local_a0 = 0;
  }
  else {
    FUN_01b356b8(&local_b0,param_4);
  }
  local_58 = FUN_043f88dc;
  local_50 = &PTR_FUN_0df6b9c8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_5;
  FUN_042cc3f0(local_80,local_98,&local_b0,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f3094;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f3094:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162146
// ==== 0x43f3234  objc::Categories::ArcBrowserContext_Extensions_::setLocalStorage:key:value:completion:  (668 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x30 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::setLocalStorage_key_value_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,
               undefined4 param_7)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_d8;
  undefined8 uStack_d0;
  ulong local_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f34cc);
    (*pcVar2)();
  }
  FUN_043abdf8(local_a8,param_3);
  if (param_4 == 0) {
    local_c0 = 0;
    uStack_b8 = 0;
    local_b0 = 0;
  }
  else {
    FUN_01b356b8(&local_c0,param_4);
  }
  if (param_5 == 0) {
    local_d8 = 0;
    uStack_d0 = 0;
    local_c8 = 0;
  }
  else {
    FUN_01b356b8(&local_d8,param_5);
  }
  local_68 = &LAB_04045100;
  local_60 = &PTR_FUN_0df6b9e8;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_6;
  FUN_042ccef8(local_90,local_a8,&local_c0,&local_d8,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if ((long)local_c8 < 0) {
    FUN_00005810(local_d8,local_c8 & 0x7fffffffffffffff);
  }
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_88[1];
    local_88[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar1 = local_88[2];
        local_88[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f33f0;
      }
      pcVar2 = *(code **)(*local_88 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar2)(local_88);
      }
    }
  }
LAB_043f33f0:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162250
// ==== 0x43f35bc  objc::Categories::ArcBrowserContext_Extensions_::createWebContents:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createWebContents_
             (ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 local_58;
  long *local_50;
  byte local_48;
  undefined8 local_40;
  long *local_38;
  
  local_40 = 0;
  local_38 = (long *)0x0;
  _objc_msgSend(param_1,"browser_context",&local_58);
  if ((local_48 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f37b8);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    _objc_msgSend(param_3,"nativeWebContentsConfiguration",&local_70);
  }
  FUN_042d7064(&local_40,local_58,&local_70);
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f3648;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_043f3648:
  IVar3 = _objc_alloc(&class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",&local_40);
  plVar1 = local_38;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_38[1];
    local_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162349
// ==== 0x43f3874  objc::Categories::ArcBrowserContext_Extensions_::import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:  (1732 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Removing unreachable block (ram,0x043f3a18) */
/* WARNING: Removing unreachable block (ram,0x043f3e94) */
/* WARNING: Removing unreachable block (ram,0x043f3ed8) */
/* WARNING: Removing unreachable block (ram,0x043f3ee8) */
/* WARNING: Removing unreachable block (ram,0x043f3eb0) */
/* WARNING: Removing unreachable block (ram,0x043f3eec) */
/* WARNING: Removing unreachable block (ram,0x043f3efc) */
/* WARNING: Removing unreachable block (ram,0x043f3ec8) */
/* WARNING: Removing unreachable block (ram,0x043f3f0c) */
/* Function Stack Size: 0x44 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::
     import_withCategories_withAllowedExtensions_withBookmarkImportTargets_skipExistingCookies_withDelegate_error_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,ID param_5,ID param_6,
               bool param_7,ID param_8,ID *param_9)

{
  void *pvVar1;
  long *plVar2;
  ulong uVar3;
  code *pcVar4;
  uint uVar5;
  long lVar6;
  bool bVar7;
  ID IVar8;
  ID IVar9;
  undefined8 *puVar10;
  void *pvVar11;
  uint *puVar12;
  long *plVar13;
  undefined8 *puVar14;
  size_t sVar15;
  uint *local_178;
  ulong local_170;
  long *plStack_168;
  undefined8 uStack_160;
  long *plStack_158;
  char local_150;
  void *local_148;
  long local_140;
  ID IStack_138;
  char local_130;
  uint *local_128;
  undefined8 local_120;
  long *plStack_118;
  ulong local_110;
  ulong local_100;
  long *plStack_f8;
  byte local_f0;
  ulong local_e0;
  long *plStack_d8;
  undefined8 local_d0;
  long *plStack_c8;
  undefined8 local_c0;
  undefined *local_b8;
  undefined8 uStack_b0;
  undefined1 *local_a8;
  undefined *puStack_a0;
  void **local_98;
  void *local_90;
  long lStack_88;
  ID local_80;
  undefined8 local_78;
  
  lStack_88 = 0;
  local_90 = (void *)0x0;
  local_78 = 0;
  local_80 = 0;
  if (param_5 != 0) {
    local_90 = (void *)0x0;
    lStack_88 = 0;
    local_80 = 0;
    local_78 = 1;
    IVar8 = _objc_msgSend(param_5,"count");
    IVar9 = local_80;
    lVar6 = lStack_88;
    if (local_80 < IVar8) {
      if (0xaaaaaaaaaaaaaaa < IVar8) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar11 = (void *)FUN_00008790(IVar8 * 0x18);
      pvVar1 = local_90;
      sVar15 = lVar6 * 0x18;
      if (sVar15 != 0) {
        _memcpy(pvVar11,local_90,sVar15);
      }
      lStack_88 = ((long)sVar15 >> 3) * -0x5555555555555555;
      local_90 = pvVar11;
      local_80 = IVar8;
      if (pvVar1 != (void *)0x0) {
        FUN_00005810(pvVar1,IVar9 * 0x18);
      }
    }
    local_b8 = PTR___NSConcreteGlobalBlock_0d824130;
    uStack_b0 = 0xd0800000;
    local_a8 = &LAB_043f4214;
    puStack_a0 = &DAT_0df6b218;
    local_98 = &local_90;
    _objc_msgSend(param_5,"enumerateObjectsUsingBlock:",&local_b8);
  }
  local_c0 = 0;
  plStack_d8 = (long *)0x0;
  local_e0 = 0;
  plStack_c8 = (long *)0x0;
  local_d0 = 0;
  if (param_6 != 0) {
    IVar9 = _objc_msgSend(param_6,"targetNodeForBookmarkBarBookmarks");
    if ((((IVar9 == 0) || (_objc_msgSend(IVar9,"node",&local_100), (local_f0 & 1) == 0)) ||
        (IVar9 = _objc_msgSend(param_6,"targetNodeForOtherBookmarks"), IVar9 == 0)) ||
       (_objc_msgSend(IVar9,"node",&local_120), (local_110 & 1) == 0)) goto LAB_043f3e18;
    if ((char)local_c0 != '\0') {
      FUN_04333708(&local_e0);
    }
    plStack_d8 = plStack_f8;
    local_e0 = local_100;
    plStack_c8 = plStack_118;
    local_d0 = local_120;
    local_100 = 0;
    plStack_f8 = (long *)0x0;
    local_120 = 0;
    plStack_118 = (long *)0x0;
    local_c0 = CONCAT71(local_c0._1_7_,1);
  }
  local_128 = (uint *)0x0;
  _objc_msgSend(param_1,"browser_context",&local_100);
  if ((local_f0 & 1) == 0) {
LAB_043f3e18:
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43f3e1c);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_120 = 0;
    plStack_118 = (long *)0x0;
    local_110 = 0;
LAB_043f3f20:
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  _objc_msgSend(param_3,"profile",&local_120);
  IVar9 = local_80;
  lVar6 = lStack_88;
  if ((local_110 & 1) == 0) goto LAB_043f3f20;
  local_148 = (void *)((ulong)local_148 & 0xffffffffffffff00);
  local_130 = (char)local_78 != '\0';
  if ((bool)local_130) {
    local_148 = local_90;
    local_90 = (void *)0x0;
    lStack_88 = 0;
    local_80 = 0;
    IStack_138 = IVar9;
    local_140 = lVar6;
  }
  puVar10 = (undefined8 *)FUN_00008790(0x10);
  *puVar10 = &PTR_FUN_0da87958;
  _objc_initWeak(puVar10 + 1,param_8);
  plVar2 = plStack_d8;
  uVar3 = local_e0;
  local_170 = local_170 & 0xffffffffffffff00;
  local_150 = (char)local_c0 != '\0';
  if ((bool)local_150) {
    local_e0 = 0;
    plStack_d8 = (long *)0x0;
    plStack_168 = plVar2;
    local_170 = uVar3;
    plStack_158 = plStack_c8;
    uStack_160 = local_d0;
    local_d0 = 0;
    plStack_c8 = (long *)0x0;
  }
  FUN_04333d7c(&local_128,&local_100,&local_120,(uint)param_4 & 0xfff,&local_148,puVar10,&local_170,
               param_7);
  if (local_150 != '\0') {
    FUN_04333708(&local_170);
  }
  pvVar1 = local_148;
  if ((local_130 != '\0') && (local_148 != (void *)0x0)) {
    if (local_140 * 0x18 != 0) {
      lVar6 = local_140 * -0x18;
      puVar10 = (undefined8 *)(local_140 * 0x18 + (long)local_148);
      do {
        puVar14 = puVar10 + -3;
        if (puVar14 == (undefined8 *)0x0) goto LAB_043f3e18;
        if (*(char *)((long)puVar10 + -1) < '\0') {
          FUN_00005810(*puVar14,puVar10[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar10 = puVar14;
      } while (lVar6 != 0);
    }
    local_140 = ((long)pvVar1 - (long)local_148 >> 3) * -0x5555555555555555;
    FUN_00005810(local_148,IStack_138 * 0x18);
  }
  plVar2 = plStack_118;
  if (((char)local_110 != '\0') && (plStack_118 != (long *)0x0)) {
    LOAcquire();
    lVar6 = plStack_118[1];
    plStack_118[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar13 = plStack_118 + 2;
      (**(code **)(*plStack_118 + 0x10))(plStack_118);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar6 = plVar2[2];
        plVar2[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_043f3b54;
      }
      pcVar4 = *(code **)(*plVar2 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar4)(plVar2);
      }
    }
  }
LAB_043f3b54:
  plVar2 = plStack_f8;
  if ((local_f0 != 0) && (plStack_f8 != (long *)0x0)) {
    LOAcquire();
    lVar6 = plStack_f8[1];
    plStack_f8[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar13 = plStack_f8 + 2;
      (**(code **)(*plStack_f8 + 0x10))(plStack_f8);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar6 = plVar2[2];
        plVar2[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_043f3b78;
      }
      pcVar4 = *(code **)(*plVar2 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar4)(plVar2);
      }
    }
  }
LAB_043f3b78:
  local_178 = local_128;
  puVar12 = local_128;
  if (((ulong)local_128 & 1) == 0) {
    uVar5 = *local_128;
    *local_128 = uVar5 + 1;
    puVar12 = (uint *)(ulong)uVar5;
  }
  bVar7 = FUN_04402548(puVar12,&local_178,param_9);
  if (((ulong)local_178 & 1) == 0) {
    FUN_04b35898();
  }
  if (((ulong)local_128 & 1) == 0) {
    FUN_04b35898();
  }
  if ((char)local_c0 != '\0') {
    FUN_04333708(&local_e0);
  }
  pvVar1 = local_90;
  if (((char)local_78 != '\0') && (local_90 != (void *)0x0)) {
    if (lStack_88 * 0x18 != 0) {
      lVar6 = lStack_88 * -0x18;
      puVar10 = (undefined8 *)(lStack_88 * 0x18 + (long)local_90);
      do {
        puVar14 = puVar10 + -3;
        if (puVar14 == (undefined8 *)0x0) goto LAB_043f3e18;
        if (*(char *)((long)puVar10 + -1) < '\0') {
          FUN_00005810(*puVar14,puVar10[-1] & 0x7fffffffffffffff);
        }
        lVar6 = lVar6 + 0x18;
        puVar10 = puVar14;
      } while (lVar6 != 0);
    }
    lStack_88 = ((long)pvVar1 - (long)local_90 >> 3) * -0x5555555555555555;
    FUN_00005810(local_90,local_80 * 0x18);
  }
  return bVar7;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162640
// ==== 0x43f4390  objc::Categories::ArcBrowserContext_Extensions_::importPasswords:  (340 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::importPasswords_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f44e4);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  FUN_042d5d2c(local_48,&local_60);
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162706
// ==== 0x43f4590  objc::Categories::ArcBrowserContext_Extensions_::favicon:completion:  (516 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::favicon_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_d8;
  long *local_d0;
  byte local_c8;
  ID *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  ID *local_68;
  undefined8 uStack_60;
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_b8 = 0;
  local_c0 = (ID *)0x0;
  FUN_043abdf8(&local_68,param_3);
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  uStack_b8 = uStack_60;
  local_c0 = local_68;
  local_b0 = (ulong)local_58;
  _objc_msgSend(param_1,"browser_context",&local_d8);
  if ((local_c8 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f4790);
    (*pcVar2)();
  }
  local_58 = FUN_043f8954;
  local_50 = &PTR_FUN_0df6ba08;
  local_68 = (ID *)FUN_00008790(8);
  *local_68 = param_4;
  FUN_042d5f00(local_d8,&local_c0,&local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68);
  }
  if ((local_c8 != 0) && (local_d0 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_d0[1];
    local_d0[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_d0 + 0x10))(local_d0);
      if (local_d0[2] != 0) {
        LOAcquire();
        lVar1 = local_d0[2];
        local_d0[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f46c0;
      }
      pcVar2 = *(code **)(*local_d0 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_d0,0x20);
      }
      else {
        (*pcVar2)(local_d0);
      }
    }
  }
LAB_043f46c0:
  FUN_041ab210(&local_c0);
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162803
// ==== 0x43f47ac  objc::Categories::ArcBrowserContext_Extensions_::favicon:iconTypes:sizeInPixels:fallbackToHost:completion:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     favicon_iconTypes_sizeInPixels_fallbackToHost_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,long_long param_5,bool param_6,
               ID param_7,undefined4 param_8)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  ID *local_78 [2];
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f49a4);
    (*pcVar2)();
  }
  FUN_043abdf8(local_a8,param_3);
  local_68 = FUN_043f8954;
  local_60 = &PTR_FUN_0df6ba28;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_7;
  FUN_042d6548(local_90,local_a8,(uint)param_4 & 0x1f,param_5,param_6,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_88[1];
    local_88[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar1 = local_88[2];
        local_88[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f48d8;
      }
      pcVar2 = *(code **)(*local_88 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar2)(local_88);
      }
    }
  }
LAB_043f48d8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162880
// ==== 0x43f4a54  objc::Categories::ArcBrowserContext_Extensions_::faviconTempFileForPage:iconTypes:sizeInPixels:fallbackToHost:completion:  (192 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     faviconTempFileForPage_iconTypes_sizeInPixels_fallbackToHost_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,long_long param_5,bool param_6,
               ID param_7,uint param_8)

{
  undefined *local_78;
  undefined8 uStack_70;
  undefined1 *local_68;
  undefined *puStack_60;
  ID local_58;
  
  local_78 = PTR___NSConcreteStackBlock_0d824120;
  uStack_70 = 0xc2000000;
  local_68 = &LAB_043f4b14;
  puStack_60 = &DAT_0df6b888;
  local_58 = param_7;
  _objc_msgSend(param_1,"favicon:iconTypes:sizeInPixels:fallbackToHost:completion:",param_3,param_4,
                param_5,(ulong)param_6,&local_78,(ulong)param_8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:162909
// ==== 0x43f4b48  objc::Categories::ArcBrowserContext_Extensions_::setFavicon:forPage:iconURL:iconType:completion:  (752 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setFavicon_forPage_iconURL_iconType_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               ,undefined4 param_8)

{
  long *plVar1;
  long lVar2;
  code *pcVar3;
  long *plVar4;
  undefined **local_c8;
  undefined8 uStack_c0;
  long *local_b8;
  undefined8 local_b0 [2];
  undefined7 local_a0;
  char cStack_99;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f4e34);
    (*pcVar3)();
  }
  FUN_043abdf8(local_98,param_4);
  FUN_043abdf8(local_b0,param_5);
  if (param_3 == 0) {
    local_c8 = (undefined **)0x0;
    uStack_c0 = 0;
    local_b8 = (long *)0x0;
  }
  else {
    _objc_msgSend(param_3,"image",&local_c8);
  }
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_7;
  local_58 = &LAB_04045100;
  local_50 = &PTR_FUN_0df6ba48;
  FUN_042d6b6c(local_80,local_98,local_b0,(uint)param_6 & 0x1f,&local_c8,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  plVar1 = local_b8;
  local_c8 = &PTR_FUN_0da7eed0;
  if (local_b8 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_b8[1];
    local_b8[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar4 = local_b8 + 2;
      (**(code **)(*local_b8 + 0x10))(local_b8);
      if (*plVar4 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f4c7c;
      }
      pcVar3 = *(code **)(*plVar1 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar3)(plVar1);
      }
    }
  }
LAB_043f4c7c:
  if (cStack_99 < '\0') {
    FUN_00005810(local_b0[0],CONCAT17(cStack_99,local_a0) & 0x7fffffffffffffff);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_78[1];
    local_78[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar2 = local_78[2];
        local_78[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043f4ccc;
      }
      pcVar3 = *(code **)(*local_78 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar3)(local_78);
      }
    }
  }
LAB_043f4ccc:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163033
// ==== 0x43f4fd8  objc::Categories::ArcBrowserContext_Extensions_::setFaviconData:forPage:iconURL:iconType:completion:  (372 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x38 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     setFaviconData_forPage_iconURL_iconType_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6,ID param_7
               ,undefined4 param_8)

{
  ID IVar1;
  ID IVar2;
  ID *local_88 [2];
  undefined1 *local_78;
  undefined **local_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  IVar1 = _objc_retainAutorelease(param_3);
  IVar1 = _objc_msgSend(IVar1,"bytes");
  IVar2 = _objc_msgSend(param_3,"length");
  local_78 = &LAB_043f89b4;
  local_70 = &PTR_FUN_0df6ba88;
  local_88[0] = (ID *)FUN_00008790(0x28);
  *local_88[0] = param_1;
  local_88[0][1] = param_4;
  local_88[0][2] = param_5;
  local_88[0][3] = param_6;
  local_88[0][4] = param_7;
  FUN_04338920(0x3f800000,IVar1,IVar2,local_88,0,0,0);
  if ((code *)local_70[1] != (code *)0x0) {
    (*(code *)local_70[1])(local_88[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163077
// ==== 0x43f5194  objc::Categories::ArcBrowserContext_Extensions_::getAdBlockDebugInfo:completion:  (476 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::getAdBlockDebugInfo_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_98 [2];
  undefined7 local_88;
  char cStack_81;
  undefined8 local_80;
  long *local_78;
  byte local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_80,param_3,param_4,(ulong)param_5);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f536c);
    (*pcVar2)();
  }
  FUN_043abdf8(local_98,param_3);
  local_58 = FUN_043f8ed0;
  local_50 = &PTR_FUN_0df6baa8;
  local_68[0] = (ID *)FUN_00008790(8);
  *local_68[0] = param_4;
  FUN_042d6edc(local_80,local_98,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if (cStack_81 < '\0') {
    FUN_00005810(local_98[0],CONCAT17(cStack_81,local_88) & 0x7fffffffffffffff);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f52a4;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043f52a4:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_48) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163152
// ==== 0x43f541c  objc::Categories::ArcBrowserContext_Extensions_::id  (360 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::id(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined4 local_38;
  
  IVar2 = _objc_alloc(&class_t::ArcBrowserContextID);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5584);
    (*pcVar3)();
  }
  if (*(char *)(local_68 + 0xa7) < '\0') {
    FUN_00060a60(&local_50,*(undefined8 *)(local_68 + 0x90),*(undefined8 *)(local_68 + 0x98));
  }
  else {
    uStack_48 = *(undefined8 *)(local_68 + 0x98);
    local_50 = *(undefined8 *)(local_68 + 0x90);
    local_40 = *(ulong *)(local_68 + 0xa0);
  }
  local_38 = *(undefined4 *)(local_68 + 0xa8);
  IVar2 = ArcBrowserContextID::initWithBrowserContextID_
                    (IVar2,"initWithBrowserContextID:",&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163221
// ==== 0x43f5648  objc::Categories::ArcBrowserContext_Extensions_::metrics  (512 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::metrics(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"metrics");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&class_t::ArcMetrics);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5848);
    (*pcVar3)();
  }
  FUN_04298600(auStack_50,*(undefined8 *)(local_68 + 0x120));
  IVar2 = ArcMetrics::initWithBrowserContextMetrics_
                    (IVar2,"initWithBrowserContextMetrics:",auStack_50);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f56ec;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_043f56ec:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f570c;
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
LAB_043f570c:
  _objc_setAssociatedObject(param_1,"metrics",IVar2,1);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163307
// ==== 0x43f598c  objc::Categories::ArcBrowserContext_Extensions_::extensions  (516 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::extensions(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"extensions");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&class_t::ArcBrowserContextExtensions);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5b90);
    (*pcVar3)();
  }
  FUN_042c8400(auStack_50,*(undefined8 *)(local_68 + 0x100));
  IVar2 = ArcBrowserContextExtensions::initWithBrowserContextExtensions_forBrowserContext_
                    (IVar2,"initWithBrowserContextExtensions:forBrowserContext:",auStack_50,param_1)
  ;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f5a34;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_043f5a34:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f5a54;
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
LAB_043f5a54:
  _objc_setAssociatedObject(param_1,"extensions",IVar2,1);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163394
// ==== 0x43f5cd4  objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed:withReason:  (448 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed_withReason_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  undefined8 uVar2;
  code *pcVar3;
  undefined8 local_78;
  undefined8 uStack_70;
  ulong local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5e94);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  if (param_4 == 0) {
    local_78 = 0;
    uStack_70 = 0;
    local_68 = 0;
  }
  else {
    FUN_01b356b8(&local_78,param_4);
  }
  if (*(long *)(local_48 + 0x88) != 0) {
    uVar2 = FUN_04135760(*(undefined8 *)(*(long *)(local_48 + 0x88) + 0x18));
    FUN_0416cc3c(uVar2,&local_60,&local_78);
  }
  if ((long)local_68 < 0) {
    FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
  }
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar3)(local_40);
      }
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:163478
// ==== 0x43f5f60  objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders:  (352 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::createAutocompleteControllerWithProviders_
             (ID param_1,SEL param_2,unsigned_long_long param_3)

{
  long lVar1;
  undefined8 uVar2;
  unique_ptr<arc::ArcAutocompleteController,_std::default_delete<arc::ArcAutocompleteController>>
  uVar3;
  ID IVar4;
  code *pcVar5;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(1,0x43f60c0);
    (*pcVar5)();
  }
  uVar2 = FUN_00008790(0x78);
  FUN_042c2f5c(uVar2,*(undefined8 *)(local_48 + 0x88),(uint)param_3 & 3);
  uVar3.field0_0x0 = (ArcAutocompleteController *)FUN_00008790(8);
  *(undefined8 *)uVar3.field0_0x0 = uVar2;
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f5fdc;
      }
      pcVar5 = *(code **)(*local_40 + 0x20);
      if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar5)(local_40);
      }
    }
  }
LAB_043f5fdc:
  IVar4 = _objc_alloc(&class_t::ArcBrowserContextAutocomplete);
  if (IVar4 == 0) {
    if (uVar3.field0_0x0 != (ArcAutocompleteController *)0x0) {
      lVar1 = *(long *)uVar3.field0_0x0;
      *(undefined8 *)uVar3.field0_0x0 = 0;
      if (lVar1 != 0) {
        FUN_042c33cc();
      }
      FUN_00005810(uVar3.field0_0x0,8);
    }
    IVar4 = 0;
  }
  else {
    IVar4 = ArcBrowserContextAutocomplete::initWithAutocompleteController_
                      (IVar4,"initWithAutocompleteController:",uVar3);
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x43.c:167865
// ==== 0x43fe3c0  ArcBrowserContextExtensions::.cxx_destruct  (360 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextExtensions::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x30) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x28), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_043fe3f4;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_043fe3f4:
  plVar3 = *(long **)(param_1 + 0x18);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043fe40c;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_043fe40c:
  _objc_destroyWeak(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:987
// ==== 0x4400f4c  ArcFreezingVoteToken::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcFreezingVoteToken::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x10) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:1201
// ==== 0x4401164  AXAnnouncementSpec::setWindow:  (12 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x18 bytes */

void AXAnnouncementSpec::setWindow_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x18) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:1532
// ==== 0x4401710  ArcHistoryQueryOptions::options  (32 bytes)

/* Function Stack Size: 0x10 bytes */

HistoryQueryOptions ArcHistoryQueryOptions::options(ID param_1,SEL param_2)

{
  long *plVar1;
  undefined8 *in_x8;
  long lVar2;
  undefined8 uVar3;
  HistoryQueryOptions HVar4;
  
  lVar2 = *(long *)(param_1 + 0x10);
  uVar3 = *(undefined8 *)(param_1 + 8);
  in_x8[1] = *(undefined8 *)(param_1 + 0x10);
  *in_x8 = uVar3;
  if (lVar2 != 0) {
    plVar1 = (long *)(lVar2 + 8);
    *plVar1 = *plVar1 + 1;
  }
  HVar4.field0_0x0.field1_0x8 = (__shared_weak_count *)param_2;
  HVar4.field0_0x0.field0_0x0 = (Impl *)param_1;
  return (HistoryQueryOptions)HVar4.field0_0x0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:1784
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



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:1894
// ==== 0x4401ca0  objc::Categories::ArcCommandBarRankingExample_Extensions_::input  (452 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcCommandBarRankingExample_Extensions_::input(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  ID self;
  ID IVar3;
  long lVar4;
  undefined4 *puVar5;
  long lVar6;
  long lVar7;
  undefined4 *local_90;
  ulong local_88;
  long local_80;
  long local_78;
  long local_70;
  long local_68;
  byte local_58;
  
  _objc_msgSend(param_1,"command_bar_ranking_example",&local_90);
  if ((local_58 & 1) != 0) {
    IVar2 = _objc_alloc(&class_t::ArcCommandBarRankingInput);
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_88);
    puVar5 = local_90;
    for (lVar7 = local_88 << 2; lVar7 != 0; lVar7 = lVar7 + -4) {
      IVar3 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar5);
      NSMutableArray::addObject_(self,"addObject:",IVar3);
      puVar5 = puVar5 + 1;
    }
    IVar2 = ArcCommandBarRankingInput::init_(IVar2,"init:",self);
    lVar7 = local_78;
    if (local_58 != 0) {
      if (local_78 != 0) {
        if (local_70 * 0xa8 != 0) {
          lVar4 = local_70 * 0xa8 + local_78;
          lVar6 = local_70 * -0xa8;
          do {
            if (lVar4 == 0xa8) goto LAB_04401e74;
            lVar4 = FUN_042a3ff4();
            lVar6 = lVar6 + 0xa8;
          } while (lVar6 != 0);
        }
        local_70 = (lVar7 - local_78 >> 3) * -0x30c30c30c30c30c3;
        FUN_00005810(local_78,local_68 * 0xa8);
      }
      if (local_90 != (undefined4 *)0x0) {
        if ((local_88 & 0x3fffffffffffffff) != 0) {
          lVar7 = local_88 * -4;
          puVar5 = local_90 + local_88;
          do {
            puVar5 = puVar5 + -1;
            if (puVar5 == (undefined4 *)0x0) goto LAB_04401e74;
            lVar7 = lVar7 + 4;
          } while (lVar7 != 0);
        }
        local_88 = 0;
        FUN_00005810(local_90,local_80 << 2);
      }
    }
    return IVar2;
  }
LAB_04401e74:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4401e78);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:1971
// ==== 0x4401f34  objc::Categories::ArcCommandBarRankingExample_Extensions_::candidates  (932 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcCommandBarRankingExample_Extensions_::candidates(ID param_1,SEL param_2)

{
  int iVar1;
  int iVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  char cVar6;
  code *pcVar7;
  undefined8 *puVar8;
  ID self;
  ID IVar9;
  ID IVar10;
  ID IVar11;
  ID IVar12;
  ID IVar13;
  ID IVar14;
  ID IVar15;
  ID self_00;
  long lVar16;
  long lVar17;
  undefined8 *puVar18;
  uint *puVar19;
  float fVar20;
  int in_stack_fffffffffffffec8;
  undefined1 uVar21;
  bool in_stack_fffffffffffffed8;
  char *pcVar22;
  char *op;
  long local_b8;
  long local_b0;
  long local_a8;
  undefined8 *local_a0;
  long local_98;
  long local_90;
  byte local_80;
  
  _objc_msgSend(param_1,"command_bar_ranking_example",&local_b8);
  puVar18 = local_a0;
  if ((local_80 & 1) == 0) {
LAB_044022d4:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x44022d8);
    (*pcVar7)();
  }
  if (local_a0 != (undefined8 *)0x0) {
    if (local_98 * 0xa8 != 0) {
      puVar8 = local_a0 + local_98 * 0x15;
      lVar17 = local_98 * -0xa8;
      do {
        if ((dword *)puVar8 == &section_00000068.flags) goto LAB_044022d4;
        puVar8 = (undefined8 *)FUN_042a3ff4();
        lVar17 = lVar17 + 0xa8;
      } while (lVar17 != 0);
    }
    local_98 = ((long)puVar18 - (long)local_a0 >> 3) * -0x30c30c30c30c30c3;
    FUN_00005810(local_a0,local_90 * 0xa8);
  }
  if (local_b8 != 0) {
    if (local_b0 * 4 != 0) {
      lVar17 = local_b0 * -4;
      lVar16 = local_b0 * 4 + local_b8;
      do {
        lVar16 = lVar16 + -4;
        if (lVar16 == 0) goto LAB_044022d4;
        lVar17 = lVar17 + 4;
      } while (lVar17 != 0);
    }
    local_b0 = 0;
    FUN_00005810(local_b8,local_a8 << 2);
  }
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_98);
  if (local_98 * 0xa8 != 0) {
    lVar17 = local_98 * 0x15;
    pcVar22 = 
    "init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding:"
    ;
    op = "addObject:";
    puVar18 = local_a0;
    do {
      IVar10 = _objc_alloc(&class_t::ArcCommandBarRankingCandidate);
      cVar6 = *(char *)((long)puVar18 + 0x17);
      puVar8 = (undefined8 *)*puVar18;
      if (-1 < (long)cVar6) {
        puVar8 = puVar18;
      }
      lVar16 = puVar18[1];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar11 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x2f);
      puVar8 = (undefined8 *)puVar18[3];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 3;
      }
      lVar16 = puVar18[4];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar12 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x47);
      puVar8 = (undefined8 *)puVar18[6];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 6;
      }
      lVar16 = puVar18[7];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar13 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x5f);
      puVar8 = (undefined8 *)puVar18[9];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 9;
      }
      lVar16 = puVar18[10];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar14 = FUN_01c391bc(puVar8,lVar16);
      cVar6 = *(char *)((long)puVar18 + 0x77);
      puVar8 = (undefined8 *)puVar18[0xc];
      if (-1 < (long)cVar6) {
        puVar8 = puVar18 + 0xc;
      }
      lVar16 = puVar18[0xd];
      if (-1 < cVar6) {
        lVar16 = (long)cVar6;
      }
      IVar15 = FUN_01c391bc(puVar8,lVar16);
      iVar1 = *(int *)(puVar18 + 0xf);
      iVar2 = *(int *)((long)puVar18 + 0x7c);
      fVar20 = *(float *)((long)puVar18 + 0x84);
      uVar3 = *(undefined1 *)(puVar18 + 0x11);
      uVar4 = *(undefined1 *)((long)puVar18 + 0x89);
      uVar5 = *(undefined1 *)((long)puVar18 + 0x8a);
      self_00 = NSMutableArray::arrayWithCapacity_
                          ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",puVar18[0x13]);
      uVar21 = (undefined1)(in_stack_fffffffffffffec8 >> 0x18);
      lVar16 = puVar18[0x13] << 2;
      if (lVar16 != 0) {
        puVar19 = (uint *)puVar18[0x12];
        do {
          IVar9 = NSNumber::numberWithFloat_
                            ((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",(ulong)*puVar19);
          NSMutableArray::addObject_(self_00,"addObject:",IVar9);
          uVar21 = (undefined1)(in_stack_fffffffffffffec8 >> 0x18);
          lVar16 = lVar16 + -4;
          puVar19 = puVar19 + 1;
        } while (lVar16 != 0);
      }
      in_stack_fffffffffffffec8 = CONCAT31(CONCAT21(CONCAT11(uVar21,uVar5),uVar4),uVar3);
      IVar10 = ArcCommandBarRankingCandidate::
               init_archetype_destination_title_url_lastNavSeconds_freqEver_typedCount_siteEngagement_isExactURLMatch_isBaseDomainMatch_isTitlePrefix_titleEmbedding_
                         (IVar10,pcVar22,IVar11,IVar12,IVar13,IVar14,IVar15,iVar1,iVar2,
                          in_stack_fffffffffffffec8,fVar20,(bool)self_00,in_stack_fffffffffffffed8,
                          (bool)pcVar22,(ID)op);
      NSMutableArray::addObject_(self,op,IVar10);
      puVar18 = puVar18 + 0x15;
    } while (puVar18 != local_a0 + lVar17);
  }
  return self;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3076
// ==== 0x44034ac  ArcAdhocHistogramSample::initWithType:value:  (80 bytes)

/* Function Stack Size: 0x1c bytes */

ID ArcAdhocHistogramSample::initWithType_value_
             (ID param_1,SEL param_2,unsigned_long_long param_3,int param_4)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAdhocHistogramSample;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(unsigned_long_long *)(IVar1 + 0x10) = param_3;
    *(int *)(IVar1 + 8) = param_4;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3099
// ==== 0x44034fc  ArcAdhocHistogramSample::setValue:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcAdhocHistogramSample::setValue_(ID param_1,SEL param_2,int param_3)

{
  *(int *)(param_1 + 8) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3156
// ==== 0x440360c  ArcHistoryQueryOptions::searchText  (144 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryQueryOptions::searchText(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  ID IVar3;
  undefined8 *puVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  puVar4 = *(undefined8 **)(param_1 + 8);
  if (*(char *)((long)puVar4 + 0x17) < '\0') {
    FUN_00054420(&local_40,*puVar4,puVar4[1]);
  }
  else {
    lStack_38 = puVar4[1];
    local_40 = (undefined1 *)*puVar4;
    local_30 = puVar4[2];
  }
  ppuVar1 = (undefined1 **)local_40;
  if (-1 < (long)local_30._7_1_) {
    ppuVar1 = &local_40;
  }
  lVar2 = lStack_38;
  if (-1 < local_30) {
    lVar2 = (long)local_30._7_1_;
  }
  IVar3 = FUN_01d9fb90(ppuVar1,lVar2);
  if (local_30 < 0) {
    FUN_00005810(local_40,local_30 << 1);
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3199
// ==== 0x44036c0  ArcHistoryQueryOptions::setSearchText:  (216 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setSearchText_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 **ppuVar1;
  undefined8 ***pppuVar2;
  undefined8 ***pppuVar3;
  undefined8 **local_38;
  undefined8 *puStack_30;
  undefined8 *local_28;
  
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    puStack_30 = (undefined8 **)0x0;
    local_28 = (undefined8 **)0x0;
  }
  else {
    FUN_083b1228(&local_38,param_3);
  }
  pppuVar3 = *(undefined8 ****)(param_1 + 8);
  if (pppuVar3 != &local_38) {
    if (*(char *)((long)pppuVar3 + 0x17) < '\0') {
      ppuVar1 = (undefined8 **)puStack_30;
      pppuVar2 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        ppuVar1 = (undefined8 **)((ulong)local_28 >> 0x38);
        pppuVar2 = &local_38;
      }
      FUN_007ab764(pppuVar3,pppuVar2,ppuVar1);
    }
    else if ((long)local_28 < 0) {
      FUN_007ab624(pppuVar3,local_38,puStack_30);
    }
    else {
      pppuVar3[2] = (undefined8 **)local_28;
      pppuVar3[1] = (undefined8 **)puStack_30;
      *pppuVar3 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,(long)local_28 << 1);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3250
// ==== 0x4403798  ArcHistoryQueryOptions::beginTime  (152 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcHistoryQueryOptions::beginTime(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  long lVar3;
  double dVar4;
  
  lVar3 = *(long *)(*(long *)(param_1 + 8) + 0x18);
  if (lVar3 == 0) {
    dVar4 = 0.0;
  }
  else if (lVar3 + 0x8000000000000001U < 2) {
    dVar4 = -INFINITY;
    if (-1 < lVar3) {
      dVar4 = INFINITY;
    }
  }
  else {
    lVar1 = -0x8000000000000000;
    if (!SCARRY8(lVar3,-0x295e9648864000)) {
      lVar1 = lVar3 + -0x295e9648864000;
    }
    if (lVar1 == -0x8000000000000000) {
      dVar4 = -INFINITY;
    }
    else {
      dVar4 = (double)lVar1 / 1000000.0;
    }
  }
  IVar2 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",dVar4);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3290
// ==== 0x4403830  ArcHistoryQueryOptions::setBeginTime:  (176 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setBeginTime_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  double extraout_d0;
  double dVar3;
  
  _objc_msgSend(param_3,"timeIntervalSince1970");
  lVar2 = 0x295e9648864000;
  if ((extraout_d0 != 0.0) && (lVar2 = 0, !NAN(extraout_d0))) {
    dVar3 = extraout_d0 * 1000000.0;
    lVar2 = 0;
    if (-9.223372036854776e+18 <= dVar3) {
      lVar2 = 0x7fffffffffffffff;
    }
    lVar1 = (long)dVar3;
    if (9.223372036854775e+18 < dVar3) {
      lVar1 = lVar2;
    }
    lVar2 = lVar1;
    if (1 < lVar1 + 0x8000000000000001U) {
      lVar2 = 0x7fffffffffffffff;
      if (!SCARRY8(lVar1,0x295e9648864000)) {
        lVar2 = lVar1 + 0x295e9648864000;
      }
    }
  }
  *(long *)(*(long *)(param_1 + 8) + 0x18) = lVar2;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3327
// ==== 0x44038e0  ArcHistoryQueryOptions::endTime  (152 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcHistoryQueryOptions::endTime(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  long lVar3;
  double dVar4;
  
  lVar3 = *(long *)(*(long *)(param_1 + 8) + 0x20);
  if (lVar3 == 0) {
    dVar4 = 0.0;
  }
  else if (lVar3 + 0x8000000000000001U < 2) {
    dVar4 = -INFINITY;
    if (-1 < lVar3) {
      dVar4 = INFINITY;
    }
  }
  else {
    lVar1 = -0x8000000000000000;
    if (!SCARRY8(lVar3,-0x295e9648864000)) {
      lVar1 = lVar3 + -0x295e9648864000;
    }
    if (lVar1 == -0x8000000000000000) {
      dVar4 = -INFINITY;
    }
    else {
      dVar4 = (double)lVar1 / 1000000.0;
    }
  }
  IVar2 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",dVar4);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3367
// ==== 0x4403978  ArcHistoryQueryOptions::setEndTime:  (176 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setEndTime_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  double extraout_d0;
  double dVar3;
  
  _objc_msgSend(param_3,"timeIntervalSince1970");
  lVar2 = 0x295e9648864000;
  if ((extraout_d0 != 0.0) && (lVar2 = 0, !NAN(extraout_d0))) {
    dVar3 = extraout_d0 * 1000000.0;
    lVar2 = 0;
    if (-9.223372036854776e+18 <= dVar3) {
      lVar2 = 0x7fffffffffffffff;
    }
    lVar1 = (long)dVar3;
    if (9.223372036854775e+18 < dVar3) {
      lVar1 = lVar2;
    }
    lVar2 = lVar1;
    if (1 < lVar1 + 0x8000000000000001U) {
      lVar2 = 0x7fffffffffffffff;
      if (!SCARRY8(lVar1,0x295e9648864000)) {
        lVar2 = lVar1 + 0x295e9648864000;
      }
    }
  }
  *(long *)(*(long *)(param_1 + 8) + 0x20) = lVar2;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3404
// ==== 0x4403a28  ArcHistoryQueryOptions::setMaxResults:  (12 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcHistoryQueryOptions::setMaxResults_(ID param_1,SEL param_2,int param_3)

{
  *(int *)(*(long *)(param_1 + 8) + 0x28) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3416
// ==== 0x4403a34  ArcHistoryQueryOptions::duplicateHandlingPolicy  (44 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcHistoryQueryOptions::duplicateHandlingPolicy(ID param_1,SEL param_2)

{
  long_long lVar1;
  uint uVar2;
  code *pcVar3;
  
  uVar2 = *(uint *)(*(long *)(param_1 + 8) + 0x30);
  if (uVar2 < 3) {
    lVar1 = 2;
    if (uVar2 != 2) {
      lVar1 = (ulong)(uVar2 == 1);
    }
    return lVar1;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(0x550b,0x4403a60);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3441
// ==== 0x4403a60  ArcHistoryQueryOptions::setDuplicateHandlingPolicy:  (20 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setDuplicateHandlingPolicy_(ID param_1,SEL param_2,long_long param_3)

{
  if (param_3 < 3) {
    *(int *)(*(long *)(param_1 + 8) + 0x30) = (int)param_3;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3455
// ==== 0x4403a74  ArcHistoryQueryOptions::matchingAlgorithm  (48 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcHistoryQueryOptions::matchingAlgorithm(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  long_long lVar3;
  
  if (*(char *)(*(long *)(param_1 + 8) + 0x38) == '\0') {
    return 0;
  }
  uVar1 = *(uint *)(*(long *)(param_1 + 8) + 0x34);
  lVar3 = (long_long)uVar1;
  if (uVar1 != 0) {
    if (uVar1 != 1) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x4403aa4);
      (*pcVar2)();
    }
    lVar3 = 1;
  }
  return lVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3483
// ==== 0x4403aa4  ArcHistoryQueryOptions::setMatchingAlgorithm:  (44 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcHistoryQueryOptions::setMatchingAlgorithm_(ID param_1,SEL param_2,long_long param_3)

{
  undefined4 uVar1;
  long lVar2;
  
  if (param_3 == 0) {
    uVar1 = 0;
  }
  else {
    if (param_3 != 1) {
      return;
    }
    uVar1 = 1;
  }
  lVar2 = *(long *)(param_1 + 8);
  *(undefined4 *)(lVar2 + 0x34) = uVar1;
  *(undefined1 *)(lVar2 + 0x38) = 1;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3509
// ==== 0x4403ad0  ArcHistoryQueryOptions::setOptions:  (12 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcHistoryQueryOptions::setOptions_(ID param_1,SEL param_2,HistoryQueryOptions param_3)

{
  FUN_04331e20(param_1 + 8,param_3.field0_0x0.field0_0x0,param_3.field0_0x0.field0_0x0,
               param_3.field0_0x0.field1_0x8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3522
// ==== 0x4403adc  ArcHistoryQueryOptions::.cxx_construct  (164 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcHistoryQueryOptions::_cxx_construct(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 *puVar3;
  
  puVar3 = (undefined8 *)FUN_00008790(0xb8);
  puVar3[1] = 0;
  puVar3[2] = 0;
  *puVar3 = &PTR_viewWillStartLiveResize_0da7ee40;
  puVar1 = puVar3 + 3;
  if (puVar1 != (undefined8 *)0x0) {
    *(undefined1 *)(puVar3 + 10) = 0;
    *(undefined1 *)(puVar3 + 0xf) = 0;
    *(undefined1 *)(puVar3 + 0x12) = 0;
    *(undefined1 *)(puVar3 + 0x13) = 0;
    puVar3[0xc] = 0;
    puVar3[0xd] = 0;
    puVar3[0xb] = 0;
    *(undefined4 *)(puVar3 + 0xe) = 0;
    puVar3[4] = 0;
    *puVar1 = 0;
    puVar3[6] = 0;
    puVar3[5] = 0;
    puVar3[8] = 0;
    puVar3[7] = 0;
    *(undefined8 *)((long)puVar3 + 0x45) = 0;
    *(undefined2 *)((long)puVar3 + 0x99) = 1;
    puVar3[0x15] = 0;
    puVar3[0x16] = 0;
    puVar3[0x14] = 0;
    FUN_0458d908(puVar3 + 6,1);
    *(undefined4 *)(puVar3 + 8) = 100;
    *(undefined8 **)(param_1 + 8) = puVar1;
    *(undefined8 **)(param_1 + 0x10) = puVar3;
    return param_1;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4403b80);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3771
// ==== 0x4403e9c  ArcImportSource::initWithImportSource:  (124 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcImportSource::initWithImportSource_(ID param_1,SEL param_2,const_ImportSource__ param_3)

{
  uint uVar1;
  code *pcVar2;
  ID IVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcImportSource;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    *(long *)(IVar3 + 8) = (long)(int)param_3->field0_0x0;
    uVar1 = param_3->field1_0x4 + 1;
    if ((0xe < uVar1) || ((0x7f1dU >> (ulong)(uVar1 & 0x1f) & 1) == 0)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0x550b,0x4403f18);
      (*pcVar2)();
    }
    *(ulong *)(IVar3 + 0x10) = (ulong)(byte)(&DAT_0c9d8158)[uVar1];
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3801
// ==== 0x4403f18  ArcImportSourceProfile::initWithImportSourceProfile:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcImportSourceProfile::initWithImportSourceProfile_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcImportSourceProfile;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_04403f78;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04403f74;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_04403f74:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_04403f78:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:3860
// ==== 0x440403c  ArcImportSourceProfile::initWithSafariExportedData:profileName:name:categories:  (1080 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

ID ArcImportSourceProfile::initWithSafariExportedData_profileName_name_categories_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,long_long param_6)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  undefined8 *****pppppuVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  undefined1 auStack_220 [136];
  undefined8 ****local_198;
  long lStack_190;
  undefined8 local_188;
  ulong local_180;
  undefined8 uStack_178;
  ulong local_170;
  undefined8 uStack_168;
  ulong local_160;
  undefined8 uStack_158;
  ulong local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  ulong local_138;
  undefined8 local_130;
  undefined8 ****local_128;
  undefined8 uStack_120;
  ulong local_118;
  ulong uStack_110;
  undefined8 local_108;
  ulong uStack_100;
  ID local_f8;
  class_t *pcStack_f0;
  ulong local_e8;
  undefined8 uStack_e0;
  ulong local_d8;
  
  pcStack_f0 = &objc::class_t::ArcImportSourceProfile;
  local_f8 = param_1;
  IVar3 = NSObject::init((ID)&local_f8,"init");
  if (IVar3 == 0) {
    return 0;
  }
  uStack_148 = 0;
  local_150 = 0;
  local_138 = 0;
  uStack_140 = 0;
  local_170 = 0;
  uStack_158 = 0;
  local_160 = 0;
  uStack_178 = 0;
  local_180 = 0;
  uStack_168 = 0xffffffff;
  uStack_120 = 0;
  local_128 = (undefined8 *****)0x0;
  uStack_110 = 0;
  local_118 = 0;
  uStack_100 = 0;
  local_108 = 0;
  local_130 = 0;
  if (param_5 == 0) {
    local_e8 = 0;
    uStack_e0 = 0;
    local_d8 = 0;
  }
  else {
    FUN_083b1228(&local_e8,param_5);
  }
  if ((long)local_170 < 0) {
    FUN_00005810(local_180,local_170 << 1);
  }
  uStack_178 = uStack_e0;
  local_180 = local_e8;
  local_170 = local_d8;
  if (param_4 == 0) {
    local_e8 = 0;
    uStack_e0 = 0;
    local_d8 = 0;
  }
  else {
    FUN_083b1228(&local_e8,param_4);
  }
  if ((long)uStack_100 < 0) {
    FUN_00005810(uStack_110,uStack_100 << 1);
  }
  local_108 = uStack_e0;
  uStack_110 = local_e8;
  uStack_100 = local_d8;
  uStack_168 = CONCAT44(uStack_168._4_4_,0xd);
  if (param_3 == 0) {
    local_198 = (undefined8 *****)0x0;
    lStack_190 = 0;
    local_188 = 0;
  }
  else {
    FUN_01b356b8(&local_198,param_3);
  }
  pppppuVar5 = (undefined8 *****)local_198;
  if (-1 < (long)local_188._7_1_) {
    pppppuVar5 = &local_198;
  }
  lVar2 = lStack_190;
  if (-1 < (long)local_188) {
    lVar2 = (long)local_188._7_1_;
  }
  FUN_008a7fc4(&local_e8,pppppuVar5,lVar2);
  if ((long)local_150 < 0) {
    FUN_00005810(local_160,local_150 & 0x7fffffffffffffff);
  }
  uStack_158 = uStack_e0;
  local_160 = local_e8;
  local_150 = local_d8;
  local_d8 = local_d8 & 0xffffffffffffff;
  local_e8 = local_e8 & 0xffffffffffffff00;
  if ((long)local_188 < 0) {
    FUN_00005810(local_198,local_188 & 0x7fffffffffffffff);
  }
  if ((long)local_138 < 0) {
    FUN_00005810(uStack_148,local_138 & 0x7fffffffffffffff);
  }
  uStack_148 = 0;
  uStack_140 = 0;
  local_138 = 0;
  local_130 = CONCAT62(local_130._2_6_,(short)param_6);
  if ((long)local_118 < 0) {
    uStack_120 = 0;
    pppppuVar5 = (undefined8 *****)local_128;
  }
  else {
    local_118 = local_118 & 0xffffffffffffff;
    pppppuVar5 = &local_128;
  }
  *(undefined1 *)pppppuVar5 = 0;
  FUN_07040958(auStack_220,&local_180);
  plVar4 = (long *)FUN_00008790(0xa0);
  plVar8 = plVar4 + 1;
  *plVar8 = 0;
  plVar4[2] = 0;
  *plVar4 = (long)&PTR_viewWillStartLiveResize_0da7ef98;
  plVar1 = plVar4 + 3;
  if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x4404474);
    (*pcVar6)();
  }
  FUN_07040958(&local_e8,auStack_220);
  FUN_07040958(plVar1,&local_e8);
  FUN_07040a20(&local_e8);
  FUN_07040a20(auStack_220);
  if (*(char *)(IVar3 + 0x18) != '\0') {
    plVar7 = *(long **)(IVar3 + 0x10);
    if (plVar7 != (long *)0x0) {
      LOAcquire();
      lVar2 = plVar7[1];
      plVar7[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar2 = plVar7[2];
          plVar7[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_0440434c;
        }
        pcVar6 = *(code **)(*plVar7 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar6)(plVar7);
        }
      }
    }
LAB_0440434c:
    *(undefined1 *)(IVar3 + 0x18) = 0;
  }
  *(long **)(IVar3 + 8) = plVar1;
  *(long **)(IVar3 + 0x10) = plVar4;
  if (plVar4 == (long *)0x0) {
    *(undefined1 *)(IVar3 + 0x18) = 1;
  }
  else {
    *plVar8 = *plVar8 + 1;
    *(undefined1 *)(IVar3 + 0x18) = 1;
    LOAcquire();
    lVar2 = *plVar8;
    *plVar8 = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar2 = plVar4[2];
        plVar4[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_04404360;
      }
      pcVar6 = *(code **)(*plVar4 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar6)(plVar4);
      }
    }
  }
LAB_04404360:
  FUN_07040a20(&local_180);
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4081
// ==== 0x44044b0  ArcImportSourceProfile::availableExtensions:  (200 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcImportSourceProfile::availableExtensions_
               (ID param_1,SEL param_2,ID param_3,undefined4 param_4)

{
  code *pcVar1;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4404574);
    (*pcVar1)();
  }
  local_48 = FUN_0440486c;
  local_40 = &PTR_FUN_0df6bb30;
  local_58[0] = (ID *)FUN_00008790(8,param_2,param_3,param_4);
  *local_58[0] = param_3;
  FUN_0433ae6c(param_1 + 8,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4120
// ==== 0x44045a8  ArcImportSourceProfile::type  (64 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::type(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x44045e4);
    (*pcVar2)();
  }
  uVar1 = *(int *)(*(long *)(param_1 + 8) + 0x18) - 1;
  if ((uVar1 < 0xd) && ((0x1fc7U >> (ulong)(uVar1 & 0x1f) & 1) != 0)) {
    return (ulong)(byte)(&DAT_0c9d8168)[uVar1];
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0,0x44045e8);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4145
// ==== 0x44045f0  ArcImportSourceProfile::sourcePath  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImportSourceProfile::sourcePath(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x37) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x20),*(undefined8 *)(lVar4 + 0x28));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x28);
      local_40 = *(undefined1 **)(lVar4 + 0x20);
      local_30 = *(ulong *)(lVar4 + 0x30);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x440468c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4193
// ==== 0x44046b0  ArcImportSourceProfile::appPath  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImportSourceProfile::appPath(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x4f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x38),*(undefined8 *)(lVar4 + 0x40));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x40);
      local_40 = *(undefined1 **)(lVar4 + 0x38);
      local_30 = *(ulong *)(lVar4 + 0x48);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x440474c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4241
// ==== 0x4404770  ArcImportSourceProfile::supportedCategories  (28 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::supportedCategories(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (ulong)*(ushort *)(*(long *)(param_1 + 8) + 0x50) & 0xfff;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440478c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4259
// ==== 0x440478c  ArcImportSourceProfile::profileName  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImportSourceProfile::profileName(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x87) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x70),*(undefined8 *)(lVar4 + 0x78));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x78);
      local_40 = *(undefined1 **)(lVar4 + 0x70);
      local_30 = *(long *)(lVar4 + 0x80);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4404828);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4307
// ==== 0x440484c  ArcImportSourceProfile::constraints  (32 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImportSourceProfile::constraints(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (ulong)(*(int *)(*(long *)(param_1 + 8) + 0x18) == 3);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440486c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4584
// ==== 0x4404fd0  ArcImageRep::decodeFromData:scale:completion:  (28 bytes)

/* Function Stack Size: 0x24 bytes */

void ArcImageRep::decodeFromData_scale_completion_
               (ID param_1,SEL param_2,ID param_3,float param_4,ID param_5,undefined4 param_6)

{
  undefined4 in_w7;
  
  decodeFromData_scale_desiredWidth_desiredHeight_maxDecodedBytes_completion_
            (param_1,"decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion:",
             param_3,param_4,0,0,0,param_5,in_w7);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4656
// ==== 0x440505c  ArcImageRep::setImageRep:  (12 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcImageRep::setImageRep_(ID param_1,SEL param_2,ImageRep *param_3)

{
  FUN_043380d4(param_1 + 8,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4809
// ==== 0x440539c  ArcImageRep::init  (256 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::init(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_58 [16];
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcImageRep;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    FUN_04337780(auStack_58);
    FUN_043381c0(IVar2 + 8,auStack_58);
    if (local_48 != (long *)0x0) {
      LOAcquire();
      lVar1 = local_48[1];
      local_48[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*local_48 + 0x10))(local_48);
        if (local_48[2] != 0) {
          LOAcquire();
          lVar1 = local_48[2];
          local_48[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) {
            return IVar2;
          }
        }
        pcVar3 = *(code **)(*local_48 + 0x20);
        if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
          FUN_00005810(local_48,0x20);
        }
        else {
          (*pcVar3)(local_48);
        }
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4860
// ==== 0x440549c  ArcImageRep::initWithArcImageRep:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcImageRep::initWithArcImageRep_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcImageRep;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_043380d4(IVar1 + 8,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4881
// ==== 0x44054f4  ArcImageRep::createFromPixelDataBGRA8888:height:scale:rowBytes:pixelData:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x38 bytes */

ID ArcImageRep::createFromPixelDataBGRA8888_height_scale_rowBytes_pixelData_
             (ID param_1,SEL param_2,long_long param_3,long_long param_4,double param_5,
             unsigned_long_long param_6,ID param_7)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined **local_58;
  undefined8 uStack_50;
  long *local_48;
  
  local_58 = (undefined **)0x0;
  uStack_50 = 0;
  local_48 = (long *)0x0;
  IVar3 = _objc_retainAutorelease(param_7);
  IVar3 = _objc_msgSend(IVar3,"bytes");
  FUN_043378d8(&local_58,(float)param_5,param_3,param_4,param_6,IVar3);
  IVar3 = _objc_alloc(&objc::class_t::ArcImageRep);
  IVar3 = initWithArcImageRep_(IVar3,"initWithArcImageRep:",&local_58);
  plVar1 = local_48;
  local_58 = &PTR_FUN_0da7eeb0;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_48[1];
    local_48[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_48 + 2;
      (**(code **)(*local_48 + 0x10))(local_48);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:4942
// ==== 0x4405700  ArcImageRep::dangerouslyDecodeFromData:scale:  (376 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcImageRep::dangerouslyDecodeFromData_scale_(ID param_1,SEL param_2,ID param_3,float param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  ID IVar4;
  code *pcVar5;
  long *plVar6;
  undefined4 in_register_00005004;
  undefined **local_60;
  undefined8 uStack_58;
  long *local_50;
  ulong local_48;
  
  uStack_58 = 0;
  local_60 = (undefined **)0x0;
  local_48 = 0;
  local_50 = (long *)0x0;
  IVar3 = _objc_retainAutorelease(param_3);
  IVar3 = _objc_msgSend(IVar3,"bytes");
  IVar4 = _objc_msgSend(param_3,"length");
  FUN_04337b04(&local_60,CONCAT44(in_register_00005004,param_4),IVar3,IVar4);
  if ((char)local_48 == '\0') {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&objc::class_t::ArcImageRep);
    if ((local_48 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    IVar3 = initWithArcImageRep_(IVar3,"initWithArcImageRep:",&local_60);
    plVar1 = local_50;
    if (((char)local_48 != '\0') && (local_60 = &PTR_FUN_0da7eeb0, local_50 != (long *)0x0)) {
      LOAcquire();
      lVar2 = local_50[1];
      local_50[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        plVar6 = local_50 + 2;
        (**(code **)(*local_50 + 0x10))(local_50);
        if (*plVar6 != 0) {
          LOAcquire();
          lVar2 = plVar1[2];
          plVar1[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) {
            return IVar3;
          }
        }
        pcVar5 = *(code **)(*plVar1 + 0x20);
        if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
          FUN_00005810(plVar1,0x20);
        }
        else {
          (*pcVar5)(plVar1);
        }
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5014
// ==== 0x4405930  ArcImageRep::decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion:  (308 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x3c bytes */

void ArcImageRep::decodeFromData_scale_desiredWidth_desiredHeight_maxDecodedBytes_completion_
               (ID param_1,SEL param_2,ID param_3,float param_4,long_long param_5,long_long param_6,
               unsigned_long_long param_7,ID param_8,undefined4 param_9)

{
  ID IVar1;
  ID IVar2;
  undefined4 in_register_00005004;
  ID *local_88 [2];
  code *local_78;
  undefined **local_70;
  long local_68;
  
  local_68 = *(long *)PTR____stack_chk_guard_0d824020;
  IVar1 = _objc_retainAutorelease(param_3);
  IVar1 = _objc_msgSend(IVar1,"bytes");
  IVar2 = _objc_msgSend(param_3,"length");
  local_78 = FUN_04405db4;
  local_70 = &PTR_FUN_0df6bb70;
  local_88[0] = (ID *)FUN_00008790(8);
  *local_88[0] = param_8;
  FUN_04337e00(CONCAT44(in_register_00005004,param_4),IVar1,IVar2,local_88,param_5,param_6,param_7);
  if ((code *)local_70[1] != (code *)0x0) {
    (*(code *)local_70[1])(local_88[0]);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5054
// ==== 0x4405a94  ArcImageRep::isNull  (44 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcImageRep::isNull(ID param_1,SEL param_2)

{
  uint uVar1;
  long lVar2;
  
  lVar2 = *(long *)(param_1 + 0x10);
  if (*(int *)(lVar2 + 0x1c0) == 0) {
    uVar1 = (uint)(*(long *)(lVar2 + 0x1d0) == 0);
  }
  else {
    uVar1 = *(byte *)(lVar2 + 0x1b8) ^ 1;
  }
  return uVar1 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5075
// ==== 0x4405ac0  ArcImageRep::pixelWidth  (12 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImageRep::pixelWidth(ID param_1,SEL param_2)

{
  return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x1c4);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5086
// ==== 0x4405acc  ArcImageRep::pixelHeight  (12 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcImageRep::pixelHeight(ID param_1,SEL param_2)

{
  return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x1c8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5097
// ==== 0x4405ad8  ArcImageRep::copyPixelDataBGRA8888  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::copyPixelDataBGRA8888(ID param_1,SEL param_2)

{
  int iVar1;
  ID IVar2;
  ID IVar3;
  
  IVar2 = NSMutableData::dataWithLength_
                    ((ID)&_OBJC_CLASS___NSMutableData,"dataWithLength:",
                     (long)(*(int *)(*(long *)(param_1 + 0x10) + 0x1c4) *
                            *(int *)(*(long *)(param_1 + 0x10) + 0x1c8) * 4));
  if (IVar2 == 0) {
    IVar2 = 0;
  }
  else {
    IVar3 = _objc_retainAutorelease();
    IVar3 = _objc_msgSend(IVar3,"mutableBytes");
    iVar1 = FUN_04338370(param_1 + 8,IVar3);
    if (iVar1 == 0) {
      IVar2 = 0;
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5130
// ==== 0x4405b64  ArcImageRep::toNSImage  (68 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainAutoreleasedReturnValue replaced with injection:
   _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::toNSImage(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  
  lVar1 = *(long *)(param_1 + 0x10);
  if (*(int *)(lVar1 + 0x1c0) == 0) {
    if (*(long *)(lVar1 + 0x1d0) != 0) goto LAB_04405b88;
  }
  else if (*(char *)(lVar1 + 0x1b8) != '\0') {
LAB_04405b88:
    FUN_022f4af4();
    IVar2 = FUN_085eac30();
    return IVar2;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5157
// ==== 0x4405ba8  ArcImageRep::encodeToPNGWithMethod:  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcImageRep::encodeToPNGWithMethod_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  ID IVar2;
  undefined8 uVar3;
  long local_40;
  ulong uStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    uVar3 = 0;
  }
  else {
    if (param_3 != 1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4405c5c);
      (*pcVar1)();
    }
    uVar3 = 1;
  }
  uStack_38 = 0;
  local_40 = 0;
  local_28 = 0;
  local_30 = 0;
  FUN_043385c0(&local_40,param_1 + 8,uVar3);
  if ((char)local_28 == '\0') {
    IVar2 = 0;
  }
  else {
    IVar2 = NSData::dataWithBytes_length_
                      ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",local_40,uStack_38);
    if (((char)local_28 != '\0') && (local_40 != 0)) {
      if ((ulong)-local_40 < uStack_38) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x4405c30);
        (*pcVar1)();
      }
      uStack_38 = 0;
      FUN_00005810(local_40,local_30);
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5211
// ==== 0x4405c98  ArcImageRep::imageRep  (44 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcImageRep::imageRep(ImageRep *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ImageRepImpl *pIVar3;
  
  __return_storage_ptr__->field0_0x0 = (undefined4 **)&PTR_FUN_0da7eeb0;
  lVar2 = *(long *)(param_1 + 0x18);
  pIVar3 = *(ImageRepImpl **)(param_1 + 0x10);
  (__return_storage_ptr__->field1_0x8).field1_0x8 = *(__shared_weak_count **)(param_1 + 0x18);
  (__return_storage_ptr__->field1_0x8).field0_0x0 = pIVar3;
  if (lVar2 != 0) {
    plVar1 = (long *)(lVar2 + 8);
    *plVar1 = *plVar1 + 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5235
// ==== 0x4405cc4  ArcImageRep::.cxx_destruct  (200 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcImageRep::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar2;
  
  *(undefined ***)(param_1 + 8) = &PTR_FUN_0da7eeb0;
  plVar2 = *(long **)(param_1 + 0x18);
  if (plVar2 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar2[1];
    plVar2[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar2 + 0x10))(plVar2);
      if (plVar2[2] != 0) {
        LOAcquire();
        lVar1 = plVar2[2];
        plVar2[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
      if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
         (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x04405d88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*UNRECOVERED_JUMPTABLE)(plVar2);
        return;
      }
      FUN_00005810(plVar2,0x20);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:5280
// ==== 0x4405d8c  ArcImageRep::.cxx_construct  (40 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcImageRep::_cxx_construct(ID param_1,SEL param_2)

{
  FUN_04337780(param_1 + 8);
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7476
// ==== 0x4408ac4  RTCVideoEncoderSettings::setName:  (12 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x18 bytes */

void RTCVideoEncoderSettings::setName_(ID param_1,SEL param_2,ID param_3)

{
  *(ID *)(param_1 + 0x20) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7489
// ==== 0x4408ad0  ArcNotificationButton::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcNotificationButton::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7504
// ==== 0x4408b0c  ArcNotification::initWithNotification:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcNotification::initWithNotification_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcNotification;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_04408b6c;
  plVar4 = *(long **)(IVar2 + 0x18);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04408b68;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_04408b68:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_04408b6c:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7563
// ==== 0x4408c30  ArcNotification::type  (48 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcNotification::type(ID param_1,SEL param_2)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4408c5c);
    (*pcVar2)();
  }
  uVar1 = *(uint *)(*(long *)(param_1 + 0x10) + 8);
  if (uVar1 < 7) {
    return (ulong)(byte)(&DAT_0c9d8178)[uVar1];
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(0,0x4408c60);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7588
// ==== 0x4408c68  ArcNotification::priority  (52 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcNotification::priority(ID param_1,SEL param_2)

{
  uint uVar1;
  long_long lVar2;
  int iVar3;
  code *pcVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4408c98);
    (*pcVar4)();
  }
  iVar3 = *(int *)(*(long *)(param_1 + 0x10) + 0x220);
  if (iVar3 + 2U < 6) {
    uVar1 = iVar3 + 1;
    lVar2 = 0;
    if (uVar1 < 5) {
      lVar2 = (ulong)uVar1 + 1;
    }
    return lVar2;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(0,0x4408c9c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7620
// ==== 0x4408ca4  ArcNotification::title  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::title(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x10);
    if (*(char *)(lVar4 + 0x3f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x28),*(undefined8 *)(lVar4 + 0x30));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x30);
      local_40 = *(undefined1 **)(lVar4 + 0x28);
      local_30 = *(long *)(lVar4 + 0x38);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4408d40);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7668
// ==== 0x4408d64  ArcNotification::browserContextQualifiedID  (132 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::browserContextQualifiedID(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e240(&local_38,*(undefined8 *)(*(long *)(param_1 + 0x10) + 0x368),
                 *(long *)(param_1 + 0x10) + 0x10);
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4408de8);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7708
// ==== 0x4408e0c  ArcNotification::browserContextLocalID  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::browserContextLocalID(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x10);
    if (*(char *)(lVar4 + 0x27) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x10),*(undefined8 *)(lVar4 + 0x18));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x18);
      local_40 = *(undefined1 **)(lVar4 + 0x10);
      local_30 = *(ulong *)(lVar4 + 0x20);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4408ea8);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7756
// ==== 0x4408ecc  ArcNotification::message  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::message(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x10);
    if (*(char *)(lVar4 + 0x57) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x40),*(undefined8 *)(lVar4 + 0x48));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x48);
      local_40 = *(undefined1 **)(lVar4 + 0x40);
      local_30 = *(long *)(lVar4 + 0x50);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4408f68);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7804
// ==== 0x4408f8c  ArcNotification::contextMessage  (124 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::contextMessage(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e440(&local_38,*(undefined8 *)(param_1 + 0x10));
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01d9fb90(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4409008);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7843
// ==== 0x440902c  ArcNotification::progressStatus  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::progressStatus(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x10);
    if (*(char *)(lVar4 + 0x2a7) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x290),*(undefined8 *)(lVar4 + 0x298));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x298);
      local_40 = *(undefined1 **)(lVar4 + 0x290);
      local_30 = *(long *)(lVar4 + 0x2a0);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x44090d0);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7891
// ==== 0x44090f4  ArcNotification::origin  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::origin(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  ID IVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    puVar2 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x10) + 0x98);
    if (*(char *)((long)puVar2 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar2,puVar2[1]);
    }
    else {
      uStack_38 = puVar2[1];
      local_40 = *puVar2;
      local_30 = puVar2[2];
    }
    IVar3 = FUN_043abd34(&local_40);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4409180);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7930
// ==== 0x44091a4  ArcNotification::documentURL  (172 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::documentURL(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong *puVar2;
  ID IVar3;
  SEL extraout_x1;
  long lVar4;
  undefined1 auVar5 [16];
  ulong local_40;
  ulong uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar4 = *(long *)(*(long *)(param_1 + 0x10) + 0x358);
    if ((lVar4 == 0) || (*(int *)(lVar4 + 8) != 1)) {
      local_30 = local_30 & 0xffffffffffffff;
      local_40 = local_40 & 0xffffffffffffff00;
    }
    else {
      auVar5 = FUN_007cf864(lVar4 + 0x10);
      param_2 = auVar5._8_8_;
      puVar2 = auVar5._0_8_;
      if (*(char *)((long)puVar2 + 0x17) < '\0') {
        FUN_00060a60(&local_40,*puVar2,puVar2[1]);
        param_2 = extraout_x1;
      }
      else {
        uStack_38 = puVar2[1];
        local_40 = *puVar2;
        local_30 = puVar2[2];
      }
    }
    IVar3 = FUN_043abd34(&local_40,param_2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4409250);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:7982
// ==== 0x4409274  ArcNotification::progress  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcNotification::progress(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 0x10) + 0x288);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440928c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8000
// ==== 0x440928c  ArcNotification::renotify  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::renotify(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x2e0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44092a4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8018
// ==== 0x44092a4  ArcNotification::permanent  (48 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::permanent(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44092d4);
    (*pcVar1)();
  }
  if ((*(byte *)(*(long *)(param_1 + 0x10) + 0x224) & 1) != 0) {
    return 1;
  }
  return (bool)(*(int *)(*(long *)(param_1 + 0x10) + 0x360) == 0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8039
// ==== 0x44092d4  ArcNotification::browserContext  (268 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::browserContext(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_40 [8];
  long *local_38;
  
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContext);
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44093e0);
    (*pcVar3)();
  }
  FUN_0433eb60(auStack_40,*(undefined8 *)(param_1 + 0x10));
  IVar2 = ArcBrowserContext::initWithBrowserContext_(IVar2,"initWithBrowserContext:",auStack_40);
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8090
// ==== 0x4409484  ArcNotification::showSnoozeButton  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::showSnoozeButton(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x314);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440949c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8108
// ==== 0x440949c  ArcNotification::showSettingsButton  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNotification::showSettingsButton(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)(*(int *)(*(long *)(param_1 + 0x10) + 0x310) != 0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44094bc);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8126
// ==== 0x44094bc  ArcNotification::items  (432 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::items(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  long lVar3;
  char cVar4;
  code *pcVar5;
  ID IVar6;
  ID IVar7;
  ID IVar8;
  long lVar9;
  undefined8 *local_80;
  long local_78;
  undefined1 *local_68;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e630(&local_80,*(undefined8 *)(param_1 + 0x10));
    lVar9 = local_78;
    local_68 = (undefined1 *)&local_80;
    FUN_0440a4e8(&local_68);
    if (lVar9 == 0) {
      IVar6 = 0;
    }
    else {
      IVar6 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
      if ((*(byte *)(param_1 + 0x20) & 1) == 0) goto LAB_04409668;
      FUN_0433e630(&local_80,*(undefined8 *)(param_1 + 0x10));
      puVar1 = local_80;
      for (lVar9 = local_78 * 0x30; lVar9 != 0; lVar9 = lVar9 + -0x30) {
        IVar7 = _objc_alloc(&objc::class_t::ArcNotificationItem);
        cVar4 = *(char *)((long)puVar1 + 0x2f);
        puVar2 = (undefined8 *)puVar1[3];
        if (-1 < (long)cVar4) {
          puVar2 = puVar1 + 3;
        }
        lVar3 = puVar1[4];
        if (-1 < cVar4) {
          lVar3 = (long)cVar4;
        }
        IVar8 = FUN_01d9fb90(puVar2,lVar3);
        AXAnnouncementSpec::setAnnouncement_(IVar7,"setMessage:",IVar8);
        cVar4 = *(char *)((long)puVar1 + 0x17);
        puVar2 = (undefined8 *)*puVar1;
        if (-1 < (long)cVar4) {
          puVar2 = puVar1;
        }
        lVar3 = puVar1[1];
        if (-1 < cVar4) {
          lVar3 = (long)cVar4;
        }
        IVar8 = FUN_01d9fb90(puVar2,lVar3);
        ICloudKeychainPresentationDelegate::setWindow_(IVar7,"setTitle:",IVar8);
        NSMutableArray::addObject_(IVar6,"addObject:",IVar7);
        puVar1 = puVar1 + 6;
      }
      local_68 = (undefined1 *)&local_80;
      FUN_0440a4e8(&local_68);
      IVar6 = NSMutableArray::copy(IVar6,"copy");
    }
    return IVar6;
  }
LAB_04409668:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x440966c);
  (*pcVar5)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8203
// ==== 0x4409688  ArcNotification::buttons  (736 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::buttons(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  char cVar2;
  code *pcVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  ID IVar7;
  undefined8 *puVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long local_78;
  long local_70;
  long local_68;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e888(&local_78,*(undefined8 *)(param_1 + 0x10));
    lVar9 = local_70;
    lVar11 = local_78;
    if (local_78 != 0) {
      if (local_70 * 0x50 != 0) {
        lVar4 = local_70 * 0x50 + local_78;
        lVar10 = local_70 * -0x50;
        do {
          if (lVar4 == 0x50) goto LAB_04409978;
          lVar4 = FUN_0433e338();
          lVar10 = lVar10 + 0x50;
        } while (lVar10 != 0);
      }
      local_70 = (lVar11 - local_78 >> 4) * -0x3333333333333333;
      FUN_00005810(local_78,local_68 * 0x50);
    }
    if (lVar9 == 0) {
      IVar5 = 0;
    }
    else {
      IVar5 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
      if ((*(byte *)(param_1 + 0x20) & 1) == 0) goto LAB_04409978;
      FUN_0433e888(&local_78,*(undefined8 *)(param_1 + 0x10));
      lVar11 = local_70 * 0x50;
      if (lVar11 != 0) {
        puVar8 = (undefined8 *)(local_78 + 0x30);
        do {
          IVar6 = _objc_alloc(&objc::class_t::ArcNotificationButton);
          cVar2 = *(char *)((long)puVar8 + -0x19);
          puVar1 = (undefined8 *)puVar8[-6];
          if (-1 < (long)cVar2) {
            puVar1 = puVar8 + -6;
          }
          lVar9 = puVar8[-5];
          if (-1 < cVar2) {
            lVar9 = (long)cVar2;
          }
          IVar7 = FUN_01d9fb90(puVar1,lVar9);
          AXAnnouncementSpec::setAnnouncement_(IVar6,"setTitle:",IVar7);
          if ((*(long *)puVar8[-2] == 0) || (*(long *)(*(long *)puVar8[-2] + 0x18) == 0)) {
            IVar7 = 0;
          }
          else {
            IVar7 = FUN_02a02ccc();
          }
          AXAnnouncementSpec::setWindow_(IVar6,"setIcon:",IVar7);
          if (*(char *)(puVar8 + 3) == '\0') {
            IVar7 = 0;
          }
          else {
            cVar2 = *(char *)((long)puVar8 + 0x17);
            puVar1 = (undefined8 *)*puVar8;
            if (-1 < (long)cVar2) {
              puVar1 = puVar8;
            }
            lVar9 = puVar8[1];
            if (-1 < cVar2) {
              lVar9 = (long)cVar2;
            }
            IVar7 = FUN_01d9fb90(puVar1,lVar9);
          }
          RTCVideoEncoderSettings::setName_(IVar6,"setPlaceholder:",IVar7);
          AXAnnouncementSpec::setPolite_(IVar6,"setIsTextInput:",(uint)*(byte *)(puVar8 + 3));
          NSMutableArray::addObject_(IVar5,"addObject:",IVar6);
          puVar8 = puVar8 + 10;
          lVar11 = lVar11 + -0x50;
        } while (lVar11 != 0);
      }
      lVar11 = local_78;
      if (local_78 != 0) {
        if (local_70 * 0x50 != 0) {
          lVar10 = local_70 * 0x50 + local_78;
          lVar9 = local_70 * -0x50;
          do {
            if (lVar10 == 0x50) goto LAB_04409978;
            lVar10 = FUN_0433e338();
            lVar9 = lVar9 + 0x50;
          } while (lVar9 != 0);
        }
        local_70 = (lVar11 - local_78 >> 4) * -0x3333333333333333;
        FUN_00005810(local_78,local_68 * 0x50);
      }
      IVar5 = NSMutableArray::copy(IVar5,"copy");
    }
    return IVar5;
  }
LAB_04409978:
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x440997c);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8324
// ==== 0x44099e8  ArcNotification::icon  (280 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::icon(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined **local_48;
  long *local_40;
  long *local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4409b00);
    (*pcVar3)();
  }
  FUN_0433e540(&local_48,param_1 + 0x10);
  if ((*local_40 == 0) || (*(long *)(*local_40 + 0x18) == 0)) {
    IVar2 = 0;
  }
  else {
    IVar2 = FUN_02a02ccc();
  }
  local_48 = &PTR_FUN_0da7eed0;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8382
// ==== 0x4409b00  ArcNotification::image  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::image(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined **local_48;
  long *local_40;
  long *local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4409c1c);
    (*pcVar3)();
  }
  FUN_04338dd8(&local_48,*(long *)(param_1 + 0x10) + 0x248);
  if ((*local_40 == 0) || (*(long *)(*local_40 + 0x18) == 0)) {
    IVar2 = 0;
  }
  else {
    IVar2 = FUN_02a02ccc();
  }
  local_48 = &PTR_FUN_0da7eed0;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8440
// ==== 0x4409c1c  ArcNotification::smallImage  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::smallImage(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined **local_48;
  long *local_40;
  long *local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4409d38);
    (*pcVar3)();
  }
  FUN_04338dd8(&local_48,*(long *)(param_1 + 0x10) + 0x250);
  if ((*local_40 == 0) || (*(long *)(*local_40 + 0x18) == 0)) {
    IVar2 = 0;
  }
  else {
    IVar2 = FUN_02a02ccc();
  }
  local_48 = &PTR_FUN_0da7eed0;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8498
// ==== 0x4409d38  ArcNotification::category  (740 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcNotification::category(ID param_1,SEL param_2)

{
  ulong uVar1;
  undefined8 **ppuVar2;
  undefined8 *puVar3;
  long lVar4;
  ulong uVar5;
  code *pcVar6;
  ID IVar7;
  ulong uVar8;
  long lVar9;
  long lVar10;
  undefined8 **ppuVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  long local_b0;
  long local_a8;
  long local_a0;
  undefined8 **local_98;
  ulong local_90;
  ulong local_88;
  undefined8 **local_80;
  undefined8 **local_78;
  ulong local_70;
  ulong local_68;
  
  local_98 = (undefined8 **)0x0;
  local_90 = 0;
  local_88 = 0;
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433e888(&local_b0,*(undefined8 *)(param_1 + 0x10));
    lVar4 = local_b0;
    lVar9 = local_a8 * 0x50;
    if (lVar9 != 0) {
      lVar10 = 0;
      do {
        uVar5 = local_90;
        ppuVar11 = local_98;
        puVar3 = (undefined8 *)(lVar4 + lVar10);
        if (local_90 == local_88) {
          uVar1 = local_90 + 1;
          if (0x555555555555555 < uVar1) {
                    /* WARNING: Subroutine does not return */
            FUN_04047380();
          }
          uVar8 = local_88 * 2;
          if (uVar8 < uVar1 || uVar8 - uVar1 == 0) {
            uVar8 = uVar1;
          }
          if (0x2aaaaaaaaaaaaa9 < local_88) {
            uVar8 = 0x555555555555555;
          }
          local_80 = (undefined8 **)0x0;
          if (uVar8 != 0) {
            if (0x555555555555555 < uVar8) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            local_80 = (undefined8 **)FUN_00008790(uVar8 * 0x30);
          }
          ppuVar11 = local_80 + uVar5 * 6;
          local_70 = 0;
          local_78 = ppuVar11;
          local_68 = uVar8;
          if (ppuVar11 == (undefined8 **)0x0) goto LAB_04409e98;
          puVar12 = (undefined8 *)(lVar4 + lVar10);
          if (*(char *)((long)puVar12 + 0x17) < '\0') {
            FUN_00054420(ppuVar11,*puVar12,puVar12[1]);
          }
          else {
            puVar13 = (undefined8 *)puVar3[1];
            puVar12 = (undefined8 *)*puVar3;
            ppuVar11[2] = (undefined8 *)puVar3[2];
            ppuVar11[1] = puVar13;
            *ppuVar11 = puVar12;
          }
          if (*(char *)(lVar4 + lVar10 + 0x47) < '\0') {
            FUN_00054420(ppuVar11 + 3,puVar3[6],*(undefined8 *)(lVar4 + lVar10 + 0x38));
          }
          else {
            puVar13 = (undefined8 *)puVar3[7];
            puVar12 = (undefined8 *)puVar3[6];
            ppuVar11[5] = (undefined8 *)puVar3[8];
            ppuVar11[4] = puVar13;
            ppuVar11[3] = puVar12;
          }
          ppuVar11 = ppuVar11 + 6;
          ppuVar2 = local_78 + local_90 * -6;
          _memcpy(ppuVar2,local_98,local_90 * 0x30);
          uVar5 = local_68;
          local_90 = ((long)ppuVar11 - (long)ppuVar2 >> 4) * -0x5555555555555555;
          local_70 = 0;
          local_68 = local_88;
          local_88 = uVar5;
          local_80 = local_98;
          local_78 = local_98;
          local_98 = ppuVar2;
          FUN_0577fc24(&local_80);
        }
        else {
          FUN_0440a5f0(&local_98,lVar4 + lVar10,puVar3 + 6);
          ppuVar11 = ppuVar11 + uVar5 * 6 + 6;
        }
        local_90 = ((long)ppuVar11 - (long)local_98 >> 4) * -0x5555555555555555;
        if ((long)ppuVar11 - (long)local_98 == 0) goto LAB_04409e98;
        lVar10 = lVar10 + 0x50;
      } while (lVar9 - lVar10 != 0);
    }
    lVar4 = local_b0;
    if (local_b0 != 0) {
      if (local_a8 * 0x50 != 0) {
        lVar10 = local_a8 * 0x50 + local_b0;
        lVar9 = local_a8 * -0x50;
        do {
          if (lVar10 == 0x50) goto LAB_04409e98;
          lVar10 = FUN_0433e338();
          lVar9 = lVar9 + 0x50;
        } while (lVar9 != 0);
      }
      local_a8 = (lVar4 - local_b0 >> 4) * -0x3333333333333333;
      FUN_00005810(local_b0,local_a0 * 0x50);
    }
    if (((*(byte *)(param_1 + 0x20) & 1) != 0) &&
       (FUN_0433e240(&local_80,*(undefined8 *)(*(long *)(param_1 + 0x10) + 0x368),
                     *(long *)(param_1 + 0x10) + 0x10), (*(byte *)(param_1 + 0x20) & 1) != 0)) {
      IVar7 = FUN_0473ed28(&local_80,&local_98,*(int *)(*(long *)(param_1 + 0x10) + 0x310) != 0);
      if ((long)local_70 < 0) {
        FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
      }
      local_80 = &local_98;
      FUN_0577ee50(&local_80);
      return IVar7;
    }
  }
LAB_04409e98:
                    /* WARNING: Does not return */
  pcVar6 = (code *)SoftwareBreakpoint(1,0x4409e9c);
  (*pcVar6)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8647
// ==== 0x440a0fc  ArcNotification::didClose:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcNotification::didClose_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433edd0(param_1 + 0x10,param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a114);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8666
// ==== 0x440a114  ArcNotification::didClick:input:  (172 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

void ArcNotification::didClick_input_(ID param_1,SEL param_2,int param_3,ID param_4)

{
  code *pcVar1;
  undefined8 local_48;
  undefined8 uStack_40;
  ulong local_38;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (param_4 == 0) {
      local_48 = 0;
      uStack_40 = 0;
      local_38 = 0;
    }
    else {
      FUN_01b356b8(&local_48,param_4);
    }
    FUN_0433ee60(param_1 + 0x10,param_3,&local_48,param_4 != 0);
    if ((long)local_38 < 0) {
      FUN_00005810(local_48,local_38 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a1c0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8701
// ==== 0x440a1e4  ArcNotification::didClickSettings  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcNotification::didClickSettings(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    FUN_0433ef10(param_1 + 0x10);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440a1f8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8720
// ==== 0x440a1f8  ArcNotification::didReceiveResponse:  (180 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcNotification::didReceiveResponse_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 local_30;
  int local_28;
  
  local_28 = 0;
  local_30 = 0;
  FUN_0473f138(&local_30,param_3);
  if (local_28 == 0) {
    didClose_(param_1,"didClose:",1);
  }
  else if (local_28 == 2) {
    didClickSettings(param_1,"didClickSettings");
  }
  else if (local_28 == 1) {
    iVar1 = (int)local_30;
    if (local_30._4_1_ == '\0') {
      iVar1 = 0xffffffff;
    }
    didClick_input_(param_1,"didClick:input:",iVar1,0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8753
// ==== 0x440a2ac  ArcMenuModel::menu_model  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMenuModel::menu_model
               (optional<arc::MenuModel> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x20) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::MenuModel> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x440a2ec);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x18);
    pIVar4 = *(Impl **)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8789
// ==== 0x440a2ec  ArcNotification::setNotification:  (288 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcNotification::setNotification_(ID param_1,SEL param_2,optional<arc::Notification> *param_3)

{
  char cVar1;
  long lVar2;
  __shared_weak_count *p_Var3;
  code *pcVar4;
  long *plVar5;
  
  cVar1 = *(char *)(param_1 + 0x20);
  if (cVar1 == (char)param_3->field1_0x10) {
    if (cVar1 == '\0') {
      return;
    }
    FUN_04331e20(param_1 + 0x10,param_3);
    return;
  }
  if (cVar1 == '\0') {
    *(Impl **)(param_1 + 0x10) = (param_3->field0_0x0).field1.field0_0x0.field0_0x0;
    p_Var3 = (param_3->field0_0x0).field1.field0_0x0.field1_0x8;
    *(__shared_weak_count **)(param_1 + 0x18) = p_Var3;
    if (p_Var3 != (__shared_weak_count *)0x0) {
      *(long *)(p_Var3 + 8) = *(long *)(p_Var3 + 8) + 1;
    }
    *(undefined1 *)(param_1 + 0x20) = 1;
    return;
  }
  plVar5 = *(long **)(param_1 + 0x18);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar5[1];
    plVar5[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar2 = plVar5[2];
        plVar5[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_0440a348;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_0440a348:
  *(undefined1 *)(param_1 + 0x20) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8850
// ==== 0x440a40c  ArcMenuModel::.cxx_destruct  (208 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcMenuModel::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x20) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x18), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0440a440;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_0440a440:
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:8892
// ==== 0x440a4dc  ArcMenuModel::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcMenuModel::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined1 *)(param_1 + 0x20) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9083
// ==== 0x440a85c  objc::Categories::ArcPermissionRequest_Extensions_::additionalData  (460 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcPermissionRequest_Extensions_::additionalData(ID param_1,SEL param_2)

{
  long *plVar1;
  char cVar2;
  int iVar3;
  long lVar4;
  ID IVar5;
  undefined8 uVar6;
  ID IVar7;
  code *pcVar8;
  long *plVar9;
  long local_68;
  long *plStack_60;
  byte local_58;
  
  IVar5 = NSMutableDictionary::dictionary((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionary");
  _objc_msgSend(param_1,"permission_request",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(1,0x440aa28);
    (*pcVar8)();
  }
  if (plStack_60 != (long *)0x0) {
    LOAcquire();
    lVar4 = plStack_60[1];
    plStack_60[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plStack_60 + 0x10))(plStack_60);
      if (plStack_60[2] != 0) {
        LOAcquire();
        lVar4 = plStack_60[2];
        plStack_60[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_0440a8cc;
      }
      pcVar8 = *(code **)(*plStack_60 + 0x20);
      if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40)) {
        FUN_00005810(plStack_60,0x20);
      }
      else {
        (*pcVar8)(plStack_60);
      }
    }
  }
LAB_0440a8cc:
  if (*(char *)(local_68 + 0x48) != '\0') {
    for (plVar9 = *(long **)(local_68 + 0x30); plVar9 != (long *)0x0; plVar9 = (long *)*plVar9) {
      iVar3 = *(int *)(plVar9 + 2);
      cVar2 = *(char *)((long)plVar9 + 0x2f);
      plVar1 = (long *)plVar9[3];
      if (-1 < (long)cVar2) {
        plVar1 = plVar9 + 3;
      }
      lVar4 = plVar9[4];
      if (-1 < cVar2) {
        lVar4 = (long)cVar2;
      }
      uVar6 = FUN_01c391bc(plVar1,lVar4);
      IVar7 = NSNumber::numberWithUnsignedInteger_
                        ((ID)&_OBJC_CLASS___NSNumber,"numberWithUnsignedInteger:",(long)iVar3);
      NSMutableDictionary::setObject_forKey_(IVar5,"setObject:forKey:",uVar6,IVar7);
    }
  }
  IVar5 = NSMutableDictionary::copy(IVar5,"copy");
  return IVar5;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9250
// ==== 0x440ab88  ArcSessionID::initWithID:  (44 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSessionID::initWithID_(ID param_1,SEL param_2,long_long param_3)

{
  ID IVar1;
  SessionID local_14;
  
  local_14.field0_0x0 = (int)param_3;
  IVar1 = initWithSessionId_(param_1,"initWithSessionId:",&local_14);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9266
// ==== 0x440abb4  ArcSessionID::initWithSessionId:  (96 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSessionID::initWithSessionId_(ID param_1,SEL param_2,const_SessionID__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcSessionID;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xc) != '\0') {
      *(undefined1 *)(IVar1 + 0xc) = 0;
    }
    *(int *)(IVar1 + 8) = param_3->field0_0x0;
    *(undefined1 *)(IVar1 + 0xc) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9291
// ==== 0x440ac14  ArcSessionID::isEqual:  (196 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcSessionID::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  uint uVar1;
  code *pcVar2;
  ID IVar3;
  bool bVar4;
  
  if (param_1 == param_3) {
    bVar4 = 1;
  }
  else {
    if (param_3 != 0) {
      IVar3 = _objc_msgSend((ID)&objc::class_t::ArcSessionID,"class");
      IVar3 = _objc_msgSend(param_3,"isKindOfClass:",IVar3);
      if ((int)IVar3 != 0) {
        IVar3 = _objc_msgSend(param_3,"sessionId");
        uVar1 = (uint)(IVar3 >> 0x20);
        if ((uVar1 & 0xff & (uint)*(byte *)(param_1 + 0xc)) == 0) {
          return (uint)((uint)*(byte *)(param_1 + 0xc) == (uVar1 & 0xff));
        }
        if ((IVar3 >> 0x20 & 1) != 0) {
          return (uint)(*(int *)(param_1 + 8) == (int)IVar3);
        }
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x440acd8);
        (*pcVar2)();
      }
    }
    bVar4 = 0;
  }
  return bVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9332
// ==== 0x440acd8  ArcSessionID::identifier  (28 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSessionID::identifier(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0xc) & 1) != 0) {
    return (long)*(int *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9347
// ==== 0x440acf4  ArcSessionID::setSessionId:  (16 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSessionID::setSessionId_(ID param_1,SEL param_2,optional<arc::SessionID> param_3)

{
  *(AnonymousUnion46 *)(param_1 + 8) = param_3.field0_0x0;
  *(char *)(param_1 + 0xc) = (char)param_3.field1_0x4;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9360
// ==== 0x440ad04  ArcSessionID::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSessionID::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0xc) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9468
// ==== 0x440ae60  ArcCookie::value  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCookie::value(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x2f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x18),*(undefined8 *)(lVar4 + 0x20));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x20);
      local_40 = *(undefined1 **)(lVar4 + 0x18);
      local_30 = *(ulong *)(lVar4 + 0x28);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x440aefc);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:9950
// ==== 0x440b880  ArcSiteSettingInfo::init  (324 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSiteSettingInfo::init(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  long *plVar4;
  code *pcVar5;
  long *local_50;
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcSiteSettingInfo;
  local_40 = param_1;
  IVar3 = NSObject::init((ID)&local_40,"init");
  if (IVar3 != 0) {
    plVar4 = (long *)FUN_00008790(0xb0);
    plVar4[1] = 0;
    plVar4[2] = 0;
    *plVar4 = (long)&PTR_viewWillStartLiveResize_0da7f770;
    plVar1 = plVar4 + 3;
    if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
      pcVar5 = (code *)SoftwareBreakpoint(1,0x440b9c4);
      (*pcVar5)();
    }
    *(undefined4 *)(plVar4 + 3) = 0;
    FUN_0bd3c0b4(plVar4 + 4);
    if ((*(byte *)(IVar3 + 0x18) & 1) == 0) {
      *(long **)(IVar3 + 8) = plVar1;
      *(long **)(IVar3 + 0x10) = plVar4;
      *(undefined1 *)(IVar3 + 0x18) = 1;
    }
    else {
      local_50 = plVar1;
      local_48 = plVar4;
      FUN_04331f00(IVar3 + 8,&local_50);
      plVar1 = local_48;
      if (local_48 != (long *)0x0) {
        LOAcquire();
        lVar2 = local_48[1];
        local_48[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          plVar4 = local_48 + 2;
          (**(code **)(*local_48 + 0x10))(local_48);
          if (*plVar4 != 0) {
            LOAcquire();
            lVar2 = plVar1[2];
            plVar1[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) {
              return IVar3;
            }
          }
          pcVar5 = *(code **)(*plVar1 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar5)(plVar1);
          }
        }
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10025
// ==== 0x440b9c4  ArcSiteSettingInfo::initWithSiteSettingInfo:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSiteSettingInfo::initWithSiteSettingInfo_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcSiteSettingInfo;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0440ba24;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0440ba20;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0440ba20:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0440ba24:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10084
// ==== 0x440bae8  ArcSiteSettingInfo::originUrl  (136 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSiteSettingInfo::originUrl(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if (((*(byte *)(param_1 + 0x18) & 1) != 0) &&
     ((*(byte *)(*(long *)(param_1 + 8) + 0x80) & 1) != 0)) {
    FUN_007d0cf8(&local_38,*(long *)(param_1 + 8) + 0x30);
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x440bb70);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10124
// ==== 0x440bb94  ArcSiteSettingInfo::setOriginUrl:  (148 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setOriginUrl_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    if (param_3 == 0) {
      local_38 = 0;
      uStack_30 = 0;
      local_28 = 0;
    }
    else {
      FUN_01b356b8(&local_38,param_3);
    }
    FUN_04341874(*(undefined8 *)(param_1 + 8),&local_38);
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc28);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10159
// ==== 0x440bc28  ArcSiteSettingInfo::permissionType  (64 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSiteSettingInfo::permissionType(ID param_1,SEL param_2)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  long_long lVar4;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04332684(**(undefined4 **)(param_1 + 8));
    uVar3 = FUN_02122528();
    if ((uVar3 >> 0x20 & 1) == 0) {
      lVar4 = 0x1c;
    }
    else {
      iVar2 = FUN_0433f850();
      lVar4 = (long_long)iVar2;
    }
    return lVar4;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc68);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10189
// ==== 0x440bc68  ArcSiteSettingInfo::contentSettingsType  (36 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcSiteSettingInfo::contentSettingsType(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bc88);
    (*pcVar1)();
  }
  if (**(uint **)(param_1 + 8) < 0x25) {
    return (ulong)**(uint **)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bc8c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10212
// ==== 0x440bc8c  ArcSiteSettingInfo::setContentSettingsType:  (36 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setContentSettingsType_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bcac);
    (*pcVar1)();
  }
  if (param_3 < 0x25) {
    **(undefined4 **)(param_1 + 8) = (int)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bcb0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10236
// ==== 0x440bcb0  ArcSiteSettingInfo::siteSetting  (72 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcSiteSettingInfo::siteSetting(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bcf4);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 8);
  if ((*(char *)(lVar2 + 0x18) != '\0') && (*(char *)(lVar2 + 0x14) == '\0')) {
    if (*(uint *)(lVar2 + 0xc) < 6) {
      return (ulong)(byte)(&DAT_0c9d8188)[*(uint *)(lVar2 + 0xc)];
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bcf8);
    (*pcVar1)();
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10264
// ==== 0x440bcf8  ArcSiteSettingInfo::setSiteSetting:  (36 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcSiteSettingInfo::setSiteSetting_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x440bd18);
    (*pcVar1)();
  }
  if (param_3 < 5) {
    FUN_04341964(*(undefined8 *)(param_1 + 8),param_3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x440bd1c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10531
// ==== 0x440c34c  ArcCpuInfo::init  (544 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCpuInfo::init(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  int iVar2;
  ID IVar3;
  undefined8 uVar4;
  long lVar5;
  undefined1 *local_50;
  long lStack_48;
  undefined7 local_40;
  char cStack_39;
  undefined1 local_31;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCpuInfo;
  local_30 = param_1;
  IVar3 = NSObject::init((ID)&local_30,"init");
  if (IVar3 != 0) {
    local_31 = 0;
    FUN_04341e44(&local_50,&local_31);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    lVar5 = lStack_48;
    if (-1 < cStack_39) {
      lVar5 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lVar5);
    *(undefined8 *)(IVar3 + 0x20) = uVar4;
    if (cStack_39 < '\0') {
      FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    FUN_04341ef0(&local_50,&local_31);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    lVar5 = lStack_48;
    if (-1 < cStack_39) {
      lVar5 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lVar5);
    *(undefined8 *)(IVar3 + 0x28) = uVar4;
    uVar4 = 0;
    if (cStack_39 < '\0') {
      uVar4 = FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    FUN_04572478(&local_50,uVar4);
    ppuVar1 = (undefined1 **)local_50;
    if (-1 < (long)cStack_39) {
      ppuVar1 = &local_50;
    }
    if (-1 < cStack_39) {
      lStack_48 = (long)cStack_39;
    }
    uVar4 = FUN_01c391bc(ppuVar1,lStack_48);
    *(undefined8 *)(IVar3 + 0x30) = uVar4;
    uVar4 = 0;
    if (cStack_39 < '\0') {
      uVar4 = FUN_00005810(local_50,CONCAT17(cStack_39,local_40) & 0x7fffffffffffffff);
    }
    lVar5 = FUN_083a7e78(uVar4);
    *(long *)(IVar3 + 0x38) = (long)*(int *)(lVar5 + 4);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x40) = (long)*(int *)(lVar5 + 8);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x48) = (long)*(int *)(lVar5 + 0xc);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x50) = (long)*(int *)(lVar5 + 0x10);
    lVar5 = FUN_083a7e78();
    *(long *)(IVar3 + 0x58) = (long)*(int *)(lVar5 + 0x14);
    lVar5 = FUN_083a7e78();
    iVar2 = *(int *)(lVar5 + 0x18);
    *(undefined1 *)(IVar3 + 8) = 0;
    *(long *)(IVar3 + 0x60) = (long)iVar2;
    *(undefined8 *)(IVar3 + 0x68) = 0;
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 9) = *(undefined1 *)(lVar5 + 0x20);
    if (((DAT_0e4a2f30 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e4a2f30), iVar2 != 0)) {
      DAT_0e4a2f28 = 0;
      thunk_FUN_0c44b760(&DAT_0e4a2f30);
    }
    *(undefined1 *)(IVar3 + 10) = DAT_0e4a2f28;
    *(undefined8 *)(IVar3 + 0xb) = 0;
    *(undefined4 *)(IVar3 + 0x12) = 0;
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x16) = *(undefined1 *)(lVar5 + 0x1c);
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x17) = *(undefined1 *)(lVar5 + 0x1d);
    lVar5 = FUN_083a7e78();
    *(undefined1 *)(IVar3 + 0x18) = *(undefined1 *)(lVar5 + 0x1e);
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10637
// ==== 0x440c5a4  ArcCpuInfo::hasMMX  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasMMX(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0xb);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10648
// ==== 0x440c5ac  ArcCpuInfo::hasSSE  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSE(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0xc);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10659
// ==== 0x440c5b4  ArcCpuInfo::hasSSE2  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSE2(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0xd);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10670
// ==== 0x440c5bc  ArcCpuInfo::hasSSE3  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSE3(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0xe);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10681
// ==== 0x440c5c4  ArcCpuInfo::hasSSSE3  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSSE3(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0xf);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10692
// ==== 0x440c5cc  ArcCpuInfo::hasSSE42  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasSSE42(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x11);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10703
// ==== 0x440c5d4  ArcCpuInfo::hasPOPCNT  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasPOPCNT(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x12);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10714
// ==== 0x440c5dc  ArcCpuInfo::hasAVX  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasAVX(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x13);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10725
// ==== 0x440c5e4  ArcCpuInfo::hasAVX2  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasAVX2(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x14);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10736
// ==== 0x440c5ec  ArcCpuInfo::hasFMA3  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasFMA3(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x15);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10747
// ==== 0x440c5f4  ArcCpuInfo::hasAESNI  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasAESNI(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x16);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10758
// ==== 0x440c5fc  ArcCpuInfo::hasMTE  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_char ArcCpuInfo::hasMTE(ID param_1,SEL param_2)

{
  return *(unsigned_char *)(param_1 + 0x17);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10769
// ==== 0x440c604  ArcCpuInfo::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcCpuInfo::_cxx_destruct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10784
// ==== 0x440c640  ArcTracingConfiguration::init  (120 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcTracingConfiguration::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcTracingConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x2f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar1 + 0x18),*(ulong *)(IVar1 + 0x28) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar1 + 0x30) = 0;
    *(undefined8 *)(IVar1 + 0x28) = 0;
    *(undefined8 *)(IVar1 + 0x20) = 0;
    *(undefined8 *)(IVar1 + 0x18) = 0;
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10812
// ==== 0x440c6b8  ArcTracingConfiguration::tracingConfiguration  (88 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcTracingConfiguration::tracingConfiguration
               (TracingConfiguration *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 0x18),
                 *(undefined8 *)(param_1 + 0x20));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x20);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x28);
  }
  __return_storage_ptr__->field1_0x18 = *(double *)(param_1 + 0x30);
  *(undefined1 *)&__return_storage_ptr__->field2_0x20 = *(undefined1 *)(param_1 + 0x38);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10840
// ==== 0x440c710  ArcTracingConfiguration::filePath  (84 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcTracingConfiguration::filePath(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 uVar4;
  
  cVar3 = *(char *)(param_1 + 0x2f);
  puVar1 = *(undefined8 **)(param_1 + 0x18);
  if (-1 < (long)cVar3) {
    puVar1 = (undefined8 *)(param_1 + 0x18);
  }
  lVar2 = *(long *)(param_1 + 0x20);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar4 = FUN_01c391bc(puVar1,lVar2);
  *(undefined8 *)(param_1 + 8) = uVar4;
  return *(ID *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10872
// ==== 0x440c764  ArcTracingConfiguration::setFilePath:  (156 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcTracingConfiguration::setFilePath_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  *(ID *)(param_1 + 8) = param_3;
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x18),*(ulong *)(param_1 + 0x28) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 0x20) = uStack_30;
  *(undefined8 *)(param_1 + 0x18) = local_38;
  *(undefined8 *)(param_1 + 0x28) = local_28;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10905
// ==== 0x440c800  ArcTracingConfiguration::tracingDuration  (12 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcTracingConfiguration::tracingDuration(ID param_1,SEL param_2)

{
  *(double *)(param_1 + 0x10) = *(double *)(param_1 + 0x30);
  return *(double *)(param_1 + 0x30);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10917
// ==== 0x440c80c  ArcTracingConfiguration::setTracingDuration:  (12 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcTracingConfiguration::setTracingDuration_(ID param_1,SEL param_2,double param_3)

{
  *(double *)(param_1 + 0x10) = param_3;
  *(double *)(param_1 + 0x30) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10930
// ==== 0x440c818  ArcTracingConfiguration::setPrivacyFilteringEnabled:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcTracingConfiguration::setPrivacyFilteringEnabled_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x38) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10942
// ==== 0x440c820  ArcTracingConfiguration::setTracingConfiguration:  (140 bytes)

/* Function Stack Size: 0x38 bytes */

void ArcTracingConfiguration::setTracingConfiguration_
               (ID param_1,SEL param_2,TracingConfiguration *param_3)

{
  byte bVar1;
  TracingConfiguration *pTVar2;
  double dVar3;
  char *pcVar4;
  unsigned_long_long uVar5;
  
  if ((TracingConfiguration *)(param_1 + 0x18) != param_3) {
    bVar1 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x2f) < '\0') {
      uVar5 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pTVar2 = (TracingConfiguration *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar1) {
        uVar5 = (ulong)bVar1;
        pTVar2 = param_3;
      }
      FUN_00359ad8(param_1 + 0x18,pTVar2,uVar5);
    }
    else if ((char)bVar1 < '\0') {
      FUN_00359998(param_1 + 0x18,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar5 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pcVar4 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x28) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x20) = uVar5;
      (((TracingConfiguration *)(param_1 + 0x18))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar4
      ;
    }
  }
  dVar3 = param_3->field1_0x18;
  *(char *)(param_1 + 0x38) = (char)param_3->field2_0x20;
  *(double *)(param_1 + 0x30) = dVar3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:10987
// ==== 0x440c8ac  ArcTracingConfiguration::.cxx_destruct  (60 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcTracingConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x2f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x18),*(ulong *)(param_1 + 0x28) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11003
// ==== 0x440c8e8  ArcTracingConfiguration::.cxx_construct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcTracingConfiguration::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined1 *)(param_1 + 0x38) = 1;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11442
// ==== 0x440d04c  _emitTraceEvent  (396 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8
_emitTraceEvent(undefined8 param_1,long param_2,undefined8 param_3,undefined8 param_4,long param_5,
               undefined8 param_6,long param_7)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  ulong local_58;
  
  if (DAT_0e4a445a != '\0') {
    local_68 = 0;
    uStack_60 = 0;
    local_58 = 0;
    if (param_5 != 0) {
      FUN_01b356b8(&local_68,param_5);
    }
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
    if (param_7 != 0) {
      FUN_01b356b8(&local_98,param_7);
      uStack_78 = uStack_90;
      local_80 = local_98;
      local_70 = local_88;
    }
    if (param_2 == 0) {
      local_98 = 0;
      uStack_90 = 0;
      local_88 = 0;
    }
    else {
      FUN_01b356b8(&local_98,param_2);
    }
    puVar1 = (undefined8 *)0x0;
    if (param_5 != 0) {
      puVar1 = &local_68;
    }
    puVar2 = (undefined8 *)0x0;
    if (param_7 != 0) {
      puVar2 = &local_80;
    }
    FUN_043422d4(param_1,&local_98,param_3,param_4,puVar1,param_6,puVar2);
    if ((long)local_88 < 0) {
      FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
    }
    if ((long)local_70 < 0) {
      FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
    }
    if ((long)local_58 < 0) {
      FUN_00005810(local_68,local_58 & 0x7fffffffffffffff);
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11511
// ==== 0x440d23c  _arcCategoryEnabledFlag  (12 bytes)

undefined1 * _arcCategoryEnabledFlag(void)

{
  return &DAT_0e4a445a;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11579
// ==== 0x440d37c  ArcURLLoadCancellationToken::initWithToken:  (160 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcURLLoadCancellationToken::initWithToken_(ID param_1,SEL param_2,const_void__ param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  byte bVar3;
  const_void__ pvVar4;
  ID IVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcURLLoadCancellationToken;
  local_30 = param_1;
  IVar5 = NSObject::init((ID)&local_30,"init");
  puVar1 = (undefined8 *)(IVar5 + 8);
  if (IVar5 != 0 && puVar1 != param_3) {
    bVar3 = *(byte *)((long)param_3 + 0x17);
    if (*(char *)(IVar5 + 0x1f) < '\0') {
      uVar2 = *(ulong *)((long)param_3 + 8);
      pvVar4 = *(const_void__ *)param_3;
      if (-1 < (char)bVar3) {
        uVar2 = (ulong)bVar3;
        pvVar4 = param_3;
      }
      FUN_00359ad8(puVar1,pvVar4,uVar2);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(puVar1,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar7 = *(undefined8 *)((long)param_3 + 8);
      uVar6 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar5 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar5 + 0x10) = uVar7;
      *puVar1 = uVar6;
    }
  }
  return IVar5;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11626
// ==== 0x440d41c  ArcURLLoadCancellationToken::setToken:  (84 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcURLLoadCancellationToken::setToken_
               (ID param_1,SEL param_2,
               basic_string<char,_std::char_traits<char>,_std::allocator<char>> *param_3)

{
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar1;
  byte bVar2;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar3;
  char *pcVar4;
  unsigned_long_long uVar5;
  
  pbVar1 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(param_1 + 8);
  if (pbVar1 != param_3) {
    bVar2 = *(byte *)((long)&param_3->field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar5 = (param_3->field0_0x0).field1.field1_0x8;
      pbVar3 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field0_0x0).field1.field0_0x0;
      if (-1 < (char)bVar2) {
        uVar5 = (ulong)bVar2;
        pbVar3 = param_3;
      }
      FUN_00359ad8(pbVar1,pbVar3,uVar5);
      return;
    }
    if ((char)bVar2 < '\0') {
      FUN_00359998(pbVar1,(param_3->field0_0x0).field1.field0_0x0,
                   (param_3->field0_0x0).field1.field1_0x8);
      return;
    }
    uVar5 = (param_3->field0_0x0).field1.field1_0x8;
    pcVar4 = (param_3->field0_0x0).field1.field0_0x0;
    *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field1.field2_0x10;
    *(unsigned_long_long *)(param_1 + 0x10) = uVar5;
    (pbVar1->field0_0x0).field1.field0_0x0 = pcVar4;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11670
// ==== 0x440d470  ArcNetworkLoggingConfiguration::.cxx_destruct  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcNetworkLoggingConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  if (-1 < *(char *)(param_1 + 0x1f)) {
    return;
  }
  FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11685
// ==== 0x440d490  ArcURLLoadConfiguration::initWithConfig:  (488 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcURLLoadConfiguration::initWithConfig_(ID param_1,SEL param_2,const_void__ param_3)

{
  undefined8 *puVar1;
  ulong uVar2;
  undefined4 uVar3;
  byte bVar4;
  const_void__ pvVar5;
  undefined8 *puVar6;
  ID IVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcURLLoadConfiguration;
  local_30 = param_1;
  IVar7 = NSObject::init((ID)&local_30,"init");
  if (IVar7 != 0) {
    if ((undefined8 *)(IVar7 + 8) != param_3) {
      bVar4 = *(byte *)((long)param_3 + 0x17);
      if (*(char *)(IVar7 + 0x1f) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 8);
        pvVar5 = *(const_void__ *)param_3;
        if (-1 < (char)bVar4) {
          uVar2 = (ulong)bVar4;
          pvVar5 = param_3;
        }
        FUN_00359ad8(IVar7 + 8,pvVar5,uVar2);
      }
      else if ((char)bVar4 < '\0') {
        FUN_00359998(IVar7 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
      }
      else {
        uVar9 = *(undefined8 *)((long)param_3 + 8);
        uVar8 = *(undefined8 *)param_3;
        *(undefined8 *)(IVar7 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
        *(undefined8 *)(IVar7 + 0x10) = uVar9;
        *(undefined8 *)(IVar7 + 8) = uVar8;
      }
    }
    puVar1 = (undefined8 *)((long)param_3 + 0x18);
    if ((undefined8 *)(IVar7 + 0x20) != puVar1) {
      bVar4 = *(byte *)((long)param_3 + 0x2f);
      if (*(char *)(IVar7 + 0x37) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 0x20);
        puVar6 = *(undefined8 **)((long)param_3 + 0x18);
        if (-1 < (char)bVar4) {
          uVar2 = (ulong)bVar4;
          puVar6 = puVar1;
        }
        FUN_00359ad8(IVar7 + 0x20,puVar6,uVar2);
      }
      else if ((char)bVar4 < '\0') {
        FUN_00359998(IVar7 + 0x20,*(undefined8 *)((long)param_3 + 0x18),
                     *(undefined8 *)((long)param_3 + 0x20));
      }
      else {
        uVar9 = *(undefined8 *)((long)param_3 + 0x20);
        uVar8 = *puVar1;
        *(undefined8 *)(IVar7 + 0x30) = *(undefined8 *)((long)param_3 + 0x28);
        *(undefined8 *)(IVar7 + 0x28) = uVar9;
        *(undefined8 *)(IVar7 + 0x20) = uVar8;
      }
    }
    puVar1 = (undefined8 *)((long)param_3 + 0x30);
    if ((undefined8 *)(IVar7 + 0x38) != puVar1) {
      bVar4 = *(byte *)((long)param_3 + 0x47);
      if (*(char *)(IVar7 + 0x4f) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 0x38);
        puVar6 = *(undefined8 **)((long)param_3 + 0x30);
        if (-1 < (char)bVar4) {
          uVar2 = (ulong)bVar4;
          puVar6 = puVar1;
        }
        FUN_00359ad8(IVar7 + 0x38,puVar6,uVar2);
      }
      else if ((char)bVar4 < '\0') {
        FUN_00359998(IVar7 + 0x38,*(undefined8 *)((long)param_3 + 0x30),
                     *(undefined8 *)((long)param_3 + 0x38));
      }
      else {
        uVar9 = *(undefined8 *)((long)param_3 + 0x38);
        uVar8 = *puVar1;
        *(undefined8 *)(IVar7 + 0x48) = *(undefined8 *)((long)param_3 + 0x40);
        *(undefined8 *)(IVar7 + 0x40) = uVar9;
        *(undefined8 *)(IVar7 + 0x38) = uVar8;
      }
    }
    puVar1 = (undefined8 *)((long)param_3 + 0x48);
    if ((undefined8 *)(IVar7 + 0x50) != puVar1) {
      bVar4 = *(byte *)((long)param_3 + 0x5f);
      if (*(char *)(IVar7 + 0x67) < '\0') {
        uVar2 = *(ulong *)((long)param_3 + 0x50);
        puVar6 = *(undefined8 **)((long)param_3 + 0x48);
        if (-1 < (char)bVar4) {
          uVar2 = (ulong)bVar4;
          puVar6 = puVar1;
        }
        FUN_00359ad8(IVar7 + 0x50,puVar6,uVar2);
      }
      else if ((char)bVar4 < '\0') {
        FUN_00359998(IVar7 + 0x50,*(undefined8 *)((long)param_3 + 0x48),
                     *(undefined8 *)((long)param_3 + 0x50));
      }
      else {
        uVar9 = *(undefined8 *)((long)param_3 + 0x50);
        uVar8 = *puVar1;
        *(undefined8 *)(IVar7 + 0x60) = *(undefined8 *)((long)param_3 + 0x58);
        *(undefined8 *)(IVar7 + 0x58) = uVar9;
        *(undefined8 *)(IVar7 + 0x50) = uVar8;
      }
    }
    FUN_04b8b0b0(IVar7 + 0x68,(long)param_3 + 0x60);
    uVar3 = *(undefined4 *)((long)param_3 + 0x97);
    uVar8 = *(undefined8 *)((long)param_3 + 0x88);
    *(undefined8 *)(IVar7 + 0x98) = *(undefined8 *)((long)param_3 + 0x90);
    *(undefined8 *)(IVar7 + 0x90) = uVar8;
    *(undefined4 *)(IVar7 + 0x9f) = uVar3;
  }
  return IVar7;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11813
// ==== 0x440d678  ArcURLLoadConfiguration::url  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::url(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = FUN_043abd34(param_1 + 8);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11827
// ==== 0x440d680  ArcURLLoadConfiguration::setUrl:  (156 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setUrl_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(param_1 + 8) = 0;
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
  }
  else {
    FUN_043abdf8(&local_38,param_3);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(param_1 + 0x10) = uStack_30;
    *(undefined8 *)(param_1 + 8) = local_38;
    *(undefined8 *)(param_1 + 0x18) = local_28;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11861
// ==== 0x440d71c  ArcURLLoadConfiguration::method  (40 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcURLLoadConfiguration::method(ID param_1,SEL param_2)

{
  uint uVar1;
  
  uVar1 = *(int *)(param_1 + 0x90) - 1;
  if (2 < uVar1) {
    return 0;
  }
  return (ulong)(byte)(&DAT_0c9d8190)[uVar1];
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11878
// ==== 0x440d744  ArcURLLoadConfiguration::setMethod:  (40 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setMethod_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  uint uVar1;
  
  if (param_3 - 1 < 3) {
    uVar1 = (uint)(byte)(&UNK_0c9d818f)[param_3];
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(param_1 + 0x90) = uVar1;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11898
// ==== 0x440d76c  ArcURLLoadConfiguration::setPayload:  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayload_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  if (*(char *)(param_1 + 0x37) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 0x28) = uStack_30;
  *(undefined8 *)(param_1 + 0x20) = local_38;
  *(undefined8 *)(param_1 + 0x30) = local_28;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11929
// ==== 0x440d7ec  ArcURLLoadConfiguration::payloadData  (72 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::payloadData(ID param_1,SEL param_2)

{
  ID IVar1;
  long lVar2;
  long lVar3;
  
  IVar1 = _objc_alloc(&_OBJC_CLASS___NSData);
  lVar3 = (long)*(char *)(param_1 + 0x37);
  if (lVar3 < 0) {
    lVar2 = *(long *)(param_1 + 0x20);
    lVar3 = *(long *)(param_1 + 0x28);
  }
  else {
    lVar2 = param_1 + 0x20;
  }
  IVar1 = NSData::initWithBytes_length_(IVar1,"initWithBytes:length:",lVar2,lVar3);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11955
// ==== 0x440d834  ArcURLLoadConfiguration::setPayloadData:  (124 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayloadData_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  ID IVar3;
  
  IVar2 = _objc_retainAutorelease(param_3);
  IVar2 = _objc_msgSend(IVar2,"bytes");
  IVar3 = _objc_msgSend(param_3,"length");
  if (IVar2 != 0 || IVar3 == 0) {
    FUN_0043b8b0(param_1 + 0x20,IVar2,IVar3);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440d8b0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11981
// ==== 0x440d8b0  ArcURLLoadConfiguration::payloadFileURL  (8 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::payloadFileURL(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = FUN_043abd34(param_1 + 0x38);
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:11995
// ==== 0x440d8b8  ArcURLLoadConfiguration::setPayloadFileURL:  (156 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayloadFileURL_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    if (*(char *)(param_1 + 0x4f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x38),*(ulong *)(param_1 + 0x48) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(param_1 + 0x38) = 0;
    *(undefined8 *)(param_1 + 0x40) = 0;
    *(undefined8 *)(param_1 + 0x48) = 0;
  }
  else {
    FUN_043abdf8(&local_38,param_3);
    if (*(char *)(param_1 + 0x4f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x38),*(ulong *)(param_1 + 0x48) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(param_1 + 0x40) = uStack_30;
    *(undefined8 *)(param_1 + 0x38) = local_38;
    *(undefined8 *)(param_1 + 0x48) = local_28;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12029
// ==== 0x440d954  ArcURLLoadConfiguration::payloadContentType  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::payloadContentType(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x67);
  plVar1 = (long *)*(long *)(param_1 + 0x50);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x50);
  }
  lVar2 = *(long *)(param_1 + 0x58);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12055
// ==== 0x440d978  ArcURLLoadConfiguration::setPayloadContentType:  (128 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setPayloadContentType_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_38;
  undefined8 uStack_30;
  undefined8 local_28;
  
  if (param_3 == 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  if (*(char *)(param_1 + 0x67) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x50),*(ulong *)(param_1 + 0x60) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 0x58) = uStack_30;
  *(undefined8 *)(param_1 + 0x50) = local_38;
  *(undefined8 *)(param_1 + 0x60) = local_28;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12086
// ==== 0x440d9f8  ArcURLLoadConfiguration::headers  (252 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::headers(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  long *plVar7;
  
  IVar4 = NSMutableDictionary::dictionary((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionary");
  for (plVar7 = *(long **)(param_1 + 0x78); plVar7 != (long *)0x0; plVar7 = (long *)*plVar7) {
    cVar3 = *(char *)((long)plVar7 + 0x3f);
    plVar1 = (long *)plVar7[5];
    if (-1 < (long)cVar3) {
      plVar1 = plVar7 + 5;
    }
    lVar2 = plVar7[6];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar5 = FUN_01c391bc(plVar1,lVar2);
    cVar3 = *(char *)((long)plVar7 + 0x27);
    plVar1 = (long *)plVar7[2];
    if (-1 < (long)cVar3) {
      plVar1 = plVar7 + 2;
    }
    lVar2 = plVar7[3];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar6 = FUN_01c391bc(plVar1,lVar2);
    NSMutableDictionary::setValue_forKey_(IVar4,"setValue:forKey:",uVar5,uVar6);
  }
  IVar4 = NSMutableDictionary::copy(IVar4,"copy");
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12134
// ==== 0x440daf4  ArcURLLoadConfiguration::setFollowRedirects:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcURLLoadConfiguration::setFollowRedirects_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xa0) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12146
// ==== 0x440dafc  ArcURLLoadConfiguration::streamResponseData  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcURLLoadConfiguration::streamResponseData(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xa1);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12157
// ==== 0x440db04  ArcURLLoadConfiguration::setStreamResponseData:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcURLLoadConfiguration::setStreamResponseData_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xa1) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12169
// ==== 0x440db0c  ArcURLLoadConfiguration::timeout  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcURLLoadConfiguration::timeout(int param_1)

{
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12180
// ==== 0x440db14  ArcURLLoadConfiguration::setTimeout:  (8 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setTimeout_(ID param_1,SEL param_2,double param_3)

{
  *(double *)(param_1 + 0x98) = param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12192
// ==== 0x440db1c  ArcURLLoadConfiguration::cachePolicy  (16 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcURLLoadConfiguration::cachePolicy(ID param_1,SEL param_2)

{
  return (ulong)(*(int *)(param_1 + 0x94) == 1);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12203
// ==== 0x440db2c  ArcURLLoadConfiguration::setCachePolicy:  (16 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setCachePolicy_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  *(uint *)(param_1 + 0x94) = (uint)(param_3 == 1);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12215
// ==== 0x440db3c  ArcURLLoadConfiguration::blockLocalRequest  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcURLLoadConfiguration::blockLocalRequest(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xa2);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12226
// ==== 0x440db44  ArcURLLoadConfiguration::setBlockLocalRequest:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcURLLoadConfiguration::setBlockLocalRequest_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xa2) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12238
// ==== 0x440db4c  ArcURLLoadConfiguration::config  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcURLLoadConfiguration::config(URLLoadConfig *__return_storage_ptr__,ID param_1,SEL param_2)

{
  FUN_04345144(__return_storage_ptr__,param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12250
// ==== 0x440db58  ArcURLLoadConfiguration::setConfig:  (440 bytes)

/* Function Stack Size: 0xb0 bytes */

void ArcURLLoadConfiguration::setConfig_(ID param_1,SEL param_2,URLLoadConfig *param_3)

{
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar1;
  undefined4 uVar2;
  byte bVar3;
  URLLoadConfig *pUVar4;
  basic_string<char,_std::char_traits<char>,_std::allocator<char>> *pbVar5;
  char *pcVar6;
  undefined8 uVar7;
  unsigned_long_long uVar8;
  
  if ((URLLoadConfig *)(param_1 + 8) != param_3) {
    bVar3 = *(byte *)((long)&(param_3->field0_0x0).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x1f) < '\0') {
      uVar8 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pUVar4 = (URLLoadConfig *)(param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar8 = (ulong)bVar3;
        pUVar4 = param_3;
      }
      FUN_00359ad8(param_1 + 8,pUVar4,uVar8);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(param_1 + 8,(param_3->field0_0x0).field0_0x0.field1.field0_0x0,
                   (param_3->field0_0x0).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar8 = (param_3->field0_0x0).field0_0x0.field1.field1_0x8;
      pcVar6 = (param_3->field0_0x0).field0_0x0.field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x18) = (param_3->field0_0x0).field0_0x0.field1.field2_0x10;
      *(unsigned_long_long *)(param_1 + 0x10) = uVar8;
      (((URLLoadConfig *)(param_1 + 8))->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar6;
    }
  }
  pbVar1 = &param_3->field1_0x18;
  if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(param_1 + 0x20) != pbVar1
     ) {
    bVar3 = *(byte *)((long)&(param_3->field1_0x18).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x37) < '\0') {
      uVar8 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
      pbVar5 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field1_0x18).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar8 = (ulong)bVar3;
        pbVar5 = pbVar1;
      }
      FUN_00359ad8(param_1 + 0x20,pbVar5,uVar8);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(param_1 + 0x20,(param_3->field1_0x18).field0_0x0.field1.field0_0x0,
                   (param_3->field1_0x18).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar8 = (param_3->field1_0x18).field0_0x0.field1.field1_0x8;
      pcVar6 = (pbVar1->field0_0x0).field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x30) = (param_3->field1_0x18).field0_0x0.field1.field2_0x10
      ;
      *(unsigned_long_long *)(param_1 + 0x28) = uVar8;
      *(char **)(param_1 + 0x20) = pcVar6;
    }
  }
  pbVar1 = &param_3->field2_0x30;
  if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(param_1 + 0x38) != pbVar1
     ) {
    bVar3 = *(byte *)((long)&(param_3->field2_0x30).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x4f) < '\0') {
      uVar8 = (param_3->field2_0x30).field0_0x0.field1.field1_0x8;
      pbVar5 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field2_0x30).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar8 = (ulong)bVar3;
        pbVar5 = pbVar1;
      }
      FUN_00359ad8(param_1 + 0x38,pbVar5,uVar8);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(param_1 + 0x38,(param_3->field2_0x30).field0_0x0.field1.field0_0x0,
                   (param_3->field2_0x30).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar8 = (param_3->field2_0x30).field0_0x0.field1.field1_0x8;
      pcVar6 = (pbVar1->field0_0x0).field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x48) = (param_3->field2_0x30).field0_0x0.field1.field2_0x10
      ;
      *(unsigned_long_long *)(param_1 + 0x40) = uVar8;
      *(char **)(param_1 + 0x38) = pcVar6;
    }
  }
  pbVar1 = &param_3->field3_0x48;
  if ((basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)(param_1 + 0x50) != pbVar1
     ) {
    bVar3 = *(byte *)((long)&(param_3->field3_0x48).field0_0x0 + 0x17);
    if (*(char *)(param_1 + 0x67) < '\0') {
      uVar8 = (param_3->field3_0x48).field0_0x0.field1.field1_0x8;
      pbVar5 = (basic_string<char,_std::char_traits<char>,_std::allocator<char>> *)
               (param_3->field3_0x48).field0_0x0.field1.field0_0x0;
      if (-1 < (char)bVar3) {
        uVar8 = (ulong)bVar3;
        pbVar5 = pbVar1;
      }
      FUN_00359ad8(param_1 + 0x50,pbVar5,uVar8);
    }
    else if ((char)bVar3 < '\0') {
      FUN_00359998(param_1 + 0x50,(param_3->field3_0x48).field0_0x0.field1.field0_0x0,
                   (param_3->field3_0x48).field0_0x0.field1.field1_0x8);
    }
    else {
      uVar8 = (param_3->field3_0x48).field0_0x0.field1.field1_0x8;
      pcVar6 = (pbVar1->field0_0x0).field1.field0_0x0;
      *(AnonymousBitField1 *)(param_1 + 0x60) = (param_3->field3_0x48).field0_0x0.field1.field2_0x10
      ;
      *(unsigned_long_long *)(param_1 + 0x58) = uVar8;
      *(char **)(param_1 + 0x50) = pcVar6;
    }
  }
  FUN_04b8b0b0(param_1 + 0x68,&param_3->field4_0x60);
  uVar2 = *(undefined4 *)((long)&param_3->field8_0x94 + 3);
  uVar7 = *(undefined8 *)&param_3->field6_0x88;
  *(undefined8 *)(param_1 + 0x98) = *(undefined8 *)((long)&param_3->field7_0x8c + 4);
  *(undefined8 *)(param_1 + 0x90) = uVar7;
  *(undefined4 *)(param_1 + 0x9f) = uVar2;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:12380
// ==== 0x440dd10  ArcURLLoadConfiguration::.cxx_construct  (64 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x80) = 0;
  *(undefined8 *)(param_1 + 0x78) = 0;
  *(undefined8 *)(param_1 + 0x70) = 0;
  *(undefined8 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x60) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0x88) = 0x3f800000;
  *(undefined8 *)(param_1 + 0x90) = 0;
  *(undefined8 *)(param_1 + 0x98) = 0;
  *(undefined2 *)(param_1 + 0xa0) = 1;
  *(undefined1 *)(param_1 + 0xa2) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:14049
// ==== 0x4410ab4  objc::Categories::ArcWebContents_Extensions_::closeWithForegroundCommitTiming:  (240 bytes)

/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::closeWithForegroundCommitTiming_
               (ID param_1,SEL param_2,long_long param_3)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_38;
  long *local_30;
  byte local_28;
  
  _objc_msgSend(param_1,"web_contents",&local_38);
  if ((local_28 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4410ba4);
    (*pcVar2)();
  }
  FUN_0437854c(local_38,param_3);
  if ((local_28 != 0) && (local_30 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_30[1];
    local_30[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_30 + 0x10))(local_30);
      if (local_30[2] != 0) {
        LOAcquire();
        lVar1 = local_30[2];
        local_30[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_30 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_30,0x20);
      }
      else {
        (*pcVar2)(local_30);
      }
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:14099
// ==== 0x4410ba4  objc::Categories::ArcWebContents_Extensions_::close  (16 bytes)

/* Function Stack Size: 0x10 bytes */

int objc::Categories::ArcWebContents_Extensions_::close(int param_1)

{
  ID IVar1;
  
  IVar1 = _objc_msgSend((ulong)(uint)param_1,"closeWithForegroundCommitTiming:",1);
  return (int)IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:14325
// ==== 0x4411214  objc::Categories::ArcWebContents_Extensions_::devTools  (712 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::devTools(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *in_x3;
  code *pcVar4;
  long *plVar5;
  long lVar6;
  undefined8 uVar7;
  long *plVar8;
  DevTools DVar9;
  undefined8 local_88;
  long *local_80;
  long local_78;
  long *local_70;
  byte local_68;
  undefined8 local_60;
  long *plStack_58;
  
  IVar3 = _objc_getAssociatedObject(param_1,"devTools");
  if (IVar3 != 0) {
    return IVar3;
  }
  _objc_msgSend(param_1,"web_contents",&local_78);
  if ((local_68 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x44114dc);
    (*pcVar4)();
  }
  lVar6 = *(long *)(local_78 + 0x2b8);
  if (lVar6 == 0) {
    plVar5 = (long *)0x0;
    uVar7 = 0;
LAB_044112bc:
    if (local_70 != (long *)0x0) {
      LOAcquire();
      lVar2 = local_70[1];
      local_70[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*local_70 + 0x10))(local_70);
        if (local_70[2] != 0) {
          LOAcquire();
          lVar2 = local_70[2];
          local_70[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_044112d4;
        }
        pcVar4 = *(code **)(*local_70 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(local_70,0x20);
        }
        else {
          (*pcVar4)(local_70);
        }
      }
    }
LAB_044112d4:
    if (lVar6 == 0) {
      return 0;
    }
  }
  else {
    FUN_042c8400(&local_60,lVar6);
    plVar5 = plStack_58;
    uVar7 = local_60;
    if ((local_68 & 1) != 0) goto LAB_044112bc;
  }
  IVar3 = _objc_alloc(&class_t::ArcWebContentsDevTools);
  if (plVar5 != (long *)0x0) {
    plVar5[1] = plVar5[1] + 1;
  }
  DVar9.field0_0x0.field1_0x8 = in_x3;
  DVar9.field0_0x0.field0_0x0 = (Impl *)&local_88;
  local_88 = uVar7;
  local_80 = plVar5;
  IVar3 = ArcWebContentsDevTools::init_(IVar3,"init:",DVar9);
  plVar1 = local_80;
  if (local_80 != (long *)0x0) {
    LOAcquire();
    lVar6 = local_80[1];
    local_80[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar8 = local_80 + 2;
      (**(code **)(*local_80 + 0x10))(local_80);
      if (*plVar8 != 0) {
        LOAcquire();
        lVar6 = plVar1[2];
        plVar1[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_04411324;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_04411324:
  _objc_setAssociatedObject(param_1,"devTools",IVar3,1);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar6 = plVar5[1];
    plVar5[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar6 = plVar5[2];
        plVar5[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16275
// ==== 0x4414800  ArcAutocompleteMatch::autocomplete_match  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAutocompleteMatch::autocomplete_match
               (optional<arc::AutocompleteMatch> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  long *plVar1;
  code *pcVar2;
  long lVar3;
  Impl *pIVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 0;
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AutocompleteMatch> *)0x0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x4414840);
      (*pcVar2)();
    }
    lVar3 = *(long *)(param_1 + 0x20);
    pIVar4 = *(Impl **)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field1_0x8 =
         *(__shared_weak_count **)(param_1 + 0x20);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0 = pIVar4;
    if (lVar3 != 0) {
      plVar1 = (long *)(lVar3 + 8);
      *plVar1 = *plVar1 + 1;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x10 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16507
// ==== 0x4414b60  ArcWebContentsConfiguration::setForeground:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setForeground_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 8) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16519
// ==== 0x4414b68  ArcWebContentsConfiguration::untracked  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::untracked(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 9);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16530
// ==== 0x4414b70  ArcWebContentsConfiguration::setUntracked:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setUntracked_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 9) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16542
// ==== 0x4414b78  ArcWebContentsConfiguration::setForceDisplayPDFInline:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setForceDisplayPDFInline_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 10) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16554
// ==== 0x4414b80  ArcWebContentsConfiguration::disallowDownloads  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::disallowDownloads(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xd);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16565
// ==== 0x4414b88  ArcWebContentsConfiguration::setDisallowDownloads:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setDisallowDownloads_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xd) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16577
// ==== 0x4414b90  ArcWebContentsConfiguration::disallowAutoplay  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::disallowAutoplay(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xe);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16588
// ==== 0x4414b98  ArcWebContentsConfiguration::setDisallowAutoplay:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setDisallowAutoplay_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xe) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16600
// ==== 0x4414ba0  ArcWebContentsConfiguration::forceAllowAutoplay  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::forceAllowAutoplay(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xf);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16611
// ==== 0x4414ba8  ArcWebContentsConfiguration::setForceAllowAutoplay:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setForceAllowAutoplay_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xf) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16623
// ==== 0x4414bb0  ArcWebContentsConfiguration::setDisallowContextMenus:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setDisallowContextMenus_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x10) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16635
// ==== 0x4414bb8  ArcWebContentsConfiguration::transparentBackground  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::transparentBackground(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0xc);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16646
// ==== 0x4414bc0  ArcWebContentsConfiguration::setTransparentBackground:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setTransparentBackground_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xc) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16658
// ==== 0x4414bc8  ArcWebContentsConfiguration::initiallyHidden  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::initiallyHidden(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x12);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16669
// ==== 0x4414bd0  ArcWebContentsConfiguration::setInitiallyHidden:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setInitiallyHidden_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x12) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16681
// ==== 0x4414be0  ArcWebContentsConfiguration::initiallyMuted  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcWebContentsConfiguration::initiallyMuted(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x11);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16692
// ==== 0x4414be8  ArcWebContentsConfiguration::setInitiallyMuted:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setInitiallyMuted_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0x11) = (char)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16704
// ==== 0x4414bf0  ArcWebContentsConfiguration::forcedViewSize  (40 bytes)

/* Function Stack Size: 0x10 bytes */

CGSize ArcWebContentsConfiguration::forcedViewSize(ID param_1,SEL param_2)

{
  CGSize CVar1;
  
  if (*(char *)(param_1 + 0x1c) != '\0') {
    CVar1.field0_0x0 = (double)*(int *)(param_1 + 0x14);
    CVar1.field1_0x8 = (double)*(int *)(param_1 + 0x18);
    return CVar1;
  }
  return *(CGSize *)PTR__CGSizeZero_0d826488;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16722
// ==== 0x4414c18  ArcWebContentsConfiguration::setForcedViewSize:  (112 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcWebContentsConfiguration::setForcedViewSize_(ID param_1,SEL param_2,CGSize param_3)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  ulong uVar4;
  undefined1 uVar5;
  double dVar6;
  double dVar7;
  
  dVar7 = param_3.field1_0x8;
  dVar6 = param_3.field0_0x0;
  uVar4 = 0;
  uVar5 = 0;
  if ((dVar6 <= 0.10000000149011612) || (dVar7 <= 0.10000000149011612)) {
LAB_04414c78:
    *(ulong *)(param_1 + 0x14) = uVar4;
    *(undefined1 *)(param_1 + 0x1c) = uVar5;
    return;
  }
  if (dVar6 <= 2147483647.0) {
    bVar2 = true;
    bVar3 = false;
    if (dVar7 <= 2147483647.0) {
      bVar2 = false;
      bVar3 = true;
      if (!NAN(dVar7)) {
        bVar2 = dVar7 < -2147483648.0;
        bVar3 = false;
      }
    }
    if (bVar2 == bVar3) {
      uVar4 = CONCAT44((int)dVar7,(int)dVar6) & 0xffffffffffffff00 | (ulong)((int)dVar6 & 0xff);
      uVar5 = 1;
      goto LAB_04414c78;
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4414c88);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16770
// ==== 0x4414c88  ArcWebContentsConfiguration::nativeWebContentsConfiguration  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcWebContentsConfiguration::nativeWebContentsConfiguration
               (WebContentsConfiguration *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar2 = *(undefined8 *)(param_1 + 0x10);
  uVar1 = *(undefined8 *)(param_1 + 8);
  __return_storage_ptr__->field2_0x8 = (int)uVar2;
  __return_storage_ptr__->field3_0xc = (int)((ulong)uVar2 >> 0x20);
  __return_storage_ptr__->field0_0x0 = (int)uVar1;
  __return_storage_ptr__->field1_0x4 = (int)((ulong)uVar1 >> 0x20);
  *(undefined8 *)((long)&__return_storage_ptr__->field3_0xc + 1) = *(undefined8 *)(param_1 + 0x15);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16792
// ==== 0x4414c9c  ArcWebContentsConfiguration::setNativeWebContentsConfiguration:  (20 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcWebContentsConfiguration::setNativeWebContentsConfiguration_
               (ID param_1,SEL param_2,WebContentsConfiguration *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar2 = *(undefined8 *)&param_3->field2_0x8;
  uVar1 = *(undefined8 *)param_3;
  *(undefined8 *)(param_1 + 0x15) = *(undefined8 *)((long)&param_3->field3_0xc + 1);
  *(undefined8 *)(param_1 + 0x10) = uVar2;
  *(undefined8 *)(param_1 + 8) = uVar1;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16812
// ==== 0x4414cb0  ArcWebContentsConfiguration::.cxx_construct  (28 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcWebContentsConfiguration::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined2 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x12) = 1;
  *(undefined1 *)(param_1 + 0x14) = 0;
  *(undefined1 *)(param_1 + 0x1c) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16828
// ==== 0x4414ccc  ArcWebContentsDevTools::init:  (292 bytes)

/* Function Stack Size: 0x20 bytes */

ID ArcWebContentsDevTools::init_(ID param_1,SEL param_2,DevTools param_3)

{
  long lVar1;
  ID IVar2;
  Impl *pIVar3;
  code *pcVar4;
  long *plVar5;
  ID local_40;
  class_t *pcStack_38;
  
  pIVar3 = param_3.field0_0x0.field0_0x0;
  pcStack_38 = &objc::class_t::ArcWebContentsDevTools;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init",pIVar3,param_3.field0_0x0.field1_0x8);
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x30) == '\0') goto LAB_04414d2c;
  plVar5 = *(long **)(IVar2 + 0x28);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar5[1];
    plVar5[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar1 = plVar5[2];
        plVar5[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04414d28;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_04414d28:
  *(undefined1 *)(IVar2 + 0x30) = 0;
LAB_04414d2c:
  *(undefined8 *)(IVar2 + 0x20) = *(undefined8 *)pIVar3;
  lVar1 = *(long *)(pIVar3 + 8);
  *(long *)(IVar2 + 0x28) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x30) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:16961
// ==== 0x4414f60  ArcWebContentsDevTools::setDelegate:  (536 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContentsDevTools::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  undefined8 uVar2;
  code *pcVar3;
  long lVar4;
  undefined8 *puVar5;
  ulong uVar6;
  long *plVar7;
  
  if (param_3 == 0) {
    plVar7 = *(long **)(param_1 + 0x18);
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    if (plVar7 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar4 = plVar7[1];
    plVar7[1] = lVar4 + -1;
    LORelease();
    if (lVar4 != 0) {
      return;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7);
    if (plVar7[2] != 0) {
      LOAcquire();
      lVar4 = plVar7[2];
      plVar7[2] = lVar4 + -1;
      LORelease();
      if (lVar4 != 0) {
        return;
      }
    }
    pcVar3 = *(code **)(*plVar7 + 0x20);
    if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
      (*pcVar3)(plVar7);
      return;
    }
    FUN_00005810(plVar7,0x20);
    return;
  }
  if (*(long *)(param_1 + 0x10) != 0) {
    *(ID *)(*(long *)(param_1 + 0x10) + 8) = param_3;
    return;
  }
  puVar5 = (undefined8 *)FUN_00008790(0x30);
  puVar5[1] = 0;
  puVar5[2] = 0;
  *puVar5 = &PTR_viewWillStartLiveResize_0da88218;
  if (puVar5 + 3 == (undefined8 *)0x0) goto LAB_04415174;
  puVar5[3] = &PTR_FUN_0da88250;
  _objc_initWeak(puVar5 + 4,param_3);
  _objc_initWeak(puVar5 + 5,param_1);
  plVar7 = *(long **)(param_1 + 0x18);
  *(undefined8 **)(param_1 + 0x10) = puVar5 + 3;
  *(undefined8 **)(param_1 + 0x18) = puVar5;
  if (plVar7 != (long *)0x0) {
    LOAcquire();
    lVar4 = plVar7[1];
    plVar7[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar4 = plVar7[2];
        plVar7[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_0441509c;
      }
      pcVar3 = *(code **)(*plVar7 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar3)(plVar7);
      }
    }
  }
LAB_0441509c:
  uVar6 = (ulong)*(byte *)(param_1 + 0x30);
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    uVar2 = *(undefined8 *)(param_1 + 0x10);
    if (*(long *)(param_1 + 0x18) != 0) {
      puVar1 = (ulong *)(*(long *)(param_1 + 0x18) + 0x10);
      uVar6 = *puVar1;
      *puVar1 = uVar6 + 1;
    }
    FUN_04332f08(uVar6,param_1 + 0x20,uVar2);
    return;
  }
LAB_04415174:
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x4415178);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17068
// ==== 0x4415178  ArcWebContentsDevTools::open  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcWebContentsDevTools::open(char *param_1,int param_2,...)

{
  code *pcVar1;
  int iVar2;
  
  if ((param_1[0x30] & 1U) != 0) {
    iVar2 = FUN_04356fdc(*(undefined8 *)(param_1 + 0x20),param_2);
    return iVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x441518c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17088
// ==== 0x441518c  ArcWebContentsDevTools::close  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcWebContentsDevTools::close(int param_1)

{
  code *pcVar1;
  int iVar2;
  
  if ((*(byte *)((ulong)(uint)param_1 + 0x30) & 1) != 0) {
    iVar2 = FUN_04357fd0(*(undefined8 *)((ulong)(uint)param_1 + 0x20));
    return iVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44151a0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17344
// ==== 0x4415520  ArcWebContentsDevTools::execute:completion:error:  (712 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

bool ArcWebContentsDevTools::execute_completion_error_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5,ID *param_6)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  ID IVar4;
  uint *puVar5;
  uint *local_c0;
  uint *local_b8;
  ID *local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  ID *local_98;
  undefined8 uStack_90;
  ulong uStack_88;
  ID *local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68;
  undefined8 uStack_60;
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  local_70 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_78 = 0;
  local_80 = (ID *)0x0;
  uStack_a8 = 0;
  local_b0 = (ID *)0x0;
  local_98 = (ID *)0x0;
  local_a0 = 0;
  IVar4 = _objc_msgSend(param_3,"domain");
  if (IVar4 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar4);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  uStack_a8 = uStack_60;
  local_b0 = local_68;
  local_a0 = (ulong)local_58;
  IVar4 = _objc_msgSend(param_3,"name");
  if (IVar4 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar4);
  }
  if ((long)uStack_88 < 0) {
    FUN_00005810(local_98,uStack_88 & 0x7fffffffffffffff);
  }
  uStack_90 = uStack_60;
  local_98 = local_68;
  uStack_88 = (ulong)local_58;
  IVar4 = _objc_msgSend(param_3,"params");
  if (IVar4 != 0) {
    IVar4 = _objc_msgSend(param_3,"params");
    if (IVar4 == 0) {
      local_68 = (ID *)0x0;
      uStack_60 = 0;
      local_58 = (undefined1 *)0x0;
    }
    else {
      FUN_01b356b8(&local_68,IVar4);
    }
    if ((long)local_70 < 0) {
      FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
    }
    uStack_78 = uStack_60;
    local_80 = local_68;
    local_70 = (ulong)local_58;
  }
  local_b8 = (uint *)0x0;
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    local_68 = (ID *)FUN_00008790(8);
    *local_68 = param_4;
    local_58 = &LAB_044162ac;
    local_50 = &PTR_FUN_0df6bc48;
    FUN_0435b114(&local_b8,*(undefined8 *)(param_1 + 0x20),&local_b0,&local_68);
    if ((code *)local_50[1] != (code *)0x0) {
      (*(code *)local_50[1])(local_68);
    }
    local_c0 = local_b8;
    puVar5 = local_b8;
    if (((ulong)local_b8 & 1) == 0) {
      uVar2 = *local_b8;
      *local_b8 = uVar2 + 1;
      puVar5 = (uint *)(ulong)uVar2;
    }
    bVar3 = FUN_04402548(puVar5,&local_c0,param_5);
    if (((ulong)local_c0 & 1) == 0) {
      FUN_04b35898();
    }
    if (((ulong)local_b8 & 1) == 0) {
      FUN_04b35898();
    }
    FUN_04052260(&local_b0);
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
      return bVar3;
    }
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44157e4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17474
// ==== 0x4415838  ArcWebContentsDevTools::subscribe:autoEnable:notification:  (560 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x24 bytes */

ID ArcWebContentsDevTools::subscribe_autoEnable_notification_
             (ID param_1,SEL param_2,ID param_3,bool param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  ID IVar2;
  ID *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  ulong local_98;
  ulong local_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68;
  undefined8 uStack_60;
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4415a64);
    (*pcVar1)();
  }
  local_90 = 0;
  local_88 = 0;
  IVar2 = _objc_msgSend(param_3,"domain");
  if (IVar2 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar2);
  }
  IVar2 = _objc_msgSend(param_3,"name");
  if (IVar2 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,IVar2);
  }
  uStack_b8 = uStack_60;
  local_c0 = local_68;
  local_b0 = (ulong)local_58;
  uStack_a0 = uStack_78;
  local_a8 = local_80;
  local_98 = local_70;
  local_68 = (ID *)FUN_00008790(8);
  *local_68 = param_5;
  local_58 = &LAB_04416474;
  local_50 = &PTR_FUN_0df6bc68;
  FUN_0435b784(&local_90,*(undefined8 *)(param_1 + 0x20),&local_c0,param_4,&local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68);
  }
  if ((long)local_98 < 0) {
    FUN_00005810(local_a8,local_98 & 0x7fffffffffffffff);
  }
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if (local_90 == 1) {
    IVar2 = NSNumber::numberWithInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",local_88 & 0xffffffff);
  }
  else {
    IVar2 = 0;
  }
  if ((local_90 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return IVar2;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17571
// ==== 0x4415ad8  ArcWebContentsDevTools::unsubscribe:  (136 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcWebContentsDevTools::unsubscribe_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  bool bVar3;
  ulong local_28;
  
  if (param_3 == 0) {
    bVar3 = 0;
  }
  else {
    if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4415b60);
      (*pcVar1)();
    }
    IVar2 = _objc_msgSend(param_3,"intValue");
    FUN_0435be6c(&local_28,*(undefined8 *)(param_1 + 0x20),IVar2);
    bVar3 = (bool)(local_28 == 1);
    if ((local_28 & 1) == 0) {
      FUN_04b35898();
    }
  }
  return bVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17605
// ==== 0x4415b60  ArcWebContentsDevTools::openProtocolConnection:error:  (344 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcWebContentsDevTools::openProtocolConnection_error_
             (ID param_1,SEL param_2,ID param_3,undefined4 param_4,ID *param_5)

{
  code *pcVar1;
  uint uVar2;
  ulong uVar3;
  ID IVar4;
  uint *puVar5;
  uint *local_70;
  uint *local_68;
  ulong local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4415cac);
    (*pcVar1)();
  }
  local_58[0] = (ID *)FUN_00008790(8);
  local_68 = (uint *)0x0;
  local_60 = 0;
  *local_58[0] = param_3;
  local_48 = &LAB_04416690;
  local_40 = &PTR_FUN_0df6bc88;
  FUN_04355df8(&local_68,*(undefined8 *)(param_1 + 0x20),local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  local_70 = local_68;
  puVar5 = local_68;
  if (((ulong)local_68 & 1) == 0) {
    uVar2 = *local_68;
    *local_68 = uVar2 + 1;
    puVar5 = (uint *)(ulong)uVar2;
  }
  uVar3 = FUN_04402548(puVar5,&local_70,param_4);
  if (((ulong)local_70 & 1) == 0) {
    FUN_04b35898();
  }
  if ((uVar3 & 1) == 0) {
    IVar4 = 0;
  }
  else {
    if (local_68 != (uint *)((long)&MACH_HEADER.magic + 1)) {
                    /* WARNING: Subroutine does not return */
      FUN_04b37e18(&local_68);
    }
    IVar4 = NSNumber::numberWithInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",local_60 & 0xffffffff);
  }
  if (((ulong)local_68 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return IVar4;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17681
// ==== 0x4415cf8  ArcWebContentsDevTools::sendProtocolMessage:connectionID:error:  (248 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

bool ArcWebContentsDevTools::sendProtocolMessage_connectionID_error_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID *param_5)

{
  code *pcVar1;
  bool bVar2;
  ID IVar3;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  ulong local_48;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    IVar3 = _objc_msgSend(param_4,"intValue");
    if (param_3 == 0) {
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
    }
    else {
      FUN_01b356b8(&local_60,param_3);
    }
    FUN_043564d8(&local_48,*(undefined8 *)(param_1 + 0x20),IVar3,&local_60);
    bVar2 = FUN_04402548(&local_48,param_5);
    if ((local_48 & 1) == 0) {
      FUN_04b35898();
    }
    if ((long)local_50 < 0) {
      FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
    }
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4415df0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17725
// ==== 0x4415e20  ArcWebContentsDevTools::closeProtocolConnection:  (84 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContentsDevTools::closeProtocolConnection_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    IVar2 = _objc_msgSend(param_3,"intValue");
    FUN_04356754(*(undefined8 *)(param_1 + 0x20),IVar2);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4415e74);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17748
// ==== 0x4415e74  ArcWebContentsDevTools::.cxx_destruct  (364 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcWebContentsDevTools::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x30) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x28), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_04415ea8;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_04415ea8:
  plVar3 = *(long **)(param_1 + 0x18);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04415ec0;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04415ec0:
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17889
// ==== 0x4416808  ArcNavigation::init:  (480 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcNavigation::init_(ID param_1,SEL param_2,Navigation param_3)

{
  long lVar1;
  ID IVar2;
  ID self;
  undefined8 uVar3;
  undefined8 uVar4;
  Impl *pIVar5;
  code *pcVar6;
  long *plVar7;
  long lVar8;
  ID local_50;
  class_t *pcStack_48;
  
  pIVar5 = param_3.field0_0x0.field0_0x0;
  pcStack_48 = &objc::class_t::ArcNavigation;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init",pIVar5,param_3.field0_0x0.field1_0x8);
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0441686c;
  plVar7 = *(long **)(IVar2 + 0x18);
  if (plVar7 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar7[1];
    plVar7[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar1 = plVar7[2];
        plVar7[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04416868;
      }
      pcVar6 = *(code **)(*plVar7 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar6)(plVar7);
      }
    }
  }
LAB_04416868:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0441686c:
  lVar8 = *(long *)pIVar5;
  *(long *)(IVar2 + 0x10) = lVar8;
  lVar1 = *(long *)(pIVar5 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
    lVar8 = *(long *)(IVar2 + 0x10);
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  if (*(long *)(lVar8 + 0x38) != 0) {
    self = NSMutableDictionary::dictionaryWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionaryWithCapacity:");
    for (plVar7 = *(long **)(lVar8 + 0x30); plVar7 != (long *)0x0; plVar7 = (long *)*plVar7) {
      lVar1 = (long)*(char *)((long)plVar7 + 0x3f);
      if (lVar1 < 0) {
        lVar8 = plVar7[5];
        lVar1 = plVar7[6];
      }
      else {
        lVar8 = (long)(plVar7 + 5);
      }
      uVar3 = FUN_01c391bc(lVar8,lVar1);
      lVar1 = (long)*(char *)((long)plVar7 + 0x27);
      if (lVar1 < 0) {
        lVar8 = plVar7[2];
        lVar1 = plVar7[3];
      }
      else {
        lVar8 = (long)(plVar7 + 2);
      }
      uVar4 = FUN_01c391bc(lVar8,lVar1);
      NSMutableDictionary::setObject_forKeyedSubscript_
                (self,"setObject:forKeyedSubscript:",uVar3,uVar4);
    }
    *(ID *)(IVar2 + 8) = self;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:17986
// ==== 0x44169e8  ArcNavigation::isEqual:  (368 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcNavigation::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  bool bVar4;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  if (param_3 != 0) {
    IVar2 = _objc_msgSend((ID)&objc::class_t::ArcNavigation,"class");
    IVar2 = _objc_msgSend(param_3,"isKindOfClass:",IVar2);
    if ((int)IVar2 != 0) {
      _objc_msgSend(param_3,"nativeNavigation",&local_48);
      if ((local_38 & *(byte *)(param_1 + 0x20)) == 0) {
        bVar4 = (bool)(*(byte *)(param_1 + 0x20) == local_38);
        if (local_38 == 0) {
          return bVar4;
        }
      }
      else {
        if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x4416b58);
          (*pcVar3)();
        }
        bVar4 = (bool)(*(long *)(param_1 + 0x10) == local_48);
      }
      if (local_40 == (long *)0x0) {
        return bVar4;
      }
      LOAcquire();
      lVar1 = local_40[1];
      local_40[1] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return bVar4;
      }
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return bVar4;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
        (*pcVar3)(local_40);
        return bVar4;
      }
      FUN_00005810(local_40,0x20);
      return bVar4;
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:18058
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



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:18078
// ==== 0x4416b90  ArcNavigation::url  (44 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcNavigation::url(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  undefined8 *puVar6;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    puVar6 = *(undefined8 **)(param_1 + 0x10);
    cVar3 = *(char *)((long)puVar6 + 0x17);
    puVar1 = (undefined8 *)*puVar6;
    if (-1 < (long)cVar3) {
      puVar1 = puVar6;
    }
    lVar2 = puVar6[1];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(puVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4416bbc);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:18112
// ==== 0x4416bbc  ArcNavigation::didRedirectToOtherTab  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcNavigation::didRedirectToOtherTab(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x10) + 0x18);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4416bd4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:18130
// ==== 0x4416bd4  ArcNavigation::responseStatusCode  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcNavigation::responseStatusCode(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x48);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4416bec);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35080
// ==== 0x442c254  ArcCommandBarRankingTensor::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingTensor::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x50) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35093
// ==== 0x442c260  ArcAIInferenceModel::mutableAIInferenceModel  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcAIInferenceModel::mutableAIInferenceModel(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35209
// ==== 0x442c440  ArcCommandBarRankingEvaluation::numExamplesEvaluated  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingEvaluation::numExamplesEvaluated(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(int *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442c454);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35227
// ==== 0x442c454  ArcCommandBarRankingModel::command_bar_ranking_model  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingModel::command_bar_ranking_model
               (optional<arc::CommandBarRankingModel> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingModel> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442c4f0);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    (__return_storage_ptr__->field0_0x0).field1.field1_0x18 = *(int *)(param_1 + 0x20);
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35266
// ==== 0x442c4f0  ArcCookie::mutableCookie  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCookie::mutableCookie(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35281
// ==== 0x442c50c  ArcSearchEngine::init:keyword:url:faviconUrl:  (828 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x30 bytes */

ID ArcSearchEngine::init_keyword_url_faviconUrl_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  undefined8 local_d0;
  undefined8 uStack_c8;
  ulong local_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  ulong local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  long local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  long local_78;
  undefined8 local_70;
  long *plStack_68;
  ID local_60;
  class_t *pcStack_58;
  
  pcStack_58 = &objc::class_t::ArcSearchEngine;
  local_60 = param_1;
  IVar2 = NSObject::init((ID)&local_60,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (param_3 == 0) {
    local_88 = 0;
    uStack_80 = 0;
    local_78 = 0;
  }
  else {
    FUN_083b1228(&local_88,param_3);
  }
  if (param_4 == 0) {
    local_a0 = 0;
    uStack_98 = 0;
    local_90 = 0;
  }
  else {
    FUN_083b1228(&local_a0,param_4);
  }
  if (param_5 == 0) {
    local_b8 = 0;
    uStack_b0 = 0;
    local_a8 = 0;
  }
  else {
    FUN_01b356b8(&local_b8,param_5);
  }
  if (param_6 == 0) {
    local_d0 = 0;
    uStack_c8 = 0;
    local_c0 = 0;
  }
  else {
    FUN_01b356b8(&local_d0,param_6);
  }
  FUN_0433ffd8(&local_70,&local_88,&local_a0,&local_b8,&local_d0);
  if (*(char *)(IVar2 + 0x18) != '\0') {
    plVar4 = *(long **)(IVar2 + 0x10);
    if (plVar4 != (long *)0x0) {
      LOAcquire();
      lVar1 = plVar4[1];
      plVar4[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*plVar4 + 0x10))(plVar4);
        if (plVar4[2] != 0) {
          LOAcquire();
          lVar1 = plVar4[2];
          plVar4[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) goto LAB_0442c668;
        }
        pcVar3 = *(code **)(*plVar4 + 0x20);
        if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
          FUN_00005810(plVar4,0x20);
        }
        else {
          (*pcVar3)(plVar4);
        }
      }
    }
LAB_0442c668:
    *(undefined1 *)(IVar2 + 0x18) = 0;
  }
  *(long **)(IVar2 + 0x10) = plStack_68;
  *(undefined8 *)(IVar2 + 8) = local_70;
  if (plStack_68 == (long *)0x0) {
    *(undefined1 *)(IVar2 + 0x18) = 1;
  }
  else {
    plVar4 = plStack_68 + 1;
    *plVar4 = *plVar4 + 1;
    *(undefined1 *)(IVar2 + 0x18) = 1;
    LOAcquire();
    lVar1 = *plVar4;
    *plVar4 = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plStack_68 + 0x10))(plStack_68);
      if (plStack_68[2] != 0) {
        LOAcquire();
        lVar1 = plStack_68[2];
        plStack_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0442c6f8;
      }
      pcVar3 = *(code **)(*plStack_68 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plStack_68,0x20);
      }
      else {
        (*pcVar3)(plStack_68);
      }
    }
  }
LAB_0442c6f8:
  if ((long)local_c0 < 0) {
    FUN_00005810(local_d0,local_c0 & 0x7fffffffffffffff);
  }
  if ((long)local_a8 < 0) {
    FUN_00005810(local_b8,local_a8 & 0x7fffffffffffffff);
  }
  if (local_90 < 0) {
    FUN_00005810(local_a0,local_90 << 1);
  }
  if (local_78 < 0) {
    FUN_00005810(local_88,local_78 << 1);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35427
// ==== 0x442c8ac  ArcSearchEngine::initWithSearchEngineTemplateURL:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSearchEngine::initWithSearchEngineTemplateURL_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcSearchEngine;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0442c90c;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0442c908;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0442c908:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0442c90c:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35486
// ==== 0x442c9d0  ArcSearchEngine::name  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSearchEngine::name(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x3cf) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x3b8),*(undefined8 *)(lVar4 + 0x3c0));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x3c0);
      local_40 = *(undefined1 **)(lVar4 + 0x3b8);
      local_30 = *(long *)(lVar4 + 0x3c8);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442ca74);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35534
// ==== 0x442ca98  ArcSearchEngine::keyword  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSearchEngine::keyword(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 999) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x3d0),*(undefined8 *)(lVar4 + 0x3d8));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x3d8);
      local_40 = *(undefined1 **)(lVar4 + 0x3d0);
      local_30 = *(long *)(lVar4 + 0x3e0);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442cb3c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35582
// ==== 0x442cb60  ArcSearchEngine::url  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSearchEngine::url(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x3ff) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 1000),*(undefined8 *)(lVar4 + 0x3f0));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x3f0);
      local_40 = *(undefined1 **)(lVar4 + 1000);
      local_30 = *(ulong *)(lVar4 + 0x3f8);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442cc04);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35630
// ==== 0x442cc28  ArcSearchEngine::faviconUrl  (184 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSearchEngine::faviconUrl(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ID IVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    if (*(char *)(*(long *)(param_1 + 8) + 0x228) == '\0') {
      local_40 = (undefined1 *)0x0;
      lStack_38 = 0;
      local_30 = 0;
    }
    else {
      puVar4 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 8) + 0x210);
      if (*(char *)((long)puVar4 + 0x17) < '\0') {
        FUN_00060a60(&local_40,*puVar4,puVar4[1]);
      }
      else {
        lStack_38 = puVar4[1];
        local_40 = (undefined1 *)*puVar4;
        local_30 = puVar4[2];
      }
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar5 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x442cce0);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35686
// ==== 0x442cd04  ArcSearchEngine::status  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcSearchEngine::status(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 8) + 0x3ac);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442cd1c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35704
// ==== 0x442cd1c  ArcSearchEngine::isPrepopulated  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSearchEngine::isPrepopulated(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)(0 < *(int *)(*(long *)(param_1 + 8) + 0x354));
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442cd3c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:35935
// ==== 0x442cf84  ArcExtensionRuntimeErrorStackFrame::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionRuntimeErrorStackFrame::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x40) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36036
// ==== 0x442d10c  ArcAutocompleteMatchAdditionalData::key  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatchAdditionalData::key(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442d13c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36088
// ==== 0x442d150  ArcAutocompleteMatchAdditionalData::autocomplete_match_additional_data  (144 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAutocompleteMatchAdditionalData::autocomplete_match_additional_data
               (optional<arc::AutocompleteMatchAdditionalData> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 0;
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AutocompleteMatchAdditionalData> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442d244);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00060a60((undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18),
                   *(undefined8 *)(param_1 + 0x20),*(undefined8 *)(param_1 + 0x28));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 0x20);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x20) =
           *(undefined8 *)(param_1 + 0x28);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x28) =
           *(undefined8 *)(param_1 + 0x30);
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36138
// ==== 0x442d244  ArcAutocompleteMatchAdditionalData::.cxx_destruct  (92 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAutocompleteMatchAdditionalData::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(ulong *)(param_1 + 0x30) & 0x7fffffffffffffff);
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36158
// ==== 0x442d2a0  ArcAutocompleteMatchAdditionalData::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatchAdditionalData::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x38) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36171
// ==== 0x442d2ac  ArcNomicEmbedTextModel::init:version:  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcNomicEmbedTextModel::init_version_(ID param_1,SEL param_2,ID param_3,int param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcNomicEmbedTextModel;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x1f) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    if (param_3 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_3);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
LAB_0442d3c4:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442d3c8);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
      if ((*(byte *)(IVar2 + 0x28) & 1) == 0) goto LAB_0442d3c4;
    }
    else {
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
    }
    *(int *)(IVar2 + 0x20) = param_4;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36232
// ==== 0x442d3c8  ArcNomicEmbedTextModel::initWithNomicEmbedTextModel:  (168 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcNomicEmbedTextModel::initWithNomicEmbedTextModel_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcNomicEmbedTextModel;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x28) != '\0') {
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x28) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    *(undefined4 *)(IVar1 + 0x20) = *(undefined4 *)((long)param_3 + 0x18);
    *(undefined1 *)(IVar1 + 0x28) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36311
// ==== 0x442d54c  ArcAutocompleteMatchAdditionalData::mutableAutocompleteMatchAdditionalData  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcAutocompleteMatchAdditionalData::mutableAutocompleteMatchAdditionalData
                 (ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36327
// ==== 0x442d568  ArcNomicEmbedTextItem::init:task:  (400 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcNomicEmbedTextItem::init_task_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcNomicEmbedTextItem;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x38) != '\0') {
    if (*(char *)(IVar2 + 0x37) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
    }
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
  }
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 8) = 0;
  *(undefined1 *)(IVar2 + 0x38) = 1;
  if (param_3 == 0) {
    local_58 = 0;
    uStack_50 = 0;
    local_48 = 0;
  }
  else {
    FUN_01b356b8(&local_58,param_3);
  }
  if ((*(byte *)(IVar2 + 0x38) & 1) != 0) {
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x10) = uStack_50;
    *(undefined8 *)(IVar2 + 8) = local_58;
    *(undefined8 *)(IVar2 + 0x18) = local_48;
    if (param_4 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_4);
    }
    if ((*(byte *)(IVar2 + 0x38) & 1) != 0) {
      if (*(char *)(IVar2 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
      }
      *(undefined8 *)(IVar2 + 0x28) = uStack_50;
      *(undefined8 *)(IVar2 + 0x20) = local_58;
      *(undefined8 *)(IVar2 + 0x30) = local_48;
      return IVar2;
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442d6f8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36404
// ==== 0x442d6f8  ArcNomicEmbedTextItem::initWithNomicEmbedTextItem:  (224 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcNomicEmbedTextItem::initWithNomicEmbedTextItem_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcNomicEmbedTextItem;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x38) != '\0') {
      if (*(char *)(IVar1 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x20),*(ulong *)(IVar1 + 0x30) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x38) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    if (*(char *)((long)param_3 + 0x2f) < '\0') {
      FUN_00060a60(IVar1 + 0x20,*(undefined8 *)((long)param_3 + 0x18),
                   *(undefined8 *)((long)param_3 + 0x20));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x20);
      uVar2 = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x30) = *(undefined8 *)((long)param_3 + 0x28);
      *(undefined8 *)(IVar1 + 0x28) = uVar3;
      *(undefined8 *)(IVar1 + 0x20) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36457
// ==== 0x442d7fc  ArcAutocompleteMatchAdditionalData::value  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatchAdditionalData::value(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x37);
    plVar1 = (long *)*(long *)(param_1 + 0x20);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x20);
    }
    lVar2 = *(long *)(param_1 + 0x28);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442d82c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36542
// ==== 0x442d974  ArcMostVisitedURL::initWithMostVisitedSite:  (224 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcMostVisitedURL::initWithMostVisitedSite_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcMostVisitedURL;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x38) != '\0') {
      if (*(char *)(IVar1 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x20),*(long *)(IVar1 + 0x30) << 1);
      }
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x38) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    if (*(char *)((long)param_3 + 0x2f) < '\0') {
      FUN_00054420(IVar1 + 0x20,*(undefined8 *)((long)param_3 + 0x18),
                   *(undefined8 *)((long)param_3 + 0x20));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x20);
      uVar2 = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x30) = *(undefined8 *)((long)param_3 + 0x28);
      *(undefined8 *)(IVar1 + 0x28) = uVar3;
      *(undefined8 *)(IVar1 + 0x20) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36595
// ==== 0x442da54  ArcMostVisitedURL::url  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcMostVisitedURL::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 8);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442da68);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36615
// ==== 0x442da68  ArcMostVisitedURL::title  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcMostVisitedURL::title(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x37);
    plVar1 = (long *)*(long *)(param_1 + 0x20);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x20);
    }
    lVar2 = *(long *)(param_1 + 0x28);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01d9fb90(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442da98);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36647
// ==== 0x442da98  ArcMostVisitedURL::most_visited_site  (144 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMostVisitedURL::most_visited_site
               (optional<arc::MostVisitedURL> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 0;
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::MostVisitedURL> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442db28);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00054420((undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18),
                   *(undefined8 *)(param_1 + 0x20),*(undefined8 *)(param_1 + 0x28));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 0x20);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x20) =
           *(undefined8 *)(param_1 + 0x28);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x28) =
           *(undefined8 *)(param_1 + 0x30);
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x30 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36696
// ==== 0x442db28  ArcMostVisitedURL::.cxx_destruct  (92 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcMostVisitedURL::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x38) != '\0') {
    if (*(char *)(param_1 + 0x37) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x20),*(long *)(param_1 + 0x30) << 1);
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 8),*(ulong *)(param_1 + 0x18) & 0x7fffffffffffffff);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36716
// ==== 0x442db84  ArcSiteEngagementDetails::initWithSiteEngagementDetails:  (168 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSiteEngagementDetails::initWithSiteEngagementDetails_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcSiteEngagementDetails;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x28) != '\0') {
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x28) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    *(undefined8 *)(IVar1 + 0x20) = *(undefined8 *)((long)param_3 + 0x18);
    *(undefined1 *)(IVar1 + 0x28) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36757
// ==== 0x442dc2c  ArcSiteEngagementDetails::url  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSiteEngagementDetails::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 8);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442dc40);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36777
// ==== 0x442dc40  ArcSiteEngagementDetails::score  (20 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcSiteEngagementDetails::score(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(double *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442dc54);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36795
// ==== 0x442dc54  ArcSiteEngagementDetails::site_engagement_details  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcSiteEngagementDetails::site_engagement_details
               (optional<arc::SiteEngagementDetails> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x20 = 0;
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::SiteEngagementDetails> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442dcf0);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    (__return_storage_ptr__->field0_0x0).field1.field1_0x18 = *(double *)(param_1 + 0x20);
    *(undefined1 *)&__return_storage_ptr__->field1_0x20 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:36955
// ==== 0x442dfd8  ArcHistoryItem::initWithHistoryItem:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcHistoryItem::initWithHistoryItem_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcHistoryItem;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0442e038;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0442e034;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0442e034:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0442e038:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37014
// ==== 0x442e0fc  ArcHistoryItem::historyID  (128 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryItem::historyID(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_00c248b4(&local_38,*(undefined8 *)(*(long *)(param_1 + 8) + 8));
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442e17c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37053
// ==== 0x442e1a0  ArcContextMenuElementInfo::linkURL  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcContextMenuElementInfo::linkURL(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  ID IVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar2 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 8) + 0x10);
    if (*(char *)((long)puVar2 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar2,puVar2[1]);
    }
    else {
      uStack_38 = puVar2[1];
      local_40 = *puVar2;
      local_30 = puVar2[2];
    }
    IVar3 = FUN_043abd34(&local_40);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e22c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37092
// ==== 0x442e250  ArcHistoryItem::urlSpec  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryItem::urlSpec(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ID IVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar4 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 8) + 0x10);
    if (*(char *)((long)puVar4 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar4,puVar4[1]);
    }
    else {
      lStack_38 = puVar4[1];
      local_40 = (undefined1 *)*puVar4;
      local_30 = puVar4[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar5 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x442e2f4);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37141
// ==== 0x442e318  ArcHistoryItem::title  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryItem::title(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x9f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x88),*(undefined8 *)(lVar4 + 0x90));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x90);
      local_40 = *(undefined1 **)(lVar4 + 0x88);
      local_30 = *(long *)(lVar4 + 0x98);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442e3b4);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37189
// ==== 0x442e3d8  ArcHistoryItem::visitCount  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcHistoryItem::visitCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 8) + 0xa0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e3f0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37207
// ==== 0x442e3f0  ArcHistoryItem::typedCount  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcHistoryItem::typedCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 8) + 0xa4);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e408);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37225
// ==== 0x442e408  ArcHistoryItem::lastVisitTime  (160 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcHistoryItem::lastVisitTime(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  double dVar5;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x442e4a8);
    (*pcVar2)();
  }
  lVar4 = *(long *)(*(long *)(param_1 + 8) + 0xa8);
  if (lVar4 == 0) {
    dVar5 = 0.0;
  }
  else if (lVar4 + 0x8000000000000001U < 2) {
    dVar5 = -INFINITY;
    if (-1 < lVar4) {
      dVar5 = INFINITY;
    }
  }
  else {
    lVar1 = -0x8000000000000000;
    if (!SCARRY8(lVar4,-0x295e9648864000)) {
      lVar1 = lVar4 + -0x295e9648864000;
    }
    if (lVar1 == -0x8000000000000000) {
      dVar5 = -INFINITY;
    }
    else {
      dVar5 = (double)lVar1 / 1000000.0;
    }
  }
  IVar3 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",dVar5);
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37271
// ==== 0x442e4a8  ArcHistoryItem::visitTime  (76 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryItem::visitTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442e4f4);
    (*pcVar1)();
  }
  if ((*(uint *)(*(long *)(param_1 + 8) + 0xc0) & 1) == 0) {
    return 0;
  }
  IVar2 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",
                     *(undefined8 *)(*(long *)(param_1 + 8) + 0xb8));
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37667
// ==== 0x442e9d4  ArcExtractedPageContent::initWithExtractedPageContent:  (164 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtractedPageContent::initWithExtractedPageContent_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtractedPageContent;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x1e8) != '\0') {
      FUN_04295208(IVar1 + 0x48);
      if (*(char *)(IVar1 + 0x3f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x28),*(ulong *)(IVar1 + 0x38) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar1 + 0x27) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x10),*(ulong *)(IVar1 + 0x20) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x1e8) = 0;
    }
    FUN_04296004(IVar1 + 0x10,param_3);
    *(undefined1 *)(IVar1 + 0x1e8) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37700
// ==== 0x442ea78  ArcExtractedPageContent::url  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtractedPageContent::url(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x1e8) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x27);
    plVar1 = (long *)*(long *)(param_1 + 0x10);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x10);
    }
    lVar2 = *(long *)(param_1 + 0x18);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442eaa8);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37732
// ==== 0x442eaa8  ArcExtractedPageContent::title  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtractedPageContent::title(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x1e8) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x3f);
    plVar1 = (long *)*(long *)(param_1 + 0x28);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x28);
    }
    lVar2 = *(long *)(param_1 + 0x30);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442ead8);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37764
// ==== 0x442ead8  ArcExtractedPageContent::rootNode  (96 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtractedPageContent::rootNode(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  IVar2 = *(ID *)(param_1 + 8);
  if (IVar2 == 0) {
    IVar2 = _objc_alloc(&objc::class_t::ArcContentNode);
    if ((*(byte *)(param_1 + 0x1e8) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442eb38);
      (*pcVar1)();
    }
    IVar2 = ArcContentNode::initWithContentNode_
                      (IVar2,"initWithContentNode:",(const_void__)(param_1 + 0x48));
    *(ID *)(param_1 + 8) = IVar2;
    IVar2 = *(ID *)(param_1 + 8);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37794
// ==== 0x442eb38  ArcExtractedPageContent::extractionTime  (20 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcExtractedPageContent::extractionTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x1e8) & 1) != 0) {
    return *(double *)(param_1 + 0x40);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442eb4c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37812
// ==== 0x442eb4c  ArcExtractedPageContent::extracted_page_content  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtractedPageContent::extracted_page_content
               (optional<arc::ExtractedPageContent> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 0x30) = 0;
  if (*(char *)(param_1 + 0x1e8) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ExtractedPageContent> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442eb94);
      (*pcVar1)();
    }
    FUN_04296004(__return_storage_ptr__,param_1 + 0x10);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 0x30) = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37837
// ==== 0x442ebe0  ArcExtractedPageContent::.cxx_destruct  (100 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcExtractedPageContent::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x1e8) != '\0') {
    FUN_04295208(param_1 + 0x48);
    if (*(char *)(param_1 + 0x3f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
    }
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(ulong *)(param_1 + 0x20) & 0x7fffffffffffffff);
    }
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:37859
// ==== 0x442ec44  ArcExtractedPageContent::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtractedPageContent::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 0x10) = 0;
  *(undefined1 *)(param_1 + 0x1e8) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38379
// ==== 0x442f158  ArcExtensionInfo::mutableExtensionInfo  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcExtensionInfo::mutableExtensionInfo(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38394
// ==== 0x442f174  ArcExtensionInfo::init  (164 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionInfo;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xe0) != '\0') {
      FUN_04334d2c(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0xe0) = 0;
    }
    *(undefined8 *)(IVar1 + 0x78) = 0;
    *(undefined8 *)(IVar1 + 0x70) = 0;
    *(undefined8 *)(IVar1 + 0x68) = 0;
    *(undefined8 *)(IVar1 + 0x60) = 0;
    *(undefined8 *)(IVar1 + 0x58) = 0;
    *(undefined8 *)(IVar1 + 0x50) = 0;
    *(undefined8 *)(IVar1 + 0x48) = 0;
    *(undefined8 *)(IVar1 + 0x40) = 0;
    *(undefined8 *)(IVar1 + 0x38) = 0;
    *(undefined8 *)(IVar1 + 0x30) = 0;
    *(undefined8 *)(IVar1 + 0x28) = 0;
    *(undefined8 *)(IVar1 + 0x20) = 0;
    *(undefined8 *)(IVar1 + 0x18) = 0;
    *(undefined8 *)(IVar1 + 0x10) = 0;
    *(undefined8 *)(IVar1 + 8) = 0;
    FUN_043386a0(IVar1 + 0x80);
    *(undefined1 *)(IVar1 + 0xd8) = 0;
    *(undefined8 *)(IVar1 + 0xd0) = 0;
    *(undefined8 *)(IVar1 + 200) = 0;
    *(undefined8 *)(IVar1 + 0xc0) = 0;
    *(undefined8 *)(IVar1 + 0xb8) = 0;
    *(undefined8 *)(IVar1 + 0xb0) = 0;
    *(undefined8 *)(IVar1 + 0xa8) = 0;
    *(undefined1 *)(IVar1 + 0xe0) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38442
// ==== 0x442f218  ArcExtensionInfo::initWithExtensionInfo:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtensionInfo::initWithExtensionInfo_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionInfo;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xe0) != '\0') {
      FUN_04334d2c(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0xe0) = 0;
    }
    FUN_04334f1c(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0xe0) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38468
// ==== 0x442f28c  ArcExtensionInfo::name  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::name(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f2bc);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38500
// ==== 0x442f2bc  ArcExtensionInfo::id  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::id(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x37);
    plVar1 = (long *)*(long *)(param_1 + 0x20);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x20);
    }
    lVar2 = *(long *)(param_1 + 0x28);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f2ec);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38532
// ==== 0x442f2ec  ArcExtensionInfo::description  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::description(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x67);
    plVar1 = (long *)*(long *)(param_1 + 0x50);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x50);
    }
    lVar2 = *(long *)(param_1 + 0x58);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f31c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38564
// ==== 0x442f31c  ArcExtensionInfo::url  (20 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::url(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    IVar2 = FUN_043abd34(param_1 + 0x38);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f330);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38584
// ==== 0x442f330  ArcExtensionInfo::permissions  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::permissions(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID self;
  undefined8 uVar5;
  long lVar6;
  undefined8 *puVar7;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x70));
    lVar6 = *(long *)(param_1 + 0x70) * 0x18;
    if (lVar6 != 0) {
      puVar7 = *(undefined8 **)(param_1 + 0x68);
      do {
        cVar3 = *(char *)((long)puVar7 + 0x17);
        puVar1 = (undefined8 *)*puVar7;
        if (-1 < (long)cVar3) {
          puVar1 = puVar7;
        }
        lVar2 = puVar7[1];
        if (-1 < cVar3) {
          lVar2 = (long)cVar3;
        }
        uVar5 = FUN_01d9fb90(puVar1,lVar2);
        NSMutableArray::addObject_(self,"addObject:",uVar5);
        puVar7 = puVar7 + 3;
        lVar6 = lVar6 + -0x18;
      } while (lVar6 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f3f0);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38635
// ==== 0x442f3f0  ArcExtensionInfo::icon  (44 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::icon(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f41c);
    (*pcVar1)();
  }
  if ((**(long **)(param_1 + 0x88) != 0) && (*(long *)(**(long **)(param_1 + 0x88) + 0x18) != 0)) {
    IVar2 = FUN_02a02ccc();
    return IVar2;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38658
// ==== 0x442f41c  ArcExtensionInfo::rating  (20 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcExtensionInfo::rating(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return *(double *)(param_1 + 0x98);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f430);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38676
// ==== 0x442f430  ArcExtensionInfo::numRatings  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcExtensionInfo::numRatings(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return (long)*(int *)(param_1 + 0xa0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f444);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38694
// ==== 0x442f444  ArcExtensionInfo::enabled  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionInfo::enabled(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0xa4);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f458);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38712
// ==== 0x442f458  ArcExtensionInfo::hasAction  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionInfo::hasAction(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0xa5);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f46c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38730
// ==== 0x442f46c  ArcExtensionInfo::version  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::version(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0xbf);
    plVar1 = (long *)*(long *)(param_1 + 0xa8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0xa8);
    }
    lVar2 = *(long *)(param_1 + 0xb0);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f49c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38762
// ==== 0x442f49c  ArcExtensionInfo::omniboxKeyword  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::omniboxKeyword(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0xd7);
    plVar1 = (long *)*(long *)(param_1 + 0xc0);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0xc0);
    }
    lVar2 = *(long *)(param_1 + 200);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f4cc);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38794
// ==== 0x442f4cc  ArcExtensionInfo::fromWebstore  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcExtensionInfo::fromWebstore(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0xd8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442f4e0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38812
// ==== 0x442f4e0  ArcExtensionInfo::extension_info  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionInfo::extension_info
               (optional<arc::ExtensionInfo> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0xd8 = 0;
  if (*(char *)(param_1 + 0xe0) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::ExtensionInfo> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x442f528);
      (*pcVar1)();
    }
    FUN_04334f1c(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)&__return_storage_ptr__->field1_0xd8 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38837
// ==== 0x442f528  ArcExtensionInfo::.cxx_destruct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionInfo::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0xe0) == '\0') {
    return;
  }
  FUN_04334d2c(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38852
// ==== 0x442f53c  ArcExtensionInfo::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0xe0) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38865
// ==== 0x442f548  ArcDownloadItem::initWithDownloadItem:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcDownloadItem::initWithDownloadItem_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcDownloadItem;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0442f5a8;
  plVar4 = *(long **)(IVar2 + 0x18);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0442f5a4;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0442f5a4:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0442f5a8:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38924
// ==== 0x442f66c  ArcDownloadItem::completedBytes  (92 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::completedBytes(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long_long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f6c8);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442f6b0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x28))();
    return lVar3;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38952
// ==== 0x442f6c8  ArcDownloadItem::totalBytes  (92 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::totalBytes(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long_long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f724);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442f70c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x30))();
    return lVar3;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:38980
// ==== 0x442f724  ArcDownloadItem::currentSpeed  (100 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::currentSpeed(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442f788);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x10);
  if (lVar3 != 0) {
    lVar2 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))();
    if (lVar2 == 0) {
      return 0;
    }
    lVar3 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))();
    if ((*(byte *)(lVar3 + 0x3a4) & 1) == 0) {
      return *(long_long *)(lVar3 + 0x430);
    }
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39011
// ==== 0x442f788  ArcDownloadItem::targetPath  (260 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::targetPath(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  ulong uVar2;
  code *pcVar3;
  bool bVar4;
  long lVar5;
  ID IVar6;
  uint uVar7;
  long lVar8;
  undefined1 *local_50;
  ulong uStack_48;
  undefined8 local_40;
  undefined1 *local_38;
  ulong uStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar8 = *(long *)(param_1 + 0x10);
    if (lVar8 == 0) {
      uVar7 = 0;
      local_40 = local_40 & 0xffffffffffffff;
      local_50 = (undefined1 *)((ulong)local_50 & 0xffffffffffffff00);
    }
    else {
      lVar5 = (**(code **)(**(long **)(lVar8 + 0x18) + 0xf8))();
      if (lVar5 == 0) {
        uVar7 = 0;
        local_50 = (undefined1 *)0x0;
        uStack_48 = 0;
        local_40 = 0;
      }
      else {
        (**(code **)(**(long **)(lVar8 + 0x18) + 0x108))(&local_38);
        if (cStack_21 < '\0') {
          FUN_00060a60(&local_50,local_38,uStack_30);
          if (cStack_21 < '\0') {
            FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
          }
        }
        else {
          uStack_48 = uStack_30;
          local_50 = local_38;
          local_40 = CONCAT17(cStack_21,local_28);
        }
        uVar7 = (uint)local_40._7_1_;
      }
    }
    bVar4 = -1 < (char)uVar7;
    ppuVar1 = (undefined1 **)local_50;
    if (bVar4) {
      ppuVar1 = &local_50;
    }
    uVar2 = uStack_48;
    if (bVar4) {
      uVar2 = (ulong)uVar7;
    }
    IVar6 = FUN_01c391bc(ppuVar1,uVar2);
    if ((long)local_40 < 0) {
      FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
    }
    return IVar6;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x442f88c);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39089
// ==== 0x442f8b0  ArcDownloadItem::sourceURL  (148 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::sourceURL(ID param_1,SEL param_2)

{
  undefined1 *puVar1;
  code *pcVar2;
  bool bVar3;
  ID IVar4;
  uint uVar5;
  undefined1 local_38;
  undefined7 uStack_37;
  ulong uStack_30;
  undefined7 local_28;
  byte bStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (*(long *)(param_1 + 0x10) == 0) {
      uVar5 = 0;
      bStack_21 = 0;
      local_38 = 0;
    }
    else {
      FUN_0430dfbc(&local_38);
      uVar5 = (uint)bStack_21;
    }
    bVar3 = -1 < (char)uVar5;
    puVar1 = (undefined1 *)CONCAT71(uStack_37,local_38);
    if (bVar3) {
      puVar1 = &local_38;
    }
    if (bVar3) {
      uStack_30 = (ulong)uVar5;
    }
    IVar4 = FUN_01c391bc(puVar1,uStack_30);
    if ((char)bStack_21 < '\0') {
      FUN_00005810(CONCAT71(uStack_37,local_38),CONCAT17(bStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442f944);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39140
// ==== 0x442f968  ArcDownloadItem::id  (148 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::id(ID param_1,SEL param_2)

{
  undefined1 *puVar1;
  code *pcVar2;
  bool bVar3;
  ID IVar4;
  uint uVar5;
  undefined1 local_38;
  undefined7 uStack_37;
  ulong uStack_30;
  undefined7 local_28;
  byte bStack_21;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if (*(long *)(param_1 + 0x10) == 0) {
      uVar5 = 0;
      bStack_21 = 0;
      local_38 = 0;
    }
    else {
      FUN_0430e358(&local_38);
      uVar5 = (uint)bStack_21;
    }
    bVar3 = -1 < (char)uVar5;
    puVar1 = (undefined1 *)CONCAT71(uStack_37,local_38);
    if (bVar3) {
      puVar1 = &local_38;
    }
    if (bVar3) {
      uStack_30 = (ulong)uVar5;
    }
    IVar4 = FUN_01c391bc(puVar1,uStack_30);
    if ((char)bStack_21 < '\0') {
      FUN_00005810(CONCAT71(uStack_37,local_38),CONCAT17(bStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x442f9fc);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39191
// ==== 0x442fa20  ArcDownloadItem::startTime  (80 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::startTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  undefined1 auVar3 [16];
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fa70);
    (*pcVar1)();
  }
  IVar2 = 0;
  if (*(long *)(param_1 + 0x10) != 0) {
    auVar3 = FUN_0430def0();
    if ((auVar3._8_8_ & 1) == 0) {
      IVar2 = 0;
    }
    else {
      IVar2 = NSDate::dateWithTimeIntervalSince1970_
                        ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",auVar3._0_8_);
    }
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39225
// ==== 0x442fa70  ArcDownloadItem::webContents  (324 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::webContents(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_48 [8];
  long *local_40;
  byte local_38;
  
  if (*(ID *)(param_1 + 8) != 0) {
    return *(ID *)(param_1 + 8);
  }
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if ((*(long *)(param_1 + 0x10) == 0) || (FUN_0430e1e8(auStack_48), local_38 == 0)) {
      return 0;
    }
    IVar2 = _objc_alloc(&objc::class_t::ArcWebContents);
    if ((local_38 & 1) != 0) {
      IVar2 = ArcWebContents::initWithWebContents_(IVar2,"initWithWebContents:",auStack_48);
      *(ID *)(param_1 + 8) = IVar2;
      if ((local_38 != 0) && (local_40 != (long *)0x0)) {
        LOAcquire();
        lVar1 = local_40[1];
        local_40[1] = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*local_40 + 0x10))(local_40);
          if (local_40[2] != 0) {
            LOAcquire();
            lVar1 = local_40[2];
            local_40[2] = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_0442fb24;
          }
          pcVar3 = *(code **)(*local_40 + 0x20);
          if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
            FUN_00005810(local_40,0x20);
          }
          else {
            (*pcVar3)(local_40);
          }
        }
      }
LAB_0442fb24:
      return *(ID *)(param_1 + 8);
    }
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x442fbb4);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39286
// ==== 0x442fc60  ArcDownloadItem::currentState  (104 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::currentState(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  long lVar3;
  long_long lVar4;
  long lVar5;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fcc4);
    (*pcVar1)();
  }
  lVar5 = *(long *)(param_1 + 0x10);
  if (lVar5 == 0) {
    lVar4 = 0;
  }
  else {
    lVar3 = (**(code **)(**(long **)(lVar5 + 0x18) + 0xf8))();
    lVar4 = 0;
    if (lVar3 != 0) {
      uVar2 = (**(code **)(**(long **)(lVar5 + 0x18) + 0x118))();
      if (3 < uVar2) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(0,0x442fcc8);
        (*pcVar1)();
      }
      lVar4 = (long_long)(uVar2 + 1);
    }
  }
  return lVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39325
// ==== 0x442fcd0  ArcDownloadItem::remainingTimeInSeconds  (144 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcDownloadItem::remainingTimeInSeconds(ID param_1,SEL param_2)

{
  code *pcVar1;
  ulong local_20;
  long_long local_18;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fd54);
    (*pcVar1)();
  }
  local_20 = 0;
  local_18 = 0;
  if (*(long *)(param_1 + 0x10) == 0) {
    local_20 = FUN_04b37104(0x35,"impl is null",0xc,0x23,"../../arc/browser/public/download_item.cc"
                           );
    if (local_20 != 1) goto LAB_0442fd38;
    FUN_04b37cec(&local_20);
  }
  else {
    FUN_0430ddd0(&local_20);
  }
  if (local_20 == 1) {
    return local_18;
  }
LAB_0442fd38:
  if ((local_20 & 1) == 0) {
    FUN_04b35898();
  }
  return 0xffffffffffffffff;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39363
// ==== 0x442fd60  ArcDownloadItem::isPaused  (32 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcDownloadItem::isPaused(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
                    /* WARNING: Could not recover jumptable at 0x0442fd78. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar2 = (**(code **)(**(long **)(*(long *)(param_1 + 0x10) + 0x18) + 0x120))();
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442fd80);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39385
// ==== 0x442fd80  ArcDownloadItem::pause  (88 bytes)

/* Function Stack Size: 0x10 bytes */

int __thiscall ArcDownloadItem::pause(ArcDownloadItem *this)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  
  if (((byte)this[0x20] & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fdd8);
    (*pcVar1)();
  }
  lVar4 = *(long *)(this + 0x10);
  if ((lVar4 != 0) &&
     (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), this = (ArcDownloadItem *)0x0,
     lVar3 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442fdc4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x170))();
    return iVar2;
  }
  return (int)this;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39415
// ==== 0x442fdd8  ArcDownloadItem::cancel:  (96 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcDownloadItem::cancel_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fe38);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x10);
  if ((lVar3 != 0) && (lVar2 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442fe24. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(long **)(lVar3 + 0x18) + 0x180))(*(long **)(lVar3 + 0x18),param_3);
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39442
// ==== 0x442fe38  ArcDownloadItem::resume  (88 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcDownloadItem::resume(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fe90);
    (*pcVar1)();
  }
  lVar3 = *(long *)(param_1 + 0x10);
  if ((lVar3 != 0) && (lVar2 = (**(code **)(**(long **)(lVar3 + 0x18) + 0xf8))(), lVar2 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442fe7c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(long **)(lVar3 + 0x18) + 0x178))();
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39469
// ==== 0x442fe90  ArcDownloadItem::remove  (88 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcDownloadItem::remove(char *param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  
  if ((param_1[0x20] & 1U) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fee8);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) &&
     (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), param_1 = (char *)0x0, lVar3 != 0))
  {
                    /* WARNING: Could not recover jumptable at 0x0442fed4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x188))();
    return iVar2;
  }
  return (int)param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39499
// ==== 0x442fee8  ArcDownloadItem::isDone  (92 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcDownloadItem::isDone(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442ff44);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar3 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442ff2c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x168))();
    return bVar2;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39527
// ==== 0x442ff44  ArcDownloadItem::canResume  (92 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcDownloadItem::canResume(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long lVar3;
  long lVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442ffa0);
    (*pcVar1)();
  }
  lVar4 = *(long *)(param_1 + 0x10);
  if ((lVar4 != 0) && (lVar3 = (**(code **)(**(long **)(lVar4 + 0x18) + 0xf8))(), lVar3 != 0)) {
                    /* WARNING: Could not recover jumptable at 0x0442ff88. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    bVar2 = (**(code **)(**(long **)(lVar4 + 0x18) + 0x1a0))();
    return bVar2;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39555
// ==== 0x442ffa0  ArcCookie::init  (420 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCookie::init(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  ID IVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  long *plVar6;
  ID local_170;
  class_t *pcStack_168;
  undefined **local_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined2 local_108;
  undefined4 local_104;
  undefined1 local_100;
  undefined1 local_90;
  undefined8 local_88;
  undefined1 local_80;
  undefined1 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  pcStack_168 = &objc::class_t::ArcCookie;
  local_170 = param_1;
  IVar3 = NSObject::init((ID)&local_170,"init");
  if (IVar3 == 0) {
    return 0;
  }
  if (*(char *)(IVar3 + 0x18) == '\0') goto LAB_0442fffc;
  plVar6 = *(long **)(IVar3 + 0x10);
  if (plVar6 != (long *)0x0) {
    LOAcquire();
    lVar2 = plVar6[1];
    plVar6[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar6 + 0x10))(plVar6);
      if (plVar6[2] != 0) {
        LOAcquire();
        lVar2 = plVar6[2];
        plVar6[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_0442fff8;
      }
      pcVar1 = *(code **)(*plVar6 + 0x20);
      if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
        FUN_00005810(plVar6,0x20);
      }
      else {
        (*pcVar1)(plVar6);
      }
    }
  }
LAB_0442fff8:
  *(undefined1 *)(IVar3 + 0x18) = 0;
LAB_0442fffc:
  local_104 = 0;
  local_100 = 0;
  local_90 = 0;
  uStack_150 = 0;
  local_158 = 0;
  uStack_140 = 0;
  local_148 = 0;
  uStack_130 = 0;
  local_138 = 0;
  uStack_120 = 0;
  local_128 = 0;
  uStack_110 = 0;
  local_118 = 0;
  local_108 = 0;
  local_88 = 0xffffffff00000000;
  local_160 = &PTR_FUN_0daa6758;
  local_80 = 0;
  local_58 = 0;
  uStack_40 = 0;
  local_48 = 0;
  local_50 = 0;
  local_38 = 0x300000001;
  puVar4 = (undefined8 *)FUN_00008790(0x90);
  puVar4[1] = 0;
  puVar4[2] = 0;
  *puVar4 = &PTR_viewWillStartLiveResize_0da7e888;
  if (puVar4 + 3 != (undefined8 *)0x0) {
    uVar5 = FUN_043345f8(puVar4 + 3,&local_160);
    *(undefined8 *)(IVar3 + 8) = uVar5;
    *(undefined8 **)(IVar3 + 0x10) = puVar4;
    FUN_016a2f44(&local_160);
    *(undefined1 *)(IVar3 + 0x18) = 1;
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430144);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39667
// ==== 0x4430144  ArcCookie::initWithCookie:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCookie::initWithCookie_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCookie;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_044301a4;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044301a0;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_044301a0:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_044301a4:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39726
// ==== 0x4430268  ArcCookie::name  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCookie::name(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar5 = *(undefined8 **)(param_1 + 8);
    if (*(char *)((long)puVar5 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar5,puVar5[1]);
    }
    else {
      lStack_38 = puVar5[1];
      local_40 = (undefined1 *)*puVar5;
      local_30 = puVar5[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar4 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x4430304);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39775
// ==== 0x4430328  ArcCookie::setName:  (228 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setName_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 **ppuVar1;
  undefined8 ***pppuVar2;
  code *pcVar3;
  undefined8 ***pppuVar4;
  undefined8 **local_38;
  undefined8 *puStack_30;
  undefined8 *local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x443040c);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    puStack_30 = (undefined8 **)0x0;
    local_28 = (undefined8 **)0x0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  pppuVar4 = *(undefined8 ****)(param_1 + 8);
  if (pppuVar4 != &local_38) {
    if (*(char *)((long)pppuVar4 + 0x17) < '\0') {
      ppuVar1 = (undefined8 **)puStack_30;
      pppuVar2 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        ppuVar1 = (undefined8 **)((ulong)local_28 >> 0x38);
        pppuVar2 = &local_38;
      }
      FUN_00359ad8(pppuVar4,pppuVar2,ppuVar1);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar4,local_38,puStack_30);
    }
    else {
      pppuVar4[2] = (undefined8 **)local_28;
      pppuVar4[1] = (undefined8 **)puStack_30;
      *pppuVar4 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,(ulong)local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39832
// ==== 0x443040c  ArcCookie::setValue:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setValue_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ulong uVar2;
  undefined8 ***pppuVar3;
  code *pcVar4;
  long lVar5;
  undefined8 **local_38;
  ulong uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x44304f4);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  lVar5 = *(long *)(param_1 + 8);
  pppuVar1 = (undefined8 ***)(lVar5 + 0x18);
  if (pppuVar1 != &local_38) {
    if (*(char *)(lVar5 + 0x2f) < '\0') {
      uVar2 = uStack_30;
      pppuVar3 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        uVar2 = local_28 >> 0x38;
        pppuVar3 = &local_38;
      }
      FUN_00359ad8(pppuVar1,pppuVar3,uVar2);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar1,local_38,uStack_30);
    }
    else {
      *(ulong *)(lVar5 + 0x28) = local_28;
      *(ulong *)(lVar5 + 0x20) = uStack_30;
      *pppuVar1 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39891
// ==== 0x44304f4  ArcCookie::domain  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCookie::domain(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x5f) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x48),*(undefined8 *)(lVar4 + 0x50));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x50);
      local_40 = *(undefined1 **)(lVar4 + 0x48);
      local_30 = *(ulong *)(lVar4 + 0x58);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4430590);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39939
// ==== 0x44305b4  ArcCookie::setDomain:  (228 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setDomain_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ulong uVar2;
  undefined8 ***pppuVar3;
  code *pcVar4;
  long lVar5;
  undefined8 **local_38;
  ulong uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4430698);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  lVar5 = *(long *)(param_1 + 8);
  pppuVar1 = (undefined8 ***)(lVar5 + 0x48);
  if (pppuVar1 != &local_38) {
    if (*(char *)(lVar5 + 0x5f) < '\0') {
      uVar2 = uStack_30;
      pppuVar3 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        uVar2 = local_28 >> 0x38;
        pppuVar3 = &local_38;
      }
      FUN_00359ad8(pppuVar1,pppuVar3,uVar2);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar1,local_38,uStack_30);
    }
    else {
      *(ulong *)(lVar5 + 0x58) = local_28;
      *(ulong *)(lVar5 + 0x50) = uStack_30;
      *pppuVar1 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:39998
// ==== 0x4430698  ArcCookie::path  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCookie::path(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 8);
    if (*(char *)(lVar4 + 0x47) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0x30),*(undefined8 *)(lVar4 + 0x38));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x38);
      local_40 = *(undefined1 **)(lVar4 + 0x30);
      local_30 = *(ulong *)(lVar4 + 0x40);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4430734);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40046
// ==== 0x4430758  ArcCookie::setPath:  (232 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCookie::setPath_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ulong uVar2;
  undefined8 ***pppuVar3;
  code *pcVar4;
  long lVar5;
  undefined8 **local_38;
  ulong uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4430840);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_38 = (undefined8 ***)0x0;
    uStack_30 = 0;
    local_28 = 0;
  }
  else {
    FUN_01b356b8(&local_38,param_3);
  }
  lVar5 = *(long *)(param_1 + 8);
  pppuVar1 = (undefined8 ***)(lVar5 + 0x30);
  if (pppuVar1 != &local_38) {
    if (*(char *)(lVar5 + 0x47) < '\0') {
      uVar2 = uStack_30;
      pppuVar3 = (undefined8 ***)local_38;
      if (-1 < (long)local_28) {
        uVar2 = local_28 >> 0x38;
        pppuVar3 = &local_38;
      }
      FUN_00359ad8(pppuVar1,pppuVar3,uVar2);
    }
    else if ((long)local_28 < 0) {
      FUN_00359998(pppuVar1,local_38,uStack_30);
    }
    else {
      *(ulong *)(lVar5 + 0x40) = local_28;
      *(ulong *)(lVar5 + 0x38) = uStack_30;
      *pppuVar1 = local_38;
    }
  }
  if ((long)local_28 < 0) {
    FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40105
// ==== 0x4430840  ArcCookie::hostOnly  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCookie::hostOnly(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x61);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430858);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40123
// ==== 0x4430858  ArcCookie::setHostOnly:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setHostOnly_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 0x61) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430870);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40142
// ==== 0x4430870  ArcCookie::httpOnly  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCookie::httpOnly(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x60);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430888);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40160
// ==== 0x4430888  ArcCookie::setHttpOnly:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setHttpOnly_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 0x60) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44308a0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40179
// ==== 0x44308a0  ArcCookie::secure  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCookie::secure(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x62);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44308b8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40197
// ==== 0x44308b8  ArcCookie::setSecure:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setSecure_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 0x62) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44308d0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40216
// ==== 0x44308d0  ArcCookie::session  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCookie::session(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 99);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44308e8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40234
// ==== 0x44308e8  ArcCookie::setSession:  (24 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcCookie::setSession_(ID param_1,SEL param_2,bool param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(char *)(*(long *)(param_1 + 8) + 99) = (char)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430900);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40253
// ==== 0x4430900  ArcCookie::expirationDate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcCookie::expirationDate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return *(double *)(*(long *)(param_1 + 8) + 0x68);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430918);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40271
// ==== 0x4430918  ArcCookie::setExpirationDate:  (24 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcCookie::setExpirationDate_(ID param_1,SEL param_2,double param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(double *)(*(long *)(param_1 + 8) + 0x68) = param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430930);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40290
// ==== 0x4430930  ArcCookie::sameSite  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcCookie::sameSite(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 8) + 0x70);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430948);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:40308
// ==== 0x4430948  ArcCookie::setSameSite:  (24 bytes)

/* Function Stack Size: 0x18 bytes */

void ArcCookie::setSameSite_(ID param_1,SEL param_2,long_long param_3)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    *(int *)(*(long *)(param_1 + 8) + 0x70) = (int)param_3;
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4430960);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41608
// ==== 0x4431cd0  ArcCommandBarRankingTensor::mutableCommandBarRankingTensor  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCommandBarRankingTensor::mutableCommandBarRankingTensor(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x50) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41623
// ==== 0x4431cec  ArcCommandBarRankingTensor::init:shape:data:  (1052 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x28 bytes */

ID ArcCommandBarRankingTensor::init_shape_data_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5)

{
  ulong uVar1;
  code *pcVar2;
  ID IVar3;
  ulong uVar4;
  undefined4 *puVar5;
  ID IVar6;
  undefined4 *puVar7;
  ID IVar8;
  long lVar9;
  long lVar10;
  undefined4 *puVar11;
  ulong uVar12;
  ulong uVar13;
  long lVar14;
  ID local_140;
  class_t *pcStack_138;
  undefined8 local_130;
  long lStack_128;
  long *local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_138 = &objc::class_t::ArcCommandBarRankingTensor;
  local_140 = param_1;
  IVar3 = NSObject::init((ID)&local_140,"init");
  if (IVar3 == 0) {
LAB_04432008:
    if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
      ___stack_chk_fail(0);
    }
    return IVar3;
  }
  if (*(char *)(IVar3 + 0x50) != '\0') {
    FUN_042a44a8(IVar3 + 8);
  }
  *(undefined8 *)(IVar3 + 0x48) = 0;
  *(undefined8 *)(IVar3 + 0x40) = 0;
  *(undefined8 *)(IVar3 + 0x38) = 0;
  *(undefined8 *)(IVar3 + 0x30) = 0;
  *(undefined8 *)(IVar3 + 0x28) = 0;
  *(undefined8 *)(IVar3 + 0x20) = 0;
  *(undefined8 *)(IVar3 + 0x18) = 0;
  *(undefined8 *)(IVar3 + 0x10) = 0;
  *(undefined8 *)(IVar3 + 8) = 0;
  *(undefined1 *)(IVar3 + 0x50) = 1;
  if (param_3 == 0) {
    local_f0 = 0;
    uStack_e8 = 0;
    local_e0 = 0;
  }
  else {
    FUN_01b356b8(&local_f0,param_3);
  }
  if ((*(byte *)(IVar3 + 0x50) & 1) != 0) {
    if (*(char *)(IVar3 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar3 + 8),*(ulong *)(IVar3 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar3 + 0x10) = uStack_e8;
    *(undefined8 *)(IVar3 + 8) = local_f0;
    *(undefined8 *)(IVar3 + 0x18) = local_e0;
    uVar4 = _objc_msgSend(param_4,"count");
    if (uVar4 == 0) {
      puVar5 = (undefined4 *)0x0;
    }
    else {
      if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      puVar5 = (undefined4 *)FUN_00008790(uVar4 << 2);
    }
    uStack_108 = 0;
    local_110 = 0;
    uStack_f8 = 0;
    uStack_100 = 0;
    lStack_128 = 0;
    local_130 = 0;
    uStack_118 = 0;
    local_120 = (long *)0x0;
    IVar6 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_130,&local_f0,
                          0x10);
    if (IVar6 != 0) {
      uVar13 = 0;
      uVar12 = 0;
      lVar14 = *local_120;
      lVar9 = lVar14;
      puVar11 = puVar5;
      do {
        if (lVar9 != lVar14) {
          _objc_enumerationMutation(param_4);
        }
        IVar8 = _objc_msgSend(*(ID *)(lStack_128 + uVar12 * 8),"intValue");
        if (uVar13 == uVar4) {
          uVar1 = uVar13 + 1;
          if (uVar1 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04047380();
          }
          uVar4 = uVar13 * 2;
          if (uVar4 < uVar1 || uVar4 - uVar1 == 0) {
            uVar4 = uVar1;
          }
          if (0x1ffffffffffffffe < uVar13) {
            uVar4 = 0x3fffffffffffffff;
          }
          if (uVar4 == 0) {
            puVar7 = (undefined4 *)0x0;
          }
          else {
            if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
              FUN_04b087f0();
            }
            puVar7 = (undefined4 *)FUN_00008790(uVar4 << 2);
          }
          puVar5 = puVar7 + uVar13;
          if (puVar5 == (undefined4 *)0x0) goto LAB_04432090;
          *puVar5 = (int)IVar8;
          puVar5 = puVar5 + -uVar13;
          _memcpy(puVar5,puVar11,uVar13 << 2);
          if (puVar11 != (undefined4 *)0x0) {
            FUN_00005810(puVar11,uVar13 << 2);
          }
        }
        else {
          if (puVar11 + uVar13 == (undefined4 *)0x0) goto LAB_04432090;
          puVar11[uVar13] = (int)IVar8;
          puVar5 = puVar11;
          puVar7 = puVar11;
        }
        lVar9 = (long)puVar7 + ((uVar13 * 4 + 4) - (long)puVar5);
        if (lVar9 == 0) goto LAB_04432090;
        uVar13 = lVar9 >> 2;
        uVar12 = uVar12 + 1;
        if (IVar6 <= uVar12) {
          IVar6 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_130,
                                &local_f0,0x10);
          if (IVar6 == 0) goto LAB_04431fb0;
          uVar12 = 0;
        }
        lVar9 = *local_120;
        puVar11 = puVar5;
      } while( true );
    }
    uVar13 = 0;
LAB_04431fb0:
    if ((*(byte *)(IVar3 + 0x50) & 1) != 0) {
      lVar9 = *(long *)(IVar3 + 0x20);
      if (lVar9 != 0) {
        lVar14 = *(long *)(IVar3 + 0x28) * 4;
        if (lVar14 != 0) {
          lVar10 = *(long *)(IVar3 + 0x28) * -4;
          lVar14 = lVar14 + lVar9;
          do {
            lVar14 = lVar14 + -4;
            if (lVar14 == 0) goto LAB_04432090;
            lVar10 = lVar10 + 4;
          } while (lVar10 != 0);
        }
        *(undefined8 *)(IVar3 + 0x28) = 0;
        FUN_00005810(lVar9,*(long *)(IVar3 + 0x30) << 2);
      }
      *(undefined4 **)(IVar3 + 0x20) = puVar5;
      *(ulong *)(IVar3 + 0x28) = uVar13;
      *(ulong *)(IVar3 + 0x30) = uVar4;
      FUN_0443181c(&local_f0,param_5);
      if ((*(byte *)(IVar3 + 0x50) & 1) != 0) {
        lVar9 = *(long *)(IVar3 + 0x38);
        if (lVar9 != 0) {
          lVar14 = *(long *)(IVar3 + 0x40) * 4;
          if (lVar14 != 0) {
            lVar10 = *(long *)(IVar3 + 0x40) * -4;
            lVar14 = lVar14 + lVar9;
            do {
              lVar14 = lVar14 + -4;
              if (lVar14 == 0) goto LAB_04432090;
              lVar10 = lVar10 + 4;
            } while (lVar10 != 0);
          }
          *(undefined8 *)(IVar3 + 0x40) = 0;
          FUN_00005810(lVar9,*(long *)(IVar3 + 0x48) << 2);
        }
        *(undefined8 *)(IVar3 + 0x38) = local_f0;
        *(undefined8 *)(IVar3 + 0x40) = uStack_e8;
        *(undefined8 *)(IVar3 + 0x48) = local_e0;
        goto LAB_04432008;
      }
    }
  }
LAB_04432090:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4432094);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41838
// ==== 0x4432164  ArcCommandBarRankingTensor::initWithCommandBarRankingTensor:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingTensor::initWithCommandBarRankingTensor_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingTensor;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x50) != '\0') {
      FUN_042a44a8(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0x50) = 0;
    }
    FUN_042a5760(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0x50) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41865
// ==== 0x44321d8  ArcCommandBarRankingTensor::name  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingTensor::name(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x50) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4432208);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41897
// ==== 0x4432208  ArcCommandBarRankingTensor::shape  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingTensor::shape(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  uint *puVar4;
  
  if ((*(byte *)(param_1 + 0x50) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x28));
    lVar3 = *(long *)(param_1 + 0x28) << 2;
    if (lVar3 != 0) {
      puVar4 = *(uint **)(param_1 + 0x20);
      do {
        IVar2 = NSNumber::numberWithInt_
                          ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",(ulong)*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -4;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44322c8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41937
// ==== 0x44322c8  ArcCommandBarRankingTensor::data  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingTensor::data(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  
  if ((*(byte *)(param_1 + 0x50) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x40));
    lVar3 = *(long *)(param_1 + 0x40) << 2;
    if (lVar3 != 0) {
      puVar4 = *(undefined4 **)(param_1 + 0x38);
      do {
        IVar2 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -4;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4432388);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41976
// ==== 0x4432388  ArcCommandBarRankingTensor::command_bar_ranking_tensor  (64 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingTensor::command_bar_ranking_tensor
               (optional<arc::CommandBarRankingTensor> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x48 = 0;
  if (*(char *)(param_1 + 0x50) != '\0') {
    FUN_042a5760(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)&__return_storage_ptr__->field1_0x48 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:41995
// ==== 0x44323e4  ArcCommandBarRankingTensor::.cxx_destruct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingTensor::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x50) == '\0') {
    return;
  }
  FUN_042a44a8(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42122
// ==== 0x44326cc  ArcCommandBarRankingResult::init:score:  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcCommandBarRankingResult::init_score_(ID param_1,SEL param_2,ID param_3,float param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingResult;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x1f) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    if (param_3 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_3);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
LAB_044327e4:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x44327e8);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
      if ((*(byte *)(IVar2 + 0x28) & 1) == 0) goto LAB_044327e4;
    }
    else {
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
    }
    *(float *)(IVar2 + 0x20) = param_4;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42183
// ==== 0x44327e8  ArcCommandBarRankingResult::initWithCommandBarRankingResult:  (168 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingResult::initWithCommandBarRankingResult_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingResult;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x28) != '\0') {
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x28) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    *(undefined4 *)(IVar1 + 0x20) = *(undefined4 *)((long)param_3 + 0x18);
    *(undefined1 *)(IVar1 + 0x28) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42224
// ==== 0x4432890  ArcCommandBarRankingResult::score  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingResult::score(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(float *)(param_1 + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44328a4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42242
// ==== 0x44328a4  ArcCommandBarRankingResult::command_bar_ranking_result  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingResult::command_bar_ranking_result
               (optional<arc::CommandBarRankingResult> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingResult> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432940);
      (*pcVar1)();
    }
    if (*(char *)(param_1 + 0x1f) < '\0') {
      FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),
                   *(undefined8 *)(param_1 + 0x10));
    }
    else {
      uVar2 = *(undefined8 *)(param_1 + 8);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8) =
           *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)&__return_storage_ptr__->field0_0x0 = uVar2;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
    }
    (__return_storage_ptr__->field0_0x0).field1.field1_0x18 = *(float *)(param_1 + 0x20);
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42281
// ==== 0x4432940  ArcCommandBarRankingModel::init:version:  (284 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcCommandBarRankingModel::init_version_(ID param_1,SEL param_2,ID param_3,int param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingModel;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x1f) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    if (param_3 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_3);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
LAB_04432a58:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432a5c);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
      if ((*(byte *)(IVar2 + 0x28) & 1) == 0) goto LAB_04432a58;
    }
    else {
      *(undefined8 *)(IVar2 + 0x10) = uStack_50;
      *(undefined8 *)(IVar2 + 8) = local_58;
      *(undefined8 *)(IVar2 + 0x18) = local_48;
    }
    *(int *)(IVar2 + 0x20) = param_4;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42342
// ==== 0x4432a5c  ArcCommandBarRankingModel::initWithCommandBarRankingModel:  (168 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingModel::initWithCommandBarRankingModel_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingModel;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x28) != '\0') {
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x28) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    *(undefined4 *)(IVar1 + 0x20) = *(undefined4 *)((long)param_3 + 0x18);
    *(undefined1 *)(IVar1 + 0x28) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42422
// ==== 0x4432be0  ArcCommandBarRankingInput::mutableCommandBarRankingInput  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCommandBarRankingInput::mutableCommandBarRankingInput(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42437
// ==== 0x4432bfc  ArcCommandBarRankingInput::init:  (304 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingInput::init_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingInput;
  local_30 = param_1;
  IVar2 = NSObject::init((ID)&local_30,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x20) != '\0') && (lVar3 = *(long *)(IVar2 + 8), lVar3 != 0)) {
      lVar5 = *(long *)(IVar2 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar4 = *(long *)(IVar2 + 0x10) * -4;
        lVar5 = lVar5 + lVar3;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04432d08;
          lVar4 = lVar4 + 4;
        } while (lVar4 != 0);
      }
      *(undefined8 *)(IVar2 + 0x10) = 0;
      FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
    }
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined1 *)(IVar2 + 0x20) = 1;
    FUN_0443181c(&local_48,param_3);
    if ((*(byte *)(IVar2 + 0x20) & 1) == 0) {
LAB_04432d08:
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432d0c);
      (*pcVar1)();
    }
    lVar3 = *(long *)(IVar2 + 8);
    if (lVar3 != 0) {
      lVar5 = *(long *)(IVar2 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar4 = *(long *)(IVar2 + 0x10) * -4;
        lVar5 = lVar5 + lVar3;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04432d08;
          lVar4 = lVar4 + 4;
        } while (lVar4 != 0);
      }
      *(undefined8 *)(IVar2 + 0x10) = 0;
      FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
    }
    *(undefined8 *)(IVar2 + 8) = local_48;
    *(undefined8 *)(IVar2 + 0x10) = local_40;
    *(undefined8 *)(IVar2 + 0x18) = local_38;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42509
// ==== 0x4432d2c  ArcCommandBarRankingInput::initWithCommandBarRankingInput:  (244 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingInput::initWithCommandBarRankingInput_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  void *pvVar4;
  long lVar5;
  long lVar6;
  void *pvVar7;
  ulong uVar8;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingInput;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if (*(char *)(IVar2 + 0x20) != '\0') {
      lVar3 = *(long *)(IVar2 + 8);
      if (lVar3 != 0) {
        lVar6 = *(long *)(IVar2 + 0x10) * 4;
        if (lVar6 != 0) {
          lVar5 = *(long *)(IVar2 + 0x10) * -4;
          lVar6 = lVar6 + lVar3;
          do {
            lVar6 = lVar6 + -4;
            if (lVar6 == 0) {
                    /* WARNING: Does not return */
              pcVar1 = (code *)SoftwareBreakpoint(1,0x4432e20);
              (*pcVar1)();
            }
            lVar5 = lVar5 + 4;
          } while (lVar5 != 0);
        }
        *(undefined8 *)(IVar2 + 0x10) = 0;
        FUN_00005810(lVar3,*(long *)(IVar2 + 0x18) << 2);
      }
      *(undefined1 *)(IVar2 + 0x20) = 0;
    }
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    uVar8 = *(ulong *)((long)param_3 + 8);
    if (uVar8 != 0) {
      if (uVar8 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar7 = *(void **)param_3;
      pvVar4 = (void *)FUN_00008790(uVar8 << 2);
      *(void **)(IVar2 + 8) = pvVar4;
      *(undefined8 *)(IVar2 + 0x10) = 0;
      *(ulong *)(IVar2 + 0x18) = uVar8;
      _memcpy(pvVar4,pvVar7,uVar8 << 2);
      *(long *)(IVar2 + 0x10) = (long)(uVar8 << 2) >> 2;
    }
    *(undefined1 *)(IVar2 + 0x20) = 1;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42577
// ==== 0x4432e20  ArcCommandBarRankingInput::queryEmbedding  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingInput::queryEmbedding(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x10));
    lVar3 = *(long *)(param_1 + 0x10) << 2;
    if (lVar3 != 0) {
      puVar4 = *(undefined4 **)(param_1 + 8);
      do {
        IVar2 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -4;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4432ee0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42616
// ==== 0x4432ee0  ArcCommandBarRankingInput::command_bar_ranking_input  (140 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingInput::command_bar_ranking_input
               (optional<arc::CommandBarRankingInput> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  float *pfVar2;
  void *pvVar3;
  ulong uVar4;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 0;
  if (*(char *)(param_1 + 0x20) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingInput> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4432f68);
      (*pcVar1)();
    }
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field0_0x0 = (float *)0x0;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field1_0x8 = 0;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field2_0x10 = 0;
    uVar4 = *(ulong *)(param_1 + 0x10);
    if (uVar4 != 0) {
      if (uVar4 >> 0x3e != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04047380();
      }
      pvVar3 = *(void **)(param_1 + 8);
      pfVar2 = (float *)FUN_00008790(uVar4 << 2);
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field0_0x0 = pfVar2;
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field2_0x10 = uVar4;
      _memcpy(pfVar2,pvVar3,uVar4 << 2);
      (__return_storage_ptr__->field0_0x0).field1.field0_0x0.field0_0x0.field1_0x8 =
           (long)(uVar4 << 2) >> 2;
    }
    *(undefined1 *)&__return_storage_ptr__->field1_0x18 = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42661
// ==== 0x4432f6c  ArcCommandBarRankingInput::.cxx_destruct  (84 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingInput::_cxx_destruct(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  
  if ((*(char *)(param_1 + 0x20) != '\0') && (lVar2 = *(long *)(param_1 + 8), lVar2 != 0)) {
    lVar4 = *(long *)(param_1 + 0x10) * 4;
    if (lVar4 != 0) {
      lVar3 = *(long *)(param_1 + 0x10) * -4;
      lVar4 = lVar4 + lVar2;
      do {
        lVar4 = lVar4 + -4;
        if (lVar4 == 0) {
                    /* WARNING: Does not return */
          pcVar1 = (code *)SoftwareBreakpoint(1,0x4432fc0);
          (*pcVar1)();
        }
        lVar3 = lVar3 + 4;
      } while (lVar3 != 0);
    }
    *(undefined8 *)(param_1 + 0x10) = 0;
    FUN_00005810(lVar2,*(long *)(param_1 + 0x18) << 2);
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42696
// ==== 0x4432fc0  ArcConfirmInfobar::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcConfirmInfobar::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x20) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42765
// ==== 0x4433108  ArcCommandBarRankingExample::mutableCommandBarRankingExample  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCommandBarRankingExample::mutableCommandBarRankingExample(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42780
// ==== 0x4433124  ArcCommandBarRankingExample::init:candidates:selectedIndex:  (908 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x24 bytes */

ID ArcCommandBarRankingExample::init_candidates_selectedIndex_
             (ID param_1,SEL param_2,ID param_3,ID param_4,int param_5)

{
  long *plVar1;
  code *pcVar2;
  ID IVar3;
  ID IVar4;
  long lVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long local_1f8;
  long local_1f0;
  long local_1e8;
  undefined8 local_150;
  long lStack_148;
  long *local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  ID local_108;
  class_t *pcStack_100;
  undefined1 auStack_f8 [128];
  long local_78;
  
  local_78 = *(long *)PTR____stack_chk_guard_0d824020;
  pcStack_100 = &objc::class_t::ArcCommandBarRankingExample;
  local_108 = param_1;
  IVar3 = NSObject::init((ID)&local_108,"init");
  if (IVar3 == 0) {
LAB_0443343c:
    if (*(long *)PTR____stack_chk_guard_0d824020 != local_78) {
                    /* WARNING: Subroutine does not return */
      ___stack_chk_fail(0);
    }
    return IVar3;
  }
  plVar1 = (long *)(IVar3 + 8);
  if (*(char *)(IVar3 + 0x40) != '\0') {
    lVar8 = *(long *)(IVar3 + 0x20);
    if (lVar8 != 0) {
      lVar5 = *(long *)(IVar3 + 0x28) * 0xa8;
      lVar9 = lVar8;
      if (lVar5 != 0) {
        lVar5 = lVar5 + lVar8;
        lVar9 = *(long *)(IVar3 + 0x28) * -0xa8;
        do {
          if (lVar5 == 0xa8) goto LAB_04433488;
          lVar5 = FUN_042a3ff4();
          lVar9 = lVar9 + 0xa8;
        } while (lVar9 != 0);
        lVar9 = *(long *)(IVar3 + 0x20);
      }
      *(long *)(IVar3 + 0x28) = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
      FUN_00005810(lVar9,*(long *)(IVar3 + 0x30) * 0xa8);
    }
    lVar8 = *plVar1;
    if (lVar8 != 0) {
      lVar5 = *(long *)(IVar3 + 0x10) * 4;
      if (lVar5 != 0) {
        lVar9 = *(long *)(IVar3 + 0x10) * -4;
        lVar5 = lVar5 + lVar8;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04433488;
          lVar9 = lVar9 + 4;
        } while (lVar9 != 0);
      }
      *(undefined8 *)(IVar3 + 0x10) = 0;
      FUN_00005810(lVar8,*(long *)(IVar3 + 0x18) << 2);
    }
  }
  *(undefined8 *)(IVar3 + 0x38) = 0;
  *(undefined8 *)(IVar3 + 0x20) = 0;
  *(undefined8 *)(IVar3 + 0x18) = 0;
  *(undefined8 *)(IVar3 + 0x30) = 0;
  *(undefined8 *)(IVar3 + 0x28) = 0;
  *(undefined8 *)(IVar3 + 0x10) = 0;
  *plVar1 = 0;
  *(undefined1 *)(IVar3 + 0x40) = 1;
  FUN_04432fcc(&local_1f8,param_3);
  if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
    if (plVar1 != &local_1f8) {
      FUN_015cdd2c(IVar3 + 8,local_1f8,local_1f8 + local_1f0 * 4,(local_1f0 << 2) >> 2);
    }
    if (local_1f8 != 0) {
      if (local_1f0 * 4 != 0) {
        lVar8 = local_1f0 * -4;
        lVar5 = local_1f0 * 4 + local_1f8;
        do {
          lVar5 = lVar5 + -4;
          if (lVar5 == 0) goto LAB_04433488;
          lVar8 = lVar8 + 4;
        } while (lVar8 != 0);
      }
      local_1f0 = 0;
      FUN_00005810(local_1f8,local_1e8 << 2);
    }
    if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
      IVar4 = _objc_msgSend(param_4,"count");
      FUN_043d9858(IVar3 + 0x20,IVar4);
      uStack_128 = 0;
      local_130 = 0;
      uStack_118 = 0;
      uStack_120 = 0;
      lStack_148 = 0;
      local_150 = 0;
      uStack_138 = 0;
      local_140 = (long *)0x0;
      IVar4 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_150,
                            auStack_f8,0x10);
      if (IVar4 == 0) {
LAB_04433424:
        if ((*(byte *)(IVar3 + 0x40) & 1) != 0) {
          *(int *)(IVar3 + 0x38) = param_5;
          goto LAB_0443343c;
        }
      }
      else {
        uVar7 = 0;
        lVar5 = *local_140;
        lVar8 = lVar5;
        while( true ) {
          if (lVar8 != lVar5) {
            _objc_enumerationMutation(param_4);
          }
          if ((*(byte *)(IVar3 + 0x40) & 1) == 0) break;
          FUN_04434c34(&local_1f8,*(undefined8 *)(lStack_148 + uVar7 * 8));
          if (*(long *)(IVar3 + 0x28) == *(long *)(IVar3 + 0x30)) {
            lVar8 = FUN_043da9d0(IVar3 + 0x20,&local_1f8);
            lVar9 = *(long *)(IVar3 + 0x20);
            lVar6 = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
          }
          else {
            lVar8 = *(long *)(IVar3 + 0x20) + *(long *)(IVar3 + 0x28) * 0xa8;
            if (lVar8 == 0) break;
            FUN_042a3ea8(lVar8,&local_1f8);
            lVar8 = lVar8 + 0xa8;
            lVar9 = *(long *)(IVar3 + 0x20);
            lVar6 = (lVar8 - lVar9 >> 3) * -0x30c30c30c30c30c3;
            *(long *)(IVar3 + 0x28) = lVar6;
          }
          *(long *)(IVar3 + 0x28) = lVar6;
          if (lVar8 == lVar9) break;
          FUN_042a3ff4(&local_1f8);
          uVar7 = uVar7 + 1;
          if (IVar4 <= uVar7) {
            IVar4 = _objc_msgSend(param_4,"countByEnumeratingWithState:objects:count:",&local_150,
                                  auStack_f8,0x10);
            if (IVar4 == 0) goto LAB_04433424;
            uVar7 = 0;
          }
          lVar8 = *local_140;
        }
      }
    }
  }
LAB_04433488:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443348c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:42954
// ==== 0x4433528  ArcCommandBarRankingExample::initWithCommandBarRankingExample:  (304 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingExample::initWithCommandBarRankingExample_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCommandBarRankingExample;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if (*(char *)(IVar2 + 0x40) != '\0') {
      lVar4 = *(long *)(IVar2 + 0x20);
      if (lVar4 != 0) {
        lVar3 = *(long *)(IVar2 + 0x28) * 0xa8;
        lVar5 = lVar4;
        if (lVar3 != 0) {
          lVar3 = lVar3 + lVar4;
          lVar5 = *(long *)(IVar2 + 0x28) * -0xa8;
          do {
            if (lVar3 == 0xa8) goto LAB_04433654;
            lVar3 = FUN_042a3ff4();
            lVar5 = lVar5 + 0xa8;
          } while (lVar5 != 0);
          lVar5 = *(long *)(IVar2 + 0x20);
        }
        *(long *)(IVar2 + 0x28) = (lVar4 - lVar5 >> 3) * -0x30c30c30c30c30c3;
        FUN_00005810(lVar5,*(long *)(IVar2 + 0x30) * 0xa8);
      }
      lVar4 = *(long *)(IVar2 + 8);
      if (lVar4 != 0) {
        lVar3 = *(long *)(IVar2 + 0x10) * 4;
        if (lVar3 != 0) {
          lVar5 = *(long *)(IVar2 + 0x10) * -4;
          lVar3 = lVar3 + lVar4;
          do {
            lVar3 = lVar3 + -4;
            if (lVar3 == 0) {
LAB_04433654:
                    /* WARNING: Does not return */
              pcVar1 = (code *)SoftwareBreakpoint(1,0x4433658);
              (*pcVar1)();
            }
            lVar5 = lVar5 + 4;
          } while (lVar5 != 0);
        }
        *(undefined8 *)(IVar2 + 0x10) = 0;
        FUN_00005810(lVar4,*(long *)(IVar2 + 0x18) << 2);
      }
      *(undefined1 *)(IVar2 + 0x40) = 0;
    }
    FUN_043dad44(IVar2 + 8,param_3);
    *(undefined1 *)(IVar2 + 0x40) = 1;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43021
// ==== 0x4433658  ArcCommandBarRankingExample::selectedIndex  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingExample::selectedIndex(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    return *(int *)(param_1 + 0x38);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443366c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43039
// ==== 0x443366c  ArcCommandBarRankingExample::command_bar_ranking_example  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingExample::command_bar_ranking_example
               (optional<arc::CommandBarRankingExample> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x40) != '\0') {
    FUN_043dad44(__return_storage_ptr__,param_1 + 8);
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43058
// ==== 0x4433768  ArcCommandBarRankingExample::.cxx_destruct  (244 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingExample::_cxx_destruct(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  undefined1 auVar5 [16];
  
  if (*(char *)(param_1 + 0x40) != '\0') {
    lVar3 = *(long *)(param_1 + 0x20);
    if (lVar3 != 0) {
      lVar2 = *(long *)(param_1 + 0x28) * 0xa8;
      lVar4 = lVar3;
      if (lVar2 != 0) {
        lVar2 = lVar2 + lVar3;
        lVar4 = *(long *)(param_1 + 0x28) * -0xa8;
        do {
          if (lVar2 + -0xa8 == 0) goto LAB_04433858;
          auVar5 = FUN_042a3ff4(lVar2 + -0xa8,param_2);
          param_2 = auVar5._8_8_;
          lVar2 = auVar5._0_8_;
          lVar4 = lVar4 + 0xa8;
        } while (lVar4 != 0);
        lVar4 = *(long *)(param_1 + 0x20);
      }
      *(long *)(param_1 + 0x28) = (lVar3 - lVar4 >> 3) * -0x30c30c30c30c30c3;
      FUN_00005810(lVar4,*(long *)(param_1 + 0x30) * 0xa8);
    }
    lVar3 = *(long *)(param_1 + 8);
    if (lVar3 != 0) {
      lVar2 = *(long *)(param_1 + 0x10) * 4;
      if (lVar2 != 0) {
        lVar4 = *(long *)(param_1 + 0x10) * -4;
        lVar2 = lVar2 + lVar3;
        do {
          lVar2 = lVar2 + -4;
          if (lVar2 == 0) {
LAB_04433858:
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(1,0x443385c);
            (*pcVar1)();
          }
          lVar4 = lVar4 + 4;
        } while (lVar4 != 0);
      }
      *(undefined8 *)(param_1 + 0x10) = 0;
      FUN_00005810(lVar3,*(long *)(param_1 + 0x18) << 2);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43696
// ==== 0x4434524  ArcCommandBarRankingCandidate::mutableCommandBarRankingCandidate  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcCommandBarRankingCandidate::mutableCommandBarRankingCandidate(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43711
// ==== 0x4434540  ArcCommandBarRankingCandidate::init:archetype:destination:title:url:lastNavSeconds:freqEver:typedCount:siteEngagement:isExactURLMatch:isBaseDomainMatch:isTitlePrefix:titleEmbedding:  (960 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x5c bytes */

ID ArcCommandBarRankingCandidate::
   init_archetype_destination_title_url_lastNavSeconds_freqEver_typedCount_siteEngagement_isExactURLMatch_isBaseDomainMatch_isTitlePrefix_titleEmbedding_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,ID param_7,
             int param_8,int param_9,int param_10,float param_11,bool param_12,bool param_13,
             bool param_14,ID param_15)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  undefined4 in_stack_00000004;
  undefined4 in_stack_00000014;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcCommandBarRankingCandidate;
  local_70 = param_1;
  IVar2 = NSObject::init((ID)&local_70,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0xb0) != '\0') {
    FUN_042a3ff4(IVar2 + 8);
  }
  *(undefined8 *)(IVar2 + 0xa8) = 0;
  *(undefined8 *)(IVar2 + 0xa0) = 0;
  *(undefined8 *)(IVar2 + 0x98) = 0;
  *(undefined8 *)(IVar2 + 0x90) = 0;
  *(undefined8 *)(IVar2 + 0x88) = 0;
  *(undefined8 *)(IVar2 + 0x80) = 0;
  *(undefined8 *)(IVar2 + 0x78) = 0;
  *(undefined8 *)(IVar2 + 0x70) = 0;
  *(undefined8 *)(IVar2 + 0x68) = 0;
  *(undefined8 *)(IVar2 + 0x60) = 0;
  *(undefined8 *)(IVar2 + 0x58) = 0;
  *(undefined8 *)(IVar2 + 0x50) = 0;
  *(undefined8 *)(IVar2 + 0x48) = 0;
  *(undefined8 *)(IVar2 + 0x40) = 0;
  *(undefined8 *)(IVar2 + 0x38) = 0;
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 8) = 0;
  *(undefined1 *)(IVar2 + 0xb0) = 1;
  if (param_3 == 0) {
    local_88 = 0;
    uStack_80 = 0;
    local_78 = 0;
  }
  else {
    FUN_01b356b8(&local_88,param_3);
  }
  if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x10) = uStack_80;
    *(undefined8 *)(IVar2 + 8) = local_88;
    *(undefined8 *)(IVar2 + 0x18) = local_78;
    if (param_4 == 0) {
      local_88 = 0;
      uStack_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_4);
    }
    if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
      if (*(char *)(IVar2 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
      }
      *(undefined8 *)(IVar2 + 0x28) = uStack_80;
      *(undefined8 *)(IVar2 + 0x20) = local_88;
      *(undefined8 *)(IVar2 + 0x30) = local_78;
      if (param_5 == 0) {
        local_88 = 0;
        uStack_80 = 0;
        local_78 = 0;
      }
      else {
        FUN_01b356b8(&local_88,param_5);
      }
      if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
        if (*(char *)(IVar2 + 0x4f) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x38),*(ulong *)(IVar2 + 0x48) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x40) = uStack_80;
        *(undefined8 *)(IVar2 + 0x38) = local_88;
        *(undefined8 *)(IVar2 + 0x48) = local_78;
        if (param_6 == 0) {
          local_88 = 0;
          uStack_80 = 0;
          local_78 = 0;
        }
        else {
          FUN_01b356b8(&local_88,param_6);
        }
        if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
          if (*(char *)(IVar2 + 0x67) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x50),*(ulong *)(IVar2 + 0x60) & 0x7fffffffffffffff
                        );
          }
          *(undefined8 *)(IVar2 + 0x58) = uStack_80;
          *(undefined8 *)(IVar2 + 0x50) = local_88;
          *(undefined8 *)(IVar2 + 0x60) = local_78;
          if (param_7 == 0) {
            local_88 = 0;
            uStack_80 = 0;
            local_78 = 0;
          }
          else {
            FUN_01b356b8(&local_88,param_7);
          }
          if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
            if (*(char *)(IVar2 + 0x7f) < '\0') {
              FUN_00005810(*(undefined8 *)(IVar2 + 0x68),
                           *(ulong *)(IVar2 + 0x78) & 0x7fffffffffffffff);
              *(undefined8 *)(IVar2 + 0x70) = uStack_80;
              *(undefined8 *)(IVar2 + 0x68) = local_88;
              *(undefined8 *)(IVar2 + 0x78) = local_78;
              if ((*(byte *)(IVar2 + 0xb0) & 1) == 0) goto LAB_044348dc;
            }
            else {
              *(undefined8 *)(IVar2 + 0x70) = uStack_80;
              *(undefined8 *)(IVar2 + 0x68) = local_88;
              *(undefined8 *)(IVar2 + 0x78) = local_78;
            }
            *(int *)(IVar2 + 0x80) = param_8;
            *(int *)(IVar2 + 0x84) = param_9;
            *(undefined4 *)(IVar2 + 0x88) = in_stack_00000004;
            *(float *)(IVar2 + 0x8c) = param_11;
            *(undefined1 *)(IVar2 + 0x90) = (undefined1)param_10;
            *(undefined1 *)(IVar2 + 0x91) = param_10._1_1_;
            *(undefined1 *)(IVar2 + 0x92) = param_10._2_1_;
            FUN_0443181c(&local_88,CONCAT44(in_stack_00000014,param_12));
            if ((*(byte *)(IVar2 + 0xb0) & 1) != 0) {
              lVar3 = *(long *)(IVar2 + 0x98);
              if (lVar3 != 0) {
                lVar5 = *(long *)(IVar2 + 0xa0) * 4;
                if (lVar5 != 0) {
                  lVar4 = *(long *)(IVar2 + 0xa0) * -4;
                  lVar5 = lVar5 + lVar3;
                  do {
                    lVar5 = lVar5 + -4;
                    if (lVar5 == 0) goto LAB_044348dc;
                    lVar4 = lVar4 + 4;
                  } while (lVar4 != 0);
                }
                *(undefined8 *)(IVar2 + 0xa0) = 0;
                FUN_00005810(lVar3,*(long *)(IVar2 + 0xa8) << 2);
              }
              *(undefined8 *)(IVar2 + 0x98) = local_88;
              *(undefined8 *)(IVar2 + 0xa0) = uStack_80;
              *(undefined8 *)(IVar2 + 0xa8) = local_78;
              return IVar2;
            }
          }
        }
      }
    }
  }
LAB_044348dc:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44348e0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43892
// ==== 0x4434900  ArcCommandBarRankingCandidate::initWithCommandBarRankingCandidate:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingCandidate::initWithCommandBarRankingCandidate_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingCandidate;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xb0) != '\0') {
      FUN_042a3ff4(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0xb0) = 0;
    }
    FUN_042a3ea8(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0xb0) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43919
// ==== 0x4434974  ArcCommandBarRankingCandidate::candidateID  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::candidateID(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x44349a4);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43951
// ==== 0x44349a4  ArcCommandBarRankingCandidate::archetype  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::archetype(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x37);
    plVar1 = (long *)*(long *)(param_1 + 0x20);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x20);
    }
    lVar2 = *(long *)(param_1 + 0x28);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x44349d4);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:43983
// ==== 0x44349d4  ArcCommandBarRankingCandidate::destination  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::destination(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x4f);
    plVar1 = (long *)*(long *)(param_1 + 0x38);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x38);
    }
    lVar2 = *(long *)(param_1 + 0x40);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4434a04);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44015
// ==== 0x4434a04  ArcCommandBarRankingCandidate::title  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::title(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x67);
    plVar1 = (long *)*(long *)(param_1 + 0x50);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x50);
    }
    lVar2 = *(long *)(param_1 + 0x58);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4434a34);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44047
// ==== 0x4434a34  ArcCommandBarRankingCandidate::url  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::url(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x7f);
    plVar1 = (long *)*(long *)(param_1 + 0x68);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x68);
    }
    lVar2 = *(long *)(param_1 + 0x70);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4434a64);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44079
// ==== 0x4434a64  ArcCommandBarRankingCandidate::lastNavSeconds  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingCandidate::lastNavSeconds(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(int *)(param_1 + 0x80);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434a78);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44097
// ==== 0x4434a78  ArcCommandBarRankingCandidate::freqEver  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingCandidate::freqEver(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(int *)(param_1 + 0x84);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434a8c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44115
// ==== 0x4434a8c  ArcCommandBarRankingCandidate::typedCount  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcCommandBarRankingCandidate::typedCount(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(int *)(param_1 + 0x88);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434aa0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44133
// ==== 0x4434aa0  ArcCommandBarRankingCandidate::siteEngagement  (20 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcCommandBarRankingCandidate::siteEngagement(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return *(float *)(param_1 + 0x8c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434ab4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44151
// ==== 0x4434ab4  ArcCommandBarRankingCandidate::isExactURLMatch  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCommandBarRankingCandidate::isExactURLMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x90);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434ac8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44169
// ==== 0x4434ac8  ArcCommandBarRankingCandidate::isBaseDomainMatch  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCommandBarRankingCandidate::isBaseDomainMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x91);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434adc);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44187
// ==== 0x4434adc  ArcCommandBarRankingCandidate::isTitlePrefix  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcCommandBarRankingCandidate::isTitlePrefix(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    return (bool)*(byte *)(param_1 + 0x92);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434af0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44205
// ==== 0x4434af0  ArcCommandBarRankingCandidate::titleEmbedding  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::titleEmbedding(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  
  if ((*(byte *)(param_1 + 0xb0) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0xa0));
    lVar3 = *(long *)(param_1 + 0xa0) << 2;
    if (lVar3 != 0) {
      puVar4 = *(undefined4 **)(param_1 + 0x98);
      do {
        IVar2 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -4;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4434bb0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44244
// ==== 0x4434bb0  ArcCommandBarRankingCandidate::command_bar_ranking_candidate  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingCandidate::command_bar_ranking_candidate
               (optional<arc::CommandBarRankingCandidate> *__return_storage_ptr__,ID param_1,
               SEL param_2)

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0xa8) = 0;
  if (*(char *)(param_1 + 0xb0) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::CommandBarRankingCandidate> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x4434bf8);
      (*pcVar1)();
    }
    FUN_042a3ea8(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)((long)&__return_storage_ptr__->field0_0x0 + 0xa8) = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44270
// ==== 0x4434c14  ArcCommandBarRankingCandidate::.cxx_destruct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCommandBarRankingCandidate::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0xb0) == '\0') {
    return;
  }
  FUN_042a3ff4(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:44285
// ==== 0x4434c28  ArcCommandBarRankingCandidate::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingCandidate::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0xb0) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46305
// ==== 0x44370b0  ArcBrowserContextDownloads::initWithBrowserContextDownloads:forBrowserContext:  (336 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserContextDownloads::initWithBrowserContextDownloads_forBrowserContext_
             (ID param_1,SEL param_2,const_void__ param_3,ID param_4)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcBrowserContextDownloads;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x30) == '\0') goto LAB_04437124;
  plVar4 = *(long **)(IVar2 + 0x28);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437120;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_04437120:
  *(undefined1 *)(IVar2 + 0x30) = 0;
LAB_04437124:
  *(undefined8 *)(IVar2 + 0x20) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x28) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x30) = 1;
  *(ID *)(IVar2 + 8) = param_4;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46369
// ==== 0x4437200  ArcBrowserContextDevTools::initWithBrowserContextDevTools:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContextDevTools::initWithBrowserContextDevTools_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBrowserContextDevTools;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_04437260;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443725c;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443725c:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_04437260:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46429
// ==== 0x4437324  ArcBrowserContextDevTools::close  (36 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcBrowserContextDevTools::close(int param_1)

{
  code *pcVar1;
  int iVar2;
  long lVar3;
  
  if ((*(byte *)((ulong)(uint)param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4437348);
    (*pcVar1)();
  }
  lVar3 = *(long *)(*(long *)((ulong)(uint)param_1 + 8) + 0x90);
  if (lVar3 != 0) {
    iVar2 = FUN_0c107cd4(lVar3,3);
    return iVar2;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46454
// ==== 0x4437348  ArcBrowserContextDevTools::webContents  (84 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextDevTools::webContents(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  const_void__ pvVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    pvVar3 = *(const_void__ *)(*(long *)(param_1 + 8) + 0x98);
    if (pvVar3 == (const_void__)0x0) {
      IVar2 = 0;
    }
    else {
      IVar2 = _objc_alloc(&objc::class_t::ArcWebContents);
      IVar2 = ArcWebContents::initWithWebContents_(IVar2,"initWithWebContents:",pvVar3);
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443739c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46483
// ==== 0x443739c  ArcBrowserContext::initWithBrowserContext:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContext::initWithBrowserContext_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBrowserContext;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x40) == '\0') goto LAB_044373fc;
  plVar4 = *(long **)(IVar2 + 0x38);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044373f8;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_044373f8:
  *(undefined1 *)(IVar2 + 0x40) = 0;
LAB_044373fc:
  *(undefined8 *)(IVar2 + 0x30) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x38) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x40) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46542
// ==== 0x44374c0  ArcBrowserContext::preferences  (332 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::preferences(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"preferences");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContextPreferences);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x443760c);
    (*pcVar3)();
  }
  FUN_042c8400(auStack_50,*(undefined8 *)(*(long *)(param_1 + 0x30) + 0x140));
  IVar2 = ArcBrowserContextPreferences::initWithBrowserContextPreferences_forBrowserContext_
                    (IVar2,"initWithBrowserContextPreferences:forBrowserContext:",auStack_50,param_1
                    );
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437554;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04437554:
  _objc_setAssociatedObject(param_1,"preferences",IVar2,1);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46601
// ==== 0x44376b0  ArcBrowserContext::history  (332 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

int ArcBrowserContext::history(History *param_1,HistEvent *param_2,int param_3,...)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"history",param_3);
  if (IVar2 != 0) goto LAB_04437758;
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContextHistory);
  if (((byte)param_1[0x40] & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44377fc);
    (*pcVar3)();
  }
  FUN_042c8400(auStack_50,*(undefined8 *)(*(long *)(param_1 + 0x30) + 0x110));
  IVar2 = ArcBrowserContextHistory::initWithBrowserContextHistory_forBrowserContext_
                    (IVar2,"initWithBrowserContextHistory:forBrowserContext:",auStack_50,(ID)param_1
                    );
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437744;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04437744:
  _objc_setAssociatedObject(param_1,"history",IVar2,1);
LAB_04437758:
  return (int)IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46659
// ==== 0x44378a0  ArcBrowserContext::downloads  (332 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::downloads(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"downloads");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContextDownloads);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44379ec);
    (*pcVar3)();
  }
  FUN_042df1ec(auStack_50,*(undefined8 *)(*(long *)(param_1 + 0x30) + 0xe0));
  IVar2 = ArcBrowserContextDownloads::initWithBrowserContextDownloads_forBrowserContext_
                    (IVar2,"initWithBrowserContextDownloads:forBrowserContext:",auStack_50,param_1);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437934;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04437934:
  _objc_setAssociatedObject(param_1,"downloads",IVar2,1);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46717
// ==== 0x4437a90  ArcBrowserContext::effects  (332 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::effects(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar2 = _objc_getAssociatedObject(param_1,"effects");
  if (IVar2 != 0) {
    return IVar2;
  }
  IVar2 = _objc_alloc(&objc::class_t::ArcBrowserContextEffects);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x4437bdc);
    (*pcVar3)();
  }
  FUN_04298600(auStack_50,*(undefined8 *)(*(long *)(param_1 + 0x30) + 0xf0));
  IVar2 = ArcBrowserContextEffects::initWithBrowserContextEffects_forBrowserContext_
                    (IVar2,"initWithBrowserContextEffects:forBrowserContext:",auStack_50,param_1);
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04437b24;
      }
      pcVar3 = *(code **)(*local_48 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar3)(local_48);
      }
    }
  }
LAB_04437b24:
  _objc_setAssociatedObject(param_1,"effects",IVar2,1);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46775
// ==== 0x4437c80  ArcBrowserContext::pageContentExtraction  (364 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::pageContentExtraction(ID param_1,SEL param_2)

{
  ID IVar1;
  long lVar2;
  undefined8 *puVar3;
  code *pcVar4;
  undefined8 uVar5;
  undefined1 auStack_50 [8];
  long *local_48;
  
  IVar1 = _objc_getAssociatedObject(param_1,"pageContentExtraction");
  if (IVar1 != 0) {
    return IVar1;
  }
  IVar1 = _objc_alloc(&objc::class_t::ArcBrowserContextPageContentExtraction);
  if ((*(byte *)(param_1 + 0x40) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4437de8);
    (*pcVar4)();
  }
  puVar3 = *(undefined8 **)(*(long *)(param_1 + 0x30) + 0x130);
  if (puVar3[1] != 0) {
    uVar5 = *puVar3;
    lVar2 = FUN_00641d68();
    if (lVar2 != 0) {
      FUN_043334c8(auStack_50,uVar5,lVar2);
      IVar1 = ArcBrowserContextPageContentExtraction::
              initWithBrowserContextPageContentExtraction_forBrowserContext_
                        (IVar1,"initWithBrowserContextPageContentExtraction:forBrowserContext:",
                         auStack_50,param_1);
      if (local_48 != (long *)0x0) {
        LOAcquire();
        lVar2 = local_48[1];
        local_48[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          (**(code **)(*local_48 + 0x10))(local_48);
          if (local_48[2] != 0) {
            LOAcquire();
            lVar2 = local_48[2];
            local_48[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_04437d30;
          }
          pcVar4 = *(code **)(*local_48 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(local_48,0x20);
          }
          else {
            (*pcVar4)(local_48);
          }
        }
      }
LAB_04437d30:
      _objc_setAssociatedObject(param_1,"pageContentExtraction",IVar1,1);
      return IVar1;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_050d342c();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46846
// ==== 0x4437e90  ArcBrowserContext::userVisibleName  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::userVisibleName(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x30);
    if (*(char *)(lVar4 + 199) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar4 + 0xb0),*(undefined8 *)(lVar4 + 0xb8));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0xb8);
      local_40 = *(undefined1 **)(lVar4 + 0xb0);
      local_30 = *(ulong *)(lVar4 + 0xc0);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < (long)local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar4);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4437f2c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46894
// ==== 0x4437f50  ArcBrowserContext::setUserVisibleName:  (148 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContext::setUserVisibleName_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    if (param_3 == 0) {
      local_38 = 0;
      uStack_30 = 0;
      local_28 = 0;
    }
    else {
      FUN_01b356b8(&local_38,param_3);
    }
    FUN_042d717c(*(undefined8 *)(param_1 + 0x30),&local_38);
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4437fe4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46929
// ==== 0x4437fe4  ArcBrowserContext::bookmarkObservers  (168 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContext::bookmarkObservers
               (vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
                *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  code *pcVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 *puVar6;
  ulong uVar7;
  
  (__return_storage_ptr__->field0_0x0).field0_0x0 = (void *)0x0;
  (__return_storage_ptr__->field0_0x0).field1_0x8 = 0;
  (__return_storage_ptr__->field0_0x0).field2_0x10 = 0;
  uVar7 = *(ulong *)(param_1 + 0x50);
  if (uVar7 != 0) {
    if (uVar7 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    puVar6 = *(undefined8 **)(param_1 + 0x48);
    puVar1 = puVar6 + uVar7 * 2;
    puVar3 = (undefined8 *)FUN_00008790(uVar7 << 4);
    (__return_storage_ptr__->field0_0x0).field0_0x0 = puVar3;
    (__return_storage_ptr__->field0_0x0).field2_0x10 = uVar7;
    puVar4 = puVar3;
    do {
      if (puVar4 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
        pcVar2 = (code *)SoftwareBreakpoint(1,0x4438088);
        (*pcVar2)();
      }
      *puVar4 = *puVar6;
      lVar5 = puVar6[1];
      puVar4[1] = lVar5;
      if (lVar5 != 0) {
        *(long *)(lVar5 + 8) = *(long *)(lVar5 + 8) + 1;
      }
      puVar6 = puVar6 + 2;
      puVar4 = puVar4 + 2;
    } while (puVar6 != puVar1);
    (__return_storage_ptr__->field0_0x0).field1_0x8 = (long)puVar4 - (long)puVar3 >> 4;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:46982
// ==== 0x443808c  ArcBrowserContext::setBookmarkObservers:  (1268 bytes)

/* Function Stack Size: 0x28 bytes */

void ArcBrowserContext::setBookmarkObservers_
               (ID param_1,SEL param_2,
               vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
               *param_3)

{
  vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
  *pvVar1;
  undefined8 *puVar2;
  unsigned_long_long uVar3;
  ulong uVar4;
  void *pvVar5;
  undefined8 *puVar6;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  ulong uVar7;
  undefined8 *puVar8;
  undefined8 *puVar9;
  code *pcVar10;
  ulong uVar11;
  long lVar12;
  void *pvVar13;
  long *plVar14;
  long *plVar15;
  long *plVar16;
  long lVar17;
  long lVar18;
  
  pvVar1 = (vector<std::shared_ptr<ArcBookmarkModelObserverBridge>,_std::allocator<std::shared_ptr<ArcBookmarkModelObserverBridge>>>
            *)(param_1 + 0x48);
  if (pvVar1 != param_3) {
    puVar8 = (param_3->field0_0x0).field0_0x0;
    uVar3 = (param_3->field0_0x0).field1_0x8;
    lVar17 = uVar3 << 4;
    puVar2 = puVar8 + uVar3 * 2;
    uVar4 = (long)(uVar3 << 4) >> 4;
    uVar7 = *(ulong *)(param_1 + 0x58);
    if (uVar7 < uVar4) {
      pvVar13 = (pvVar1->field0_0x0).field0_0x0;
      if (pvVar13 != (void *)0x0) {
        lVar12 = *(long *)(param_1 + 0x50) * 0x10;
        pvVar5 = pvVar13;
        if (lVar12 != 0) {
          lVar18 = *(long *)(param_1 + 0x50) * -0x10;
          pvVar5 = (void *)(lVar12 + (long)pvVar13);
          do {
            if ((void *)((long)pvVar5 + -0x10) == (void *)0x0) goto LAB_04438368;
            plVar14 = *(long **)((long)pvVar5 + -8);
            if (plVar14 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar14[1];
              plVar14[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar14 + 0x10))(plVar14);
                if (plVar14[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar14[2];
                  plVar14[2] = lVar12 + -1;
                  LORelease();
                  if (lVar12 != 0) goto LAB_044382d4;
                }
                pcVar10 = *(code **)(*plVar14 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar14,0x20);
                }
                else {
                  (*pcVar10)(plVar14);
                }
              }
            }
LAB_044382d4:
            lVar18 = lVar18 + 0x10;
            pvVar5 = (void *)((long)pvVar5 + -0x10);
          } while (lVar18 != 0);
          uVar7 = *(ulong *)(param_1 + 0x58);
          pvVar5 = *(void **)(param_1 + 0x48);
        }
        *(long *)(param_1 + 0x50) = (long)pvVar13 - (long)pvVar5 >> 4;
        FUN_00005810(pvVar5,uVar7 << 4);
        uVar7 = 0;
        *(undefined8 *)(param_1 + 0x48) = 0;
        *(undefined8 *)(param_1 + 0x50) = 0;
        *(undefined8 *)(param_1 + 0x58) = 0;
      }
      if (uVar4 >> 0x3c != 0) {
LAB_0443857c:
                    /* WARNING: Subroutine does not return */
        FUN_040416c0();
      }
      uVar11 = uVar7 * 2;
      if (uVar11 < uVar4 || uVar11 - uVar4 == 0) {
        uVar11 = uVar4;
      }
      if (0x7fffffffffffffe < uVar7) {
        uVar11 = 0xfffffffffffffff;
      }
      if (uVar11 >> 0x3c != 0) goto LAB_0443857c;
      puVar6 = (undefined8 *)FUN_00008790(uVar11 << 4);
      *(undefined8 **)(param_1 + 0x48) = puVar6;
      *(undefined8 *)(param_1 + 0x50) = 0;
      *(ulong *)(param_1 + 0x58) = uVar11;
      puVar9 = puVar6;
      if (lVar17 != 0) {
        do {
          if (puVar6 == (undefined8 *)0x0) {
LAB_04438368:
                    /* WARNING: Does not return */
            pcVar10 = (code *)SoftwareBreakpoint(1,0x443836c);
            (*pcVar10)();
          }
          *puVar6 = *puVar8;
          lVar17 = puVar8[1];
          puVar6[1] = lVar17;
          if (lVar17 != 0) {
            *(long *)(lVar17 + 8) = *(long *)(lVar17 + 8) + 1;
          }
          puVar8 = puVar8 + 2;
          puVar6 = puVar6 + 2;
        } while (puVar8 != puVar2);
        puVar9 = *(undefined8 **)(param_1 + 0x48);
      }
      lVar17 = (long)puVar6 - (long)puVar9;
    }
    else {
      uVar7 = *(ulong *)(param_1 + 0x50);
      if (uVar7 < uVar4) {
        pvVar13 = (pvVar1->field0_0x0).field0_0x0;
        puVar6 = puVar8;
        if ((0 < (long)uVar7) && (lVar17 = uVar7 << 4, lVar17 != 0)) {
          puVar6 = puVar8 + uVar7 * 2;
          plVar14 = (long *)((long)pvVar13 + 8);
          plVar16 = puVar8 + 1;
          do {
            lVar18 = *plVar16;
            lVar12 = plVar16[-1];
            if (*plVar16 != 0) {
              plVar15 = (long *)(*plVar16 + 8);
              *plVar15 = *plVar15 + 1;
            }
            plVar15 = (long *)*plVar14;
            *plVar14 = lVar18;
            plVar14[-1] = lVar12;
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar15[1];
              plVar15[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15,param_2);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar15[2];
                  plVar15[2] = lVar12 + -1;
                  LORelease();
                  param_2 = extraout_x1;
                  if (lVar12 != 0) goto LAB_04438378;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                  param_2 = extraout_x1_00;
                }
                else {
                  (*pcVar10)(plVar15);
                  param_2 = extraout_x1_01;
                }
              }
            }
LAB_04438378:
            plVar16 = plVar16 + 2;
            plVar14 = plVar14 + 2;
            lVar17 = lVar17 + -0x10;
          } while (lVar17 != 0);
          pvVar13 = *(void **)(param_1 + 0x48);
          uVar7 = *(ulong *)(param_1 + 0x50);
        }
        puVar8 = (undefined8 *)((long)pvVar13 + uVar7 * 0x10);
        if (puVar6 != puVar2) {
          do {
            if (puVar8 == (undefined8 *)0x0) goto LAB_04438368;
            *puVar8 = *puVar6;
            lVar17 = puVar6[1];
            puVar8[1] = lVar17;
            if (lVar17 != 0) {
              *(long *)(lVar17 + 8) = *(long *)(lVar17 + 8) + 1;
            }
            puVar6 = puVar6 + 2;
            puVar8 = puVar8 + 2;
          } while (puVar6 != puVar2);
          pvVar13 = *(void **)(param_1 + 0x48);
        }
        lVar17 = (long)puVar8 - (long)pvVar13;
      }
      else {
        plVar16 = (pvVar1->field0_0x0).field0_0x0;
        plVar14 = plVar16;
        if (lVar17 != 0) {
          plVar16 = puVar8 + 1;
          do {
            lVar18 = *plVar16;
            lVar12 = plVar16[-1];
            if (*plVar16 != 0) {
              plVar15 = (long *)(*plVar16 + 8);
              *plVar15 = *plVar15 + 1;
            }
            plVar15 = (long *)plVar14[1];
            plVar14[1] = lVar18;
            *plVar14 = lVar12;
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar12 = plVar15[1];
              plVar15[1] = lVar12 + -1;
              LORelease();
              if (lVar12 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar12 = plVar15[2];
                  plVar15[2] = lVar12 + -1;
                  LORelease();
                  if (lVar12 != 0) goto LAB_04438428;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                }
                else {
                  (*pcVar10)(plVar15);
                }
              }
            }
LAB_04438428:
            plVar16 = plVar16 + 2;
            plVar14 = plVar14 + 2;
            lVar17 = lVar17 + -0x10;
          } while (lVar17 != 0);
          plVar16 = *(long **)(param_1 + 0x48);
          uVar7 = *(ulong *)(param_1 + 0x50);
        }
        plVar15 = plVar16 + uVar7 * 2;
        if (plVar14 != plVar15) {
          do {
            plVar16 = plVar15 + -2;
            if (plVar16 == (long *)0x0) goto LAB_04438368;
            plVar15 = (long *)plVar15[-1];
            if (plVar15 != (long *)0x0) {
              LOAcquire();
              lVar17 = plVar15[1];
              plVar15[1] = lVar17 + -1;
              LORelease();
              if (lVar17 == 0) {
                (**(code **)(*plVar15 + 0x10))(plVar15);
                if (plVar15[2] != 0) {
                  LOAcquire();
                  lVar17 = plVar15[2];
                  plVar15[2] = lVar17 + -1;
                  LORelease();
                  if (lVar17 != 0) goto LAB_044384f4;
                }
                pcVar10 = *(code **)(*plVar15 + 0x20);
                if (((pcVar10 == FUN_00f01bc0) || (pcVar10 == FUN_00f01bc0)) ||
                   (pcVar10 == FUN_013efb40)) {
                  FUN_00005810(plVar15,0x20);
                }
                else {
                  (*pcVar10)(plVar15);
                }
              }
            }
LAB_044384f4:
            plVar15 = plVar16;
          } while (plVar16 != plVar14);
          plVar16 = *(long **)(param_1 + 0x48);
        }
        lVar17 = (long)plVar14 - (long)plVar16;
      }
    }
    *(long *)(param_1 + 0x50) = lVar17 >> 4;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47274
// ==== 0x4438580  ArcBrowserContext::.cxx_destruct  (652 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowserContext::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  long lVar6;
  
  lVar5 = *(long *)(param_1 + 0x48);
  if (lVar5 != 0) {
    lVar3 = *(long *)(param_1 + 0x50) * 0x10;
    lVar6 = lVar5;
    if (lVar3 != 0) {
      lVar6 = *(long *)(param_1 + 0x50) * -0x10;
      lVar3 = lVar3 + lVar5;
      do {
        if (lVar3 + -0x10 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x44387e0);
          (*pcVar2)();
        }
        plVar4 = *(long **)(lVar3 + -8);
        if (plVar4 != (long *)0x0) {
          LOAcquire();
          lVar1 = plVar4[1];
          plVar4[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4,param_2);
            if (plVar4[2] != 0) {
              LOAcquire();
              lVar1 = plVar4[2];
              plVar4[2] = lVar1 + -1;
              LORelease();
              param_2 = extraout_x1;
              if (lVar1 != 0) goto LAB_044385ec;
            }
            pcVar2 = *(code **)(*plVar4 + 0x20);
            if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40))
            {
              FUN_00005810(plVar4,0x20);
              param_2 = extraout_x1_00;
            }
            else {
              (*pcVar2)(plVar4);
              param_2 = extraout_x1_01;
            }
          }
        }
LAB_044385ec:
        lVar6 = lVar6 + 0x10;
        lVar3 = lVar3 + -0x10;
      } while (lVar6 != 0);
      lVar6 = *(long *)(param_1 + 0x48);
    }
    *(long *)(param_1 + 0x50) = lVar5 - lVar6 >> 4;
    FUN_00005810(lVar6,*(long *)(param_1 + 0x58) << 4);
  }
  if ((*(char *)(param_1 + 0x40) != '\0') &&
     (plVar4 = *(long **)(param_1 + 0x38), plVar4 != (long *)0x0)) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_044386ac;
      }
      pcVar2 = *(code **)(*plVar4 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar2)(plVar4);
      }
    }
  }
LAB_044386ac:
  plVar4 = *(long **)(param_1 + 0x28);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_044386c4;
      }
      pcVar2 = *(code **)(*plVar4 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar2)(plVar4);
      }
    }
  }
LAB_044386c4:
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47399
// ==== 0x443880c  ArcBrowserContext::.cxx_construct  (24 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContext::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined1 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47417
// ==== 0x4438824  ArcAutocompleteMatchAdditionalData::init:value:  (464 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcAutocompleteMatchAdditionalData::init_value_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  ulong uVar3;
  ID IVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined8 local_88;
  undefined8 local_80;
  ulong local_78;
  ID local_70;
  class_t *pcStack_68;
  
  pcStack_68 = &objc::class_t::ArcAutocompleteMatchAdditionalData;
  local_70 = param_1;
  IVar4 = NSObject::init((ID)&local_70,"init");
  if (IVar4 != 0) {
    if (param_3 == 0) {
      local_88 = 0;
      local_80 = 0;
      local_78 = 0;
    }
    else {
      FUN_01b356b8(&local_88,param_3);
    }
    if (param_4 == 0) {
      uVar7 = 0;
      uVar6 = 0;
      uVar5 = 0;
    }
    else {
      FUN_01b356b8(&local_a0,param_4);
      uVar5 = local_a0;
      uVar6 = uStack_98;
      uVar7 = local_90;
    }
    uVar3 = local_78;
    uVar2 = local_80;
    uVar1 = local_88;
    local_80 = 0;
    local_78 = 0;
    local_90 = 0;
    local_88 = 0;
    local_a0 = 0;
    uStack_98 = 0;
    if (*(char *)(IVar4 + 0x38) != '\0') {
      if (*(char *)(IVar4 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar4 + 0x20),*(ulong *)(IVar4 + 0x30) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar4 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar4 + 8),*(ulong *)(IVar4 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar4 + 0x38) = 0;
    }
    if ((long)uVar3 < 0) {
      FUN_00060a60(IVar4 + 8,uVar1,uVar2);
    }
    else {
      *(undefined8 *)(IVar4 + 8) = uVar1;
      *(undefined8 *)(IVar4 + 0x10) = uVar2;
      *(ulong *)(IVar4 + 0x18) = uVar3;
    }
    if ((long)uVar7 < 0) {
      FUN_00060a60(IVar4 + 0x20,uVar5,uVar6);
      *(undefined1 *)(IVar4 + 0x38) = 1;
      FUN_00005810(uVar5,uVar7 & 0x7fffffffffffffff);
    }
    else {
      *(undefined8 *)(IVar4 + 0x20) = uVar5;
      *(undefined8 *)(IVar4 + 0x28) = uVar6;
      *(ulong *)(IVar4 + 0x30) = uVar7;
      *(undefined1 *)(IVar4 + 0x38) = 1;
    }
    if ((long)uVar3 < 0) {
      FUN_00005810(uVar1,uVar3 & 0x7fffffffffffffff);
    }
    if ((long)local_78 < 0) {
      FUN_00005810(local_88,local_78 & 0x7fffffffffffffff);
    }
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47513
// ==== 0x4438a6c  ArcAutocompleteMatchAdditionalData::initWithAutocompleteMatchAdditionalData:  (224 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAutocompleteMatchAdditionalData::initWithAutocompleteMatchAdditionalData_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAutocompleteMatchAdditionalData;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x38) != '\0') {
      if (*(char *)(IVar1 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x20),*(ulong *)(IVar1 + 0x30) & 0x7fffffffffffffff);
      }
      if (*(char *)(IVar1 + 0x1f) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 8),*(ulong *)(IVar1 + 0x18) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x38) = 0;
    }
    if (*(char *)((long)param_3 + 0x17) < '\0') {
      FUN_00060a60(IVar1 + 8,*(undefined8 *)param_3,*(undefined8 *)((long)param_3 + 8));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 8);
      uVar2 = *(undefined8 *)param_3;
      *(undefined8 *)(IVar1 + 0x18) = *(undefined8 *)((long)param_3 + 0x10);
      *(undefined8 *)(IVar1 + 0x10) = uVar3;
      *(undefined8 *)(IVar1 + 8) = uVar2;
    }
    if (*(char *)((long)param_3 + 0x2f) < '\0') {
      FUN_00060a60(IVar1 + 0x20,*(undefined8 *)((long)param_3 + 0x18),
                   *(undefined8 *)((long)param_3 + 0x20));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x20);
      uVar2 = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x30) = *(undefined8 *)((long)param_3 + 0x28);
      *(undefined8 *)(IVar1 + 0x28) = uVar3;
      *(undefined8 *)(IVar1 + 0x20) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x38) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47567
// ==== 0x4438b70  ArcAutocompleteMatch::initWithAutocompleteMatch:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAutocompleteMatch::initWithAutocompleteMatch_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcAutocompleteMatch;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x28) == '\0') goto LAB_04438bd0;
  plVar4 = *(long **)(IVar2 + 0x20);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04438bcc;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_04438bcc:
  *(undefined1 *)(IVar2 + 0x28) = 0;
LAB_04438bd0:
  *(undefined8 *)(IVar2 + 0x18) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x20) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x28) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47626
// ==== 0x4438c94  ArcAutocompleteMatch::destinationUrl  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::destinationUrl(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  ID IVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    puVar2 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x18) + 0x68);
    if (*(char *)((long)puVar2 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar2,puVar2[1]);
    }
    else {
      uStack_38 = puVar2[1];
      local_40 = *puVar2;
      local_30 = puVar2[2];
    }
    IVar3 = FUN_043abd34(&local_40);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4438d20);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47665
// ==== 0x4438d44  ArcAutocompleteMatch::destinationUrlSpec  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::destinationUrlSpec(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ID IVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    puVar4 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x18) + 0x68);
    if (*(char *)((long)puVar4 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar4,puVar4[1]);
    }
    else {
      lStack_38 = puVar4[1];
      local_40 = (undefined1 *)*puVar4;
      local_30 = puVar4[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar5 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x4438de8);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47714
// ==== 0x4438e0c  ArcAutocompleteMatch::imageUrl  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::imageUrl(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  ID IVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    puVar2 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x18) + 0x188);
    if (*(char *)((long)puVar2 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar2,puVar2[1]);
    }
    else {
      uStack_38 = puVar2[1];
      local_40 = *puVar2;
      local_30 = puVar2[2];
    }
    IVar3 = FUN_043abd34(&local_40);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4438e98);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47753
// ==== 0x4438ebc  ArcAutocompleteMatch::imageUrlSpec  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::imageUrlSpec(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ID IVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    puVar4 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x18) + 0x188);
    if (*(char *)((long)puVar4 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar4,puVar4[1]);
    }
    else {
      lStack_38 = puVar4[1];
      local_40 = (undefined1 *)*puVar4;
      local_30 = puVar4[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar5 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x4438f60);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47802
// ==== 0x4438f84  ArcAutocompleteMatch::relevance  (24 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAutocompleteMatch::relevance(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return *(int *)(*(long *)(param_1 + 0x18) + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4438f9c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47820
// ==== 0x4438f9c  ArcAutocompleteMatch::contents  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::contents(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x2af) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x298),*(undefined8 *)(lVar4 + 0x2a0));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x2a0);
      local_40 = *(undefined1 **)(lVar4 + 0x298);
      local_30 = *(long *)(lVar4 + 0x2a8);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439040);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47868
// ==== 0x4439064  ArcAutocompleteMatch::matchType  (152 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::matchType(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  uint uVar2;
  code *pcVar3;
  ID IVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined7 local_30;
  char cStack_29;
  undefined1 uStack_21;
  
  if (((*(byte *)(param_1 + 0x28) & 1) != 0) &&
     (uVar2 = *(uint *)(*(long *)(param_1 + 0x18) + 0x3f4), uVar2 < 0x29)) {
    FUN_00b89b80(&local_40,(&PTR_s_url_what_you_typed_0e1dcfc8)[uVar2],&uStack_21);
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)cStack_29) {
      ppuVar1 = &local_40;
    }
    if (-1 < cStack_29) {
      lStack_38 = (long)cStack_29;
    }
    IVar4 = FUN_01c391bc(ppuVar1,lStack_38);
    if (cStack_29 < '\0') {
      FUN_00005810(local_40,CONCAT17(cStack_29,local_30) & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x44390fc);
  (*pcVar3)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47910
// ==== 0x4439120  ArcAutocompleteMatch::providerType  (124 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::providerType(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    FUN_0433190c(&local_38,param_1 + 0x18);
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443919c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47949
// ==== 0x44391c0  ArcAutocompleteMatch::inlineAutocompleteText  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::inlineAutocompleteText(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x5f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x48),*(undefined8 *)(lVar4 + 0x50));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x50);
      local_40 = *(undefined1 **)(lVar4 + 0x48);
      local_30 = *(long *)(lVar4 + 0x58);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x443925c);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:47997
// ==== 0x4439280  ArcAutocompleteMatch::description  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::description(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x2df) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x2c8),*(undefined8 *)(lVar4 + 0x2d0));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x2d0);
      local_40 = *(undefined1 **)(lVar4 + 0x2c8);
      local_30 = *(long *)(lVar4 + 0x2d8);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439324);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48045
// ==== 0x4439348  ArcAutocompleteMatch::additionalData  (464 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Type propagation algorithm not settling */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::additionalData(ID param_1,SEL param_2)

{
  undefined8 *******pppppppuVar1;
  undefined8 ******ppppppuVar2;
  char cVar3;
  code *pcVar4;
  bool bVar5;
  ID self;
  ID IVar6;
  ID IVar7;
  ID IVar8;
  long lVar9;
  undefined8 *******pppppppuVar10;
  undefined8 *******pppppppuVar11;
  undefined8 *******local_80;
  undefined8 *******local_78;
  undefined8 local_70;
  undefined1 *local_68;
  
  if ((*(byte *)(param_1 + 0x28) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4439518);
    (*pcVar4)();
  }
  lVar9 = *(long *)(param_1 + 0x18);
  local_78 = (undefined8 *******)0x0;
  local_70 = 0;
  local_80 = &local_78;
  if (*(long *)(lVar9 + 0x4f8) != 0) {
    local_78 = (undefined8 *******)FUN_000d14e0(&local_80,*(undefined8 *)(lVar9 + 0x4f0),&local_80);
    local_78[2] = &local_78;
    pppppppuVar11 = local_78;
    do {
      local_80 = pppppppuVar11;
      pppppppuVar11 = (undefined8 *******)*local_80;
    } while ((undefined8 *******)*local_80 != (undefined8 *******)0x0);
    local_70 = *(undefined8 *)(lVar9 + 0x4f8);
  }
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_70);
  pppppppuVar11 = local_80;
  while ((undefined8 ********)pppppppuVar11 != &local_78) {
    IVar6 = _objc_alloc(&objc::class_t::ArcAutocompleteMatchAdditionalData);
    cVar3 = *(char *)((long)pppppppuVar11 + 0x37);
    pppppppuVar1 = (undefined8 *******)pppppppuVar11[4];
    if (-1 < (long)cVar3) {
      pppppppuVar1 = pppppppuVar11 + 4;
    }
    ppppppuVar2 = pppppppuVar11[5];
    if (-1 < cVar3) {
      ppppppuVar2 = (undefined8 ******)(long)cVar3;
    }
    IVar7 = FUN_01c391bc(pppppppuVar1,ppppppuVar2);
    cVar3 = *(char *)((long)pppppppuVar11 + 0x4f);
    pppppppuVar1 = (undefined8 *******)pppppppuVar11[7];
    if (-1 < (long)cVar3) {
      pppppppuVar1 = pppppppuVar11 + 7;
    }
    ppppppuVar2 = pppppppuVar11[8];
    if (-1 < cVar3) {
      ppppppuVar2 = (undefined8 ******)(long)cVar3;
    }
    IVar8 = FUN_01c391bc(pppppppuVar1,ppppppuVar2);
    IVar6 = ArcAutocompleteMatchAdditionalData::init_value_(IVar6,"init:value:",IVar7,IVar8);
    NSMutableArray::addObject_(self,"addObject:",IVar6);
    pppppppuVar1 = (undefined8 *******)pppppppuVar11[1];
    pppppppuVar10 = pppppppuVar11;
    if ((undefined8 *******)pppppppuVar11[1] == (undefined8 *******)0x0) {
      do {
        pppppppuVar11 = (undefined8 *******)pppppppuVar10[2];
        bVar5 = pppppppuVar10 != (undefined8 *******)*pppppppuVar11;
        pppppppuVar10 = pppppppuVar11;
      } while (bVar5);
    }
    else {
      do {
        pppppppuVar11 = pppppppuVar1;
        pppppppuVar1 = (undefined8 *******)*pppppppuVar11;
      } while ((undefined8 *******)*pppppppuVar11 != (undefined8 *******)0x0);
    }
  }
  local_68 = (undefined1 *)&local_80;
  FUN_003f9ab0(&local_68,local_78);
  return self;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48142
// ==== 0x443953c  ArcAutocompleteMatch::hasTabMatch  (44 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::hasTabMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  uint uVar2;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    if (*(char *)(*(long *)(param_1 + 0x18) + 0x419) == '\0') {
      uVar2 = 0;
    }
    else {
      uVar2 = (uint)*(byte *)(*(long *)(param_1 + 0x18) + 0x418);
    }
    return uVar2 & 1;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4439568);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48167
// ==== 0x4439568  ArcAutocompleteMatch::allowedToBeDefaultMatch  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::allowedToBeDefaultMatch(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 100);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4439580);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48185
// ==== 0x4439580  ArcAutocompleteMatch::deletable  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::deletable(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x14);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4439598);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48203
// ==== 0x4439598  ArcAutocompleteMatch::swapContentsAndDescription  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::swapContentsAndDescription(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x330);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44395b0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48221
// ==== 0x44395b0  ArcAutocompleteMatch::isFromPrevious  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::isFromPrevious(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x478);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44395c8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48239
// ==== 0x44395c8  ArcAutocompleteMatch::isFromKeyword  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcAutocompleteMatch::isFromKeyword(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 0x18) + 0x450);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44395e0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48257
// ==== 0x44395e0  ArcAutocompleteMatch::keyword  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::keyword(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x44f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x438),*(undefined8 *)(lVar4 + 0x440));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x440);
      local_40 = *(undefined1 **)(lVar4 + 0x438);
      local_30 = *(long *)(lVar4 + 0x448);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439684);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48305
// ==== 0x44396a8  ArcAutocompleteMatch::fillIntoEdit  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::fillIntoEdit(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x2f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x18),*(undefined8 *)(lVar4 + 0x20));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x20);
      local_40 = *(undefined1 **)(lVar4 + 0x18);
      local_30 = *(long *)(lVar4 + 0x28);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439744);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48353
// ==== 0x4439768  ArcAutocompleteMatch::fillIntoEditAdditionalText  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::fillIntoEditAdditionalText(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x47) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x30),*(undefined8 *)(lVar4 + 0x38));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x38);
      local_40 = *(undefined1 **)(lVar4 + 0x30);
      local_30 = *(long *)(lVar4 + 0x40);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439804);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48401
// ==== 0x4439828  ArcAutocompleteMatch::transition  (124 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::transition(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    FUN_04331c6c(&local_38,param_1 + 0x18);
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x44398a4);
  (*pcVar2)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48440
// ==== 0x44398c8  ArcAutocompleteMatch::duplicates  (484 bytes)

/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::duplicates(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  ID IVar4;
  ID IVar5;
  code *pcVar6;
  long *plVar7;
  long lVar8;
  const_void__ local_78;
  long local_70;
  long local_68;
  
  IVar4 = *(ID *)(param_1 + 8);
  if (IVar4 == 0) {
    if ((*(byte *)(param_1 + 0x28) & 1) == 0) {
LAB_04439aa8:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x4439aac);
      (*pcVar6)();
    }
    FUN_04331ae4(&local_78,*(undefined8 *)(param_1 + 0x18));
    IVar4 = NSMutableArray::arrayWithCapacity_
                      ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_70);
    pvVar1 = local_78;
    pvVar2 = local_78;
    for (lVar8 = local_70 << 4; local_78 = pvVar2, lVar8 != 0; lVar8 = lVar8 + -0x10) {
      IVar5 = _objc_alloc(&objc::class_t::ArcAutocompleteMatch);
      IVar5 = initWithAutocompleteMatch_(IVar5,"initWithAutocompleteMatch:",pvVar1);
      NSMutableArray::addObject_(IVar4,"addObject:",IVar5);
      pvVar1 = (const_void__)((long)pvVar1 + 0x10);
      pvVar2 = local_78;
    }
    *(ID *)(param_1 + 8) = IVar4;
    if (pvVar2 != (const_void__)0x0) {
      if (local_70 * 0x10 != 0) {
        lVar8 = local_70 * -0x10;
        pvVar1 = (const_void__)(local_70 * 0x10 + (long)pvVar2);
        do {
          if ((const_void__)((long)pvVar1 + -0x10) == (const_void__)0x0) goto LAB_04439aa8;
          plVar7 = *(long **)((long)pvVar1 + -8);
          if (plVar7 != (long *)0x0) {
            LOAcquire();
            lVar3 = plVar7[1];
            plVar7[1] = lVar3 + -1;
            LORelease();
            if (lVar3 == 0) {
              (**(code **)(*plVar7 + 0x10))(plVar7);
              if (plVar7[2] != 0) {
                LOAcquire();
                lVar3 = plVar7[2];
                plVar7[2] = lVar3 + -1;
                LORelease();
                if (lVar3 != 0) goto LAB_04439a2c;
              }
              pcVar6 = *(code **)(*plVar7 + 0x20);
              if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar7,0x20);
              }
              else {
                (*pcVar6)(plVar7);
              }
            }
          }
LAB_04439a2c:
          lVar8 = lVar8 + 0x10;
          pvVar1 = (const_void__)((long)pvVar1 + -0x10);
        } while (lVar8 != 0);
      }
      local_70 = (long)pvVar2 - (long)local_78 >> 4;
      FUN_00005810(local_78,local_68 << 4);
    }
    IVar4 = *(ID *)(param_1 + 8);
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48530
// ==== 0x4439ba8  ArcAutocompleteMatch::associatedKeyword  (296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::associatedKeyword(ID param_1,SEL param_2)

{
  long lVar1;
  const_void__ pvVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  
  if (*(ID *)(param_1 + 0x10) != 0) {
    return *(ID *)(param_1 + 0x10);
  }
  if ((*(byte *)(param_1 + 0x28) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4439cd0);
    (*pcVar4)();
  }
  pvVar2 = (const_void__)FUN_043319d8(*(undefined8 *)(param_1 + 0x18));
  if (pvVar2 == (const_void__)0x0) {
    return 0;
  }
  IVar3 = _objc_alloc(&objc::class_t::ArcAutocompleteMatch);
  IVar3 = initWithAutocompleteMatch_(IVar3,"initWithAutocompleteMatch:",pvVar2);
  *(ID *)(param_1 + 0x10) = IVar3;
  plVar5 = *(long **)((long)pvVar2 + 8);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar5[1];
    plVar5[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar1 = plVar5[2];
        plVar5[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04439c34;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_04439c34:
  FUN_00005810(pvVar2,0x10);
  return *(ID *)(param_1 + 0x10);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48591
// ==== 0x4439d80  ArcAutocompleteMatch::.cxx_destruct  (220 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcAutocompleteMatch::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x28) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x20), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04439db4;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04439db4:
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48634
// ==== 0x4439e5c  ArcAutocompleteMatch::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 0x18) = 0;
  *(undefined1 *)(param_1 + 0x28) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48706
// ==== 0x4439f8c  ArcAIInferenceModel::init:directory:  (264 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcAIInferenceModel::init_directory_(ID param_1,SEL param_2,long_long param_3,ID param_4)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcAIInferenceModel;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    if ((*(char *)(IVar2 + 0x28) != '\0') && (*(char *)(IVar2 + 0x27) < '\0')) {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x20) = 0;
    *(undefined8 *)(IVar2 + 0x18) = 0;
    *(undefined8 *)(IVar2 + 0x10) = 0;
    *(undefined8 *)(IVar2 + 8) = 0;
    *(undefined1 *)(IVar2 + 0x28) = 1;
    *(int *)(IVar2 + 8) = (int)param_3;
    if (param_4 == 0) {
      local_58 = 0;
      uStack_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_01b356b8(&local_58,param_4);
    }
    if ((*(byte *)(IVar2 + 0x28) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a094);
      (*pcVar1)();
    }
    if (*(char *)(IVar2 + 0x27) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 0x10),*(ulong *)(IVar2 + 0x20) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x18) = uStack_50;
    *(undefined8 *)(IVar2 + 0x10) = local_58;
    *(undefined8 *)(IVar2 + 0x20) = local_48;
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48760
// ==== 0x443a094  ArcAIInferenceModel::initWithAIInferenceModel:  (168 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIInferenceModel::initWithAIInferenceModel_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIInferenceModel;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x28) != '\0') {
      if (*(char *)(IVar1 + 0x27) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar1 + 0x10),*(ulong *)(IVar1 + 0x20) & 0x7fffffffffffffff);
      }
      *(undefined1 *)(IVar1 + 0x28) = 0;
    }
    *(undefined4 *)(IVar1 + 8) = *(undefined4 *)param_3;
    if (*(char *)((long)param_3 + 0x1f) < '\0') {
      FUN_00060a60(IVar1 + 0x10,*(undefined8 *)((long)param_3 + 8),
                   *(undefined8 *)((long)param_3 + 0x10));
    }
    else {
      uVar3 = *(undefined8 *)((long)param_3 + 0x10);
      uVar2 = *(undefined8 *)((long)param_3 + 8);
      *(undefined8 *)(IVar1 + 0x20) = *(undefined8 *)((long)param_3 + 0x18);
      *(undefined8 *)(IVar1 + 0x18) = uVar3;
      *(undefined8 *)(IVar1 + 0x10) = uVar2;
    }
    *(undefined1 *)(IVar1 + 0x28) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48801
// ==== 0x443a13c  ArcAIInferenceModel::modelType  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcAIInferenceModel::modelType(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    return (long)*(int *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a150);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48819
// ==== 0x443a150  ArcAIInferenceModel::directory  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIInferenceModel::directory(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x27);
    plVar1 = (long *)*(long *)(param_1 + 0x10);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x10);
    }
    lVar2 = *(long *)(param_1 + 0x18);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a180);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48851
// ==== 0x443a180  ArcAIInferenceModel::ai_inference_model  (112 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIInferenceModel::ai_inference_model
               (optional<arc::AIInferenceModel> *__return_storage_ptr__,ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  __return_storage_ptr__[1].field0_0x0.field0 = '\0';
  if (*(char *)(param_1 + 0x28) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AIInferenceModel> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a21c);
      (*pcVar1)();
    }
    puVar2 = (undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 8);
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0 = *(int *)(param_1 + 8);
    if (*(char *)(param_1 + 0x27) < '\0') {
      FUN_00060a60(puVar2,*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18));
    }
    else {
      uVar3 = *(undefined8 *)(param_1 + 0x10);
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x10) =
           *(undefined8 *)(param_1 + 0x18);
      *puVar2 = uVar3;
      *(undefined8 *)((long)&__return_storage_ptr__->field0_0x0 + 0x18) =
           *(undefined8 *)(param_1 + 0x20);
    }
    __return_storage_ptr__[1].field0_0x0.field0 = '\x01';
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48928
// ==== 0x443a2fc  ArcAIInferenceConfiguration::mutableAIInferenceConfiguration  (28 bytes)

/* Function Stack Size: 0x10 bytes */

AIStreamedInferenceConfiguration *
ArcAIInferenceConfiguration::mutableAIInferenceConfiguration(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x10) & 1) != 0) {
    return (AIStreamedInferenceConfiguration *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48944
// ==== 0x443a318  ArcAIInferenceConfiguration::init:streamAfterEveryNTokens:  (92 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIInferenceConfiguration::init_streamAfterEveryNTokens_
             (ID param_1,SEL param_2,int param_3,int param_4)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIInferenceConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(undefined8 *)(IVar1 + 8) = 0;
    *(undefined1 *)(IVar1 + 0x10) = 1;
    *(int *)(IVar1 + 8) = param_3;
    *(short *)(IVar1 + 0xc) = (short)param_4;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48969
// ==== 0x443a374  ArcAIInferenceConfiguration::initWithAIInferenceConfiguration:  (96 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIInferenceConfiguration::initWithAIInferenceConfiguration_
             (ID param_1,SEL param_2,const_AIStreamedInferenceConfiguration__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIInferenceConfiguration;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x10) != '\0') {
      *(undefined1 *)(IVar1 + 0x10) = 0;
    }
    *(undefined8 *)(IVar1 + 8) = *(undefined8 *)param_3;
    *(undefined1 *)(IVar1 + 0x10) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:48995
// ==== 0x443a3d4  ArcAIInferenceConfiguration::maxTokens  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAIInferenceConfiguration::maxTokens(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x10) & 1) != 0) {
    return *(int *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a3e8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49013
// ==== 0x443a3e8  ArcAIInferenceConfiguration::streamAfterEveryNTokens  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAIInferenceConfiguration::streamAfterEveryNTokens(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x10) & 1) != 0) {
    return (int)*(short *)(param_1 + 0xc);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a3fc);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49031
// ==== 0x443a3fc  ArcAIInferenceConfiguration::ai_inference_configuration  (16 bytes)

/* Function Stack Size: 0x10 bytes */

optional<arc::AIStreamedInferenceConfiguration>
ArcAIInferenceConfiguration::ai_inference_configuration(ID param_1,SEL param_2)

{
  return *(optional<arc::AIStreamedInferenceConfiguration> *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49065
// ==== 0x443a450  ArcAIDistilBertClassifier::mutableAIDistilBertClassifier  (28 bytes)

/* Function Stack Size: 0x10 bytes */

void * ArcAIDistilBertClassifier::mutableAIDistilBertClassifier(ID param_1,SEL param_2)

{
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    return (void *)(param_1 + 8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_04b08a00();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49080
// ==== 0x443a46c  ArcAIDistilBertClassifier::init:loraConfigJSON:configFilePath:classifierHeadFilePath:numClasses:  (644 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

ID ArcAIDistilBertClassifier::init_loraConfigJSON_configFilePath_classifierHeadFilePath_numClasses_
             (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,int param_7)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcAIDistilBertClassifier;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x70) != '\0') {
    FUN_042a4a04(IVar2 + 8);
  }
  *(undefined8 *)(IVar2 + 0x68) = 0;
  *(undefined8 *)(IVar2 + 0x60) = 0;
  *(undefined8 *)(IVar2 + 0x58) = 0;
  *(undefined8 *)(IVar2 + 0x50) = 0;
  *(undefined8 *)(IVar2 + 0x48) = 0;
  *(undefined8 *)(IVar2 + 0x40) = 0;
  *(undefined8 *)(IVar2 + 0x38) = 0;
  *(undefined8 *)(IVar2 + 0x30) = 0;
  *(undefined8 *)(IVar2 + 0x28) = 0;
  *(undefined8 *)(IVar2 + 0x20) = 0;
  *(undefined8 *)(IVar2 + 0x18) = 0;
  *(undefined8 *)(IVar2 + 0x10) = 0;
  *(undefined8 *)(IVar2 + 8) = 0;
  *(undefined1 *)(IVar2 + 0x70) = 1;
  if (param_3 == 0) {
    local_68 = 0;
    uStack_60 = 0;
    local_58 = 0;
  }
  else {
    FUN_01b356b8(&local_68,param_3);
  }
  if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
    if (*(char *)(IVar2 + 0x1f) < '\0') {
      FUN_00005810(*(undefined8 *)(IVar2 + 8),*(ulong *)(IVar2 + 0x18) & 0x7fffffffffffffff);
    }
    *(undefined8 *)(IVar2 + 0x10) = uStack_60;
    *(undefined8 *)(IVar2 + 8) = local_68;
    *(undefined8 *)(IVar2 + 0x18) = local_58;
    if (param_4 == 0) {
      local_68 = 0;
      uStack_60 = 0;
      local_58 = 0;
    }
    else {
      FUN_01b356b8(&local_68,param_4);
    }
    if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
      if (*(char *)(IVar2 + 0x37) < '\0') {
        FUN_00005810(*(undefined8 *)(IVar2 + 0x20),*(ulong *)(IVar2 + 0x30) & 0x7fffffffffffffff);
      }
      *(undefined8 *)(IVar2 + 0x28) = uStack_60;
      *(undefined8 *)(IVar2 + 0x20) = local_68;
      *(undefined8 *)(IVar2 + 0x30) = local_58;
      if (param_5 == 0) {
        local_68 = 0;
        uStack_60 = 0;
        local_58 = 0;
      }
      else {
        FUN_01b356b8(&local_68,param_5);
      }
      if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
        if (*(char *)(IVar2 + 0x4f) < '\0') {
          FUN_00005810(*(undefined8 *)(IVar2 + 0x38),*(ulong *)(IVar2 + 0x48) & 0x7fffffffffffffff);
        }
        *(undefined8 *)(IVar2 + 0x40) = uStack_60;
        *(undefined8 *)(IVar2 + 0x38) = local_68;
        *(undefined8 *)(IVar2 + 0x48) = local_58;
        if (param_6 == 0) {
          local_68 = 0;
          uStack_60 = 0;
          local_58 = 0;
        }
        else {
          FUN_01b356b8(&local_68,param_6);
        }
        if ((*(byte *)(IVar2 + 0x70) & 1) != 0) {
          if (*(char *)(IVar2 + 0x67) < '\0') {
            FUN_00005810(*(undefined8 *)(IVar2 + 0x50),*(ulong *)(IVar2 + 0x60) & 0x7fffffffffffffff
                        );
            *(undefined8 *)(IVar2 + 0x58) = uStack_60;
            *(undefined8 *)(IVar2 + 0x50) = local_68;
            *(undefined8 *)(IVar2 + 0x60) = local_58;
            if ((*(byte *)(IVar2 + 0x70) & 1) == 0) goto LAB_0443a6ec;
          }
          else {
            *(undefined8 *)(IVar2 + 0x58) = uStack_60;
            *(undefined8 *)(IVar2 + 0x50) = local_68;
            *(undefined8 *)(IVar2 + 0x60) = local_58;
          }
          *(short *)(IVar2 + 0x68) = (short)param_7;
          return IVar2;
        }
      }
    }
  }
LAB_0443a6ec:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a6f0);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49201
// ==== 0x443a6f0  ArcAIDistilBertClassifier::initWithAIDistilBertClassifier:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIDistilBertClassifier::initWithAIDistilBertClassifier_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIDistilBertClassifier;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x70) != '\0') {
      FUN_042a4a04(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0x70) = 0;
    }
    FUN_0443a9fc(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0x70) = 1;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49228
// ==== 0x443a764  ArcAIDistilBertClassifier::modelFilePath  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::modelFilePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x1f);
    plVar1 = (long *)*(long *)(param_1 + 8);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 8);
    }
    lVar2 = *(long *)(param_1 + 0x10);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a794);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49260
// ==== 0x443a794  ArcAIDistilBertClassifier::loraConfigJSON  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::loraConfigJSON(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x37);
    plVar1 = (long *)*(long *)(param_1 + 0x20);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x20);
    }
    lVar2 = *(long *)(param_1 + 0x28);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a7c4);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49292
// ==== 0x443a7c4  ArcAIDistilBertClassifier::configFilePath  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::configFilePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x4f);
    plVar1 = (long *)*(long *)(param_1 + 0x38);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x38);
    }
    lVar2 = *(long *)(param_1 + 0x40);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a7f4);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49324
// ==== 0x443a7f4  ArcAIDistilBertClassifier::classifierHeadFilePath  (48 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::classifierHeadFilePath(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID IVar5;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    cVar3 = *(char *)(param_1 + 0x67);
    plVar1 = (long *)*(long *)(param_1 + 0x50);
    if (-1 < (long)cVar3) {
      plVar1 = (long *)(param_1 + 0x50);
    }
    lVar2 = *(long *)(param_1 + 0x58);
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    IVar5 = FUN_01c391bc(plVar1,lVar2);
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x443a824);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49356
// ==== 0x443a824  ArcAIDistilBertClassifier::numClasses  (20 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcAIDistilBertClassifier::numClasses(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x70) & 1) != 0) {
    return (int)*(short *)(param_1 + 0x68);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443a838);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49374
// ==== 0x443a838  ArcAIDistilBertClassifier::ai_distil_bert_classifier  (72 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIDistilBertClassifier::ai_distil_bert_classifier
               (optional<arc::AIDistilBertClassifier> *__return_storage_ptr__,ID param_1,SEL param_2
               )

{
  code *pcVar1;
  
  (__return_storage_ptr__->field0_0x0).field0 = '\0';
  *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 2) = 0;
  if (*(char *)(param_1 + 0x70) != '\0') {
    if (__return_storage_ptr__ == (optional<arc::AIDistilBertClassifier> *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x443a880);
      (*pcVar1)();
    }
    FUN_0443a9fc(__return_storage_ptr__,param_1 + 8);
    *(undefined1 *)((long)&__return_storage_ptr__[1].field0_0x0 + 2) = 1;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49400
// ==== 0x443a89c  ArcAIDistilBertClassifier::.cxx_destruct  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAIDistilBertClassifier::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x70) == '\0') {
    return;
  }
  FUN_042a4a04(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49415
// ==== 0x443a8b0  ArcAIDistilBertClassifier::.cxx_construct  (12 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAIDistilBertClassifier::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x70) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49531
// ==== 0x443ab2c  ArcAccessibilityTreeNode::initWithAccessibilityTreeNode:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAccessibilityTreeNode::initWithAccessibilityTreeNode_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAccessibilityTreeNode;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441daa0(IVar1 + 8,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49553
// ==== 0x443ab84  ArcGPULogMessage::header  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcGPULogMessage::header(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x27);
  plVar1 = (long *)*(long *)(param_1 + 0x10);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x10);
  }
  lVar2 = *(long *)(param_1 + 0x18);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49579
// ==== 0x443aba8  ArcGPULogMessage::message  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcGPULogMessage::message(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x3f);
  plVar1 = (long *)*(long *)(param_1 + 0x28);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x28);
  }
  lVar2 = *(long *)(param_1 + 0x30);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49605
// ==== 0x443abcc  ArcAccessibilityTreeNode::value  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::value(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x57);
  plVar1 = (long *)*(long *)(param_1 + 0x40);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x40);
  }
  lVar2 = *(long *)(param_1 + 0x48);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49631
// ==== 0x443abf0  ArcAccessibilityTreeNode::description  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::description(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x6f);
  plVar1 = (long *)*(long *)(param_1 + 0x58);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x58);
  }
  lVar2 = *(long *)(param_1 + 0x60);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49657
// ==== 0x443ac14  ArcAccessibilityTreeNode::childTreeId  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::childTreeId(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x8f);
  plVar1 = (long *)*(long *)(param_1 + 0x78);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x78);
  }
  lVar2 = *(long *)(param_1 + 0x80);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49683
// ==== 0x443ac38  ArcAccessibilityTreeNode::children  (180 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::children(ID param_1,SEL param_2)

{
  ID self;
  ID IVar1;
  long lVar2;
  uint *puVar3;
  
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                    *(undefined8 *)(param_1 + 0x98));
  lVar2 = *(long *)(param_1 + 0x98) << 2;
  if (lVar2 != 0) {
    puVar3 = *(uint **)(param_1 + 0x90);
    do {
      IVar1 = NSNumber::numberWithInt_((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",(ulong)*puVar3);
      NSMutableArray::addObject_(self,"addObject:",IVar1);
      lVar2 = lVar2 + -4;
      puVar3 = puVar3 + 1;
    } while (lVar2 != 0);
  }
  return self;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49716
// ==== 0x443acec  ArcAccessibilityTreeNode::accessibility_tree_node  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAccessibilityTreeNode::accessibility_tree_node
               (AccessibilityTreeNode *__return_storage_ptr__,ID param_1,SEL param_2)

{
  FUN_0441d950(__return_storage_ptr__,param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49729
// ==== 0x443acf8  ArcAccessibilityTreeNode::setAccessibility_tree_node:  (12 bytes)

/* Function Stack Size: 0xb0 bytes */

void ArcAccessibilityTreeNode::setAccessibility_tree_node_
               (ID param_1,SEL param_2,AccessibilityTreeNode *param_3)

{
  FUN_0441daa0(param_1 + 8,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49742
// ==== 0x443ad04  ArcAccessibilityTreeNode::.cxx_destruct  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAccessibilityTreeNode::_cxx_destruct(ID param_1,SEL param_2)

{
  FUN_0441d874(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49754
// ==== 0x443ad0c  ArcAccessibilityTreeNode::.cxx_construct  (32 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcAccessibilityTreeNode::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x60) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x80) = 0;
  *(undefined8 *)(param_1 + 0x78) = 0;
  *(undefined8 *)(param_1 + 0x90) = 0;
  *(undefined8 *)(param_1 + 0x88) = 0;
  *(undefined8 *)(param_1 + 0xa0) = 0;
  *(undefined8 *)(param_1 + 0x98) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49976
// ==== 0x443afd0  ArcBrowserContextAttributes::initWithBrowserContextAttributes:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContextAttributes::initWithBrowserContextAttributes_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcBrowserContextAttributes;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441e0a0(IVar1 + 0x10,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:49998
// ==== 0x443b028  ArcBrowserContextAttributes::userColor  (84 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextAttributes::userColor(ID param_1,SEL param_2)

{
  ID IVar1;
  
  IVar1 = *(ID *)(param_1 + 8);
  if (IVar1 == 0) {
    IVar1 = _objc_alloc(&objc::class_t::ArcColor);
    IVar1 = ArcColor::initWithColor_(IVar1,"initWithColor:",(const_Color__)(param_1 + 0x40));
    *(ID *)(param_1 + 8) = IVar1;
    IVar1 = *(ID *)(param_1 + 8);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50021
// ==== 0x443b07c  ArcBrowserContextAttributes::browser_context_attributes  (120 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextAttributes::browser_context_attributes
               (BrowserContextAttributes *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (*(char *)(param_1 + 0x27) < '\0') {
    FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 0x10),
                 *(undefined8 *)(param_1 + 0x18));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x10);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x18);
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x20);
  }
  if (*(char *)(param_1 + 0x3f) < '\0') {
    FUN_00060a60(&__return_storage_ptr__->field1_0x18,*(undefined8 *)(param_1 + 0x28),
                 *(undefined8 *)(param_1 + 0x30));
  }
  else {
    pcVar1 = *(char **)(param_1 + 0x28);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field1_0x8 =
         *(unsigned_long_long *)(param_1 + 0x30);
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field1_0x18).field0_0x0.field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x38);
  }
  __return_storage_ptr__->field2_0x30 = *(Color *)(param_1 + 0x40);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50060
// ==== 0x443b0f4  ArcBrowserContextAttributes::setBrowser_context_attributes:  (12 bytes)

/* Function Stack Size: 0x48 bytes */

void ArcBrowserContextAttributes::setBrowser_context_attributes_
               (ID param_1,SEL param_2,BrowserContextAttributes *param_3)

{
  FUN_0441e0a0(param_1 + 0x10,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50073
// ==== 0x443b100  ArcBrowserContextAttributes::.cxx_destruct  (84 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextAttributes::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x3f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
  }
  if (*(char *)(param_1 + 0x27) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(ulong *)(param_1 + 0x20) & 0x7fffffffffffffff);
  }
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50092
// ==== 0x443b154  ArcGPULogMessage::.cxx_construct  (16 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcGPULogMessage::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50109
// ==== 0x443b164  ArcColor::initWithColor:  (76 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcColor::initWithColor_(ID param_1,SEL param_2,const_Color__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcColor;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(Color *)(IVar1 + 8) = *param_3;
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50130
// ==== 0x443b1b0  ArcColor::color  (12 bytes)

/* Function Stack Size: 0x10 bytes */

Color ArcColor::color(ID param_1,SEL param_2)

{
  undefined4 *in_x8;
  
  *in_x8 = *(undefined4 *)(param_1 + 8);
  return SUB84(param_1,0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50144
// ==== 0x443b1bc  ArcColor::setColor:  (12 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcColor::setColor_(ID param_1,SEL param_2,Color param_3)

{
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)(ulong)(uint)param_3;
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50444
// ==== 0x443b578  ArcCrashKeysReporter::setArcCrashKeysObserver:  (440 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcCrashKeysReporter::setArcCrashKeysObserver_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  int *piVar5;
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined8 local_48 [2];
  code *local_38;
  undefined **local_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((DAT_0e53bde0 & 1) == 0) && (iVar2 = thunk_FUN_0c44b610(&DAT_0e53bde0), iVar2 != 0)) {
    puVar4 = (undefined8 *)FUN_00008790(0x48);
    puVar4[5] = 0;
    puVar4[4] = 0;
    puVar4[7] = 0;
    puVar4[6] = 0;
    puVar4[1] = 0;
    *puVar4 = 0;
    puVar4[3] = 0;
    puVar4[2] = 0;
    puVar4[8] = 0;
    puVar4[2] = FUN_040490c0;
    puVar4[3] = &DAT_0c4b3500;
    puVar4[4] = 0;
    puVar4[5] = 0;
    puVar4[6] = 0;
    piVar5 = (int *)FUN_00008790(8);
    *piVar5 = 0;
    *(undefined1 *)(piVar5 + 1) = 0;
    if ((piVar5 != (int *)0x0) && (iVar2 = *piVar5, *piVar5 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x443b784);
      (*pcVar1)();
    }
    puVar4[7] = piVar5;
    puVar4[8] = puVar4;
    DAT_0e53bdd8 = puVar4;
    thunk_FUN_0c44b760(&DAT_0e53bde0);
  }
  if (param_3 == 0) {
    local_38 = FUN_040490c0;
    local_30 = (undefined **)&DAT_0c4b3500;
    FUN_045559f4(&DAT_0e53bdd8,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
  }
  else {
    _objc_initWeak(auStack_50,param_3);
    _objc_copyWeak(auStack_58,auStack_50);
    local_38 = FUN_0443b78c;
    local_30 = &PTR_FUN_0df6c2f8;
    uVar3 = FUN_00008790(8);
    _objc_moveWeak(uVar3,auStack_58);
    local_48[0] = uVar3;
    _objc_destroyWeak(auStack_58);
    FUN_045559f4(&DAT_0e53bdd8,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
    _objc_destroyWeak(auStack_50);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50736
// ==== 0x443ba88  ArcGPULogMessage::severity  (8 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcGPULogMessage::severity(ID param_1,SEL param_2)

{
  return (long)*(int *)(param_1 + 8);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50773
// ==== 0x443baa8  ArcExtensionCommand::initWithExtensionCommand:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtensionCommand::initWithExtensionCommand_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionCommand;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441e2e4(IVar1 + 8,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50794
// ==== 0x443bb00  ArcExtensionCommand::commandDescription  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionCommand::commandDescription(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x4f);
  plVar1 = (long *)*(long *)(param_1 + 0x38);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x38);
  }
  lVar2 = *(long *)(param_1 + 0x40);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01c391bc(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50820
// ==== 0x443bb24  ArcExtensionCommand::shortcut  (180 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionCommand::shortcut(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  ID IVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  lStack_38 = 0;
  local_40 = (undefined1 *)0x0;
  local_28 = 0;
  local_30 = 0;
  if (*(char *)(param_1 + 0x68) == '\0') {
    IVar3 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x67) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(param_1 + 0x50),*(undefined8 *)(param_1 + 0x58));
    }
    else {
      lStack_38 = *(long *)(param_1 + 0x58);
      local_40 = *(undefined1 **)(param_1 + 0x50);
      local_30 = *(ulong *)(param_1 + 0x60);
    }
    local_28 = CONCAT71(local_28._1_7_,1);
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar2);
    if (((char)local_28 != '\0') && ((long)local_30 < 0)) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50872
// ==== 0x443bc08  ArcExtensionCommand::extension_command  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionCommand::extension_command
               (ExtensionCommand *__return_storage_ptr__,ID param_1,SEL param_2)

{
  FUN_0441e200(__return_storage_ptr__,param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50885
// ==== 0x443bc14  ArcExtensionCommand::setExtension_command:  (12 bytes)

/* Function Stack Size: 0x80 bytes */

void ArcExtensionCommand::setExtension_command_(ID param_1,SEL param_2,ExtensionCommand *param_3)

{
  FUN_0441e2e4(param_1 + 8,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50897
// ==== 0x443bc20  ArcExtensionCommand::.cxx_destruct  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionCommand::_cxx_destruct(ID param_1,SEL param_2)

{
  FUN_0441e178(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50909
// ==== 0x443bc28  ArcExtensionCommand::.cxx_construct  (32 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionCommand::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined1 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x49) = 0;
  *(undefined8 *)(param_1 + 0x41) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50931
// ==== 0x443bc48  ArcExtensionToolbarAction::initWithExtensionToolbarAction:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcExtensionToolbarAction::initWithExtensionToolbarAction_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcExtensionToolbarAction;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441e6d8(IVar1 + 8,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50953
// ==== 0x443bca0  ArcExtensionToolbarAction::accessibleName  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::accessibleName(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x37);
  plVar1 = (long *)*(long *)(param_1 + 0x20);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x20);
  }
  lVar2 = *(long *)(param_1 + 0x28);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01d9fb90(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:50979
// ==== 0x443bcc4  ArcExtensionToolbarAction::tooltipText  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::tooltipText(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  
  cVar3 = *(char *)(param_1 + 0x4f);
  plVar1 = (long *)*(long *)(param_1 + 0x38);
  if (-1 < (long)cVar3) {
    plVar1 = (long *)(param_1 + 0x38);
  }
  lVar2 = *(long *)(param_1 + 0x40);
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  IVar4 = FUN_01d9fb90(plVar1,lVar2);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51005
// ==== 0x443bce8  ArcExtensionToolbarAction::icon  (32 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::icon(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((**(long **)(param_1 + 0x58) != 0) && (*(long *)(**(long **)(param_1 + 0x58) + 0x18) != 0)) {
    IVar1 = FUN_02a02ccc();
    return IVar1;
  }
  return 0;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51022
// ==== 0x443bd08  ArcExtensionToolbarAction::pinnedPosition  (56 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::pinnedPosition(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((*(ulong *)(param_1 + 0x6c) >> 0x20 & 1) == 0) {
    return 0;
  }
  IVar1 = NSNumber::numberWithInt_((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:");
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51042
// ==== 0x443bd40  ArcExtensionToolbarAction::extension_toolbar_action  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionToolbarAction::extension_toolbar_action
               (ExtensionToolbarAction *__return_storage_ptr__,ID param_1,SEL param_2)

{
  FUN_0441e600(__return_storage_ptr__,param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51055
// ==== 0x443bd4c  ArcExtensionToolbarAction::setExtension_toolbar_action:  (12 bytes)

/* Function Stack Size: 0x80 bytes */

void ArcExtensionToolbarAction::setExtension_toolbar_action_
               (ID param_1,SEL param_2,ExtensionToolbarAction *param_3)

{
  FUN_0441e6d8(param_1 + 8,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51068
// ==== 0x443bd58  ArcExtensionToolbarAction::.cxx_destruct  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcExtensionToolbarAction::_cxx_destruct(ID param_1,SEL param_2)

{
  FUN_0441e4e8(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51080
// ==== 0x443bd60  ArcExtensionToolbarAction::.cxx_construct  (72 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcExtensionToolbarAction::_cxx_construct(ID param_1,SEL param_2)

{
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  FUN_043386a0(param_1 + 0x50);
  *(undefined1 *)(param_1 + 0x6c) = 0;
  *(undefined1 *)(param_1 + 0x70) = 0;
  return param_1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51419
// ==== 0x443c0a8  ArcGPULogMessage::initWithGPULogMessage:  (88 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcGPULogMessage::initWithGPULogMessage_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcGPULogMessage;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    FUN_0441e818(IVar1 + 8,param_3);
  }
  return IVar1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51440
// ==== 0x443c100  ArcGPULogMessage::gpu_log_message  (124 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcGPULogMessage::gpu_log_message(GPULogMessage *__return_storage_ptr__,ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  
  puVar1 = (undefined8 *)
           ((long)&(__return_storage_ptr__->field1_0x4).field0_0x0.field1.field0_0x0 + 4);
  __return_storage_ptr__->field0_0x0 = *(int *)(param_1 + 8);
  if (*(char *)(param_1 + 0x27) < '\0') {
    FUN_00060a60(puVar1,*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18));
  }
  else {
    uVar2 = *(undefined8 *)(param_1 + 0x10);
    *(undefined8 *)((long)&(__return_storage_ptr__->field1_0x4).field0_0x0.field1.field1_0x8 + 4) =
         *(undefined8 *)(param_1 + 0x18);
    *puVar1 = uVar2;
    *(undefined8 *)((long)&(__return_storage_ptr__->field1_0x4).field0_0x0.field1.field2_0x10 + 4) =
         *(undefined8 *)(param_1 + 0x20);
  }
  if (-1 < *(char *)(param_1 + 0x3f)) {
    uVar2 = *(undefined8 *)(param_1 + 0x28);
    *(undefined8 *)((long)&(__return_storage_ptr__->field2_0x1c).field0_0x0.field1.field1_0x8 + 4) =
         *(undefined8 *)(param_1 + 0x30);
    *(undefined8 *)((long)&(__return_storage_ptr__->field2_0x1c).field0_0x0.field1.field0_0x0 + 4) =
         uVar2;
    *(undefined8 *)((long)&(__return_storage_ptr__->field2_0x1c).field0_0x0.field1.field2_0x10 + 4)
         = *(undefined8 *)(param_1 + 0x38);
    return;
  }
  FUN_00060a60((undefined1 *)((long)&(__return_storage_ptr__->field2_0x1c).field0_0x0 + 4),
               *(undefined8 *)(param_1 + 0x28),*(undefined8 *)(param_1 + 0x30));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51480
// ==== 0x443c17c  ArcGPULogMessage::setGpu_log_message:  (12 bytes)

/* Function Stack Size: 0x48 bytes */

void ArcGPULogMessage::setGpu_log_message_(ID param_1,SEL param_2,GPULogMessage *param_3)

{
  FUN_0441e818(param_1 + 8,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51492
// ==== 0x443c188  ArcGPULogMessage::.cxx_destruct  (84 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcGPULogMessage::_cxx_destruct(ID param_1,SEL param_2)

{
  if (*(char *)(param_1 + 0x3f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 0x28),*(ulong *)(param_1 + 0x38) & 0x7fffffffffffffff);
  }
  if (-1 < *(char *)(param_1 + 0x27)) {
    return;
  }
  FUN_00005810(*(undefined8 *)(param_1 + 0x10),*(ulong *)(param_1 + 0x20) & 0x7fffffffffffffff);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51510
// ==== 0x443c1dc  ArcMediaPosition::initWithMediaPosition:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcMediaPosition::initWithMediaPosition_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcMediaPosition;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0443c23c;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443c238;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443c238:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0443c23c:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51569
// ==== 0x443c300  ArcMediaPosition::playbackRate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcMediaPosition::playbackRate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return **(double **)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443c318);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51587
// ==== 0x443c318  ArcMediaPosition::duration  (84 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcMediaPosition::duration(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  double dVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443c36c);
    (*pcVar1)();
  }
  lVar2 = *(long *)(*(long *)(param_1 + 8) + 8);
  if (1 < lVar2 + 0x8000000000000001U) {
    return (double)lVar2 / 1000000.0;
  }
  dVar3 = -INFINITY;
  if (-1 < lVar2) {
    dVar3 = INFINITY;
  }
  return dVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51615
// ==== 0x443c36c  ArcMediaPosition::currentTime  (96 bytes)

/* Function Stack Size: 0x10 bytes */

double ArcMediaPosition::currentTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  double dVar3;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443c3cc);
    (*pcVar1)();
  }
  lVar2 = FUN_032b5dd0(*(undefined8 *)(param_1 + 8));
  if (1 < lVar2 + 0x8000000000000001U) {
    return (double)lVar2 / 1000000.0;
  }
  dVar3 = -INFINITY;
  if (-1 < lVar2) {
    dVar3 = INFINITY;
  }
  return dVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51643
// ==== 0x443c3cc  ArcMediaPosition::endOfMedia  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcMediaPosition::endOfMedia(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x20);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443c3e4);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51849
// ==== 0x443c65c  ArcPerformanceController::isBatterySaverModeActive  (108 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcPerformanceController::isBatterySaverModeActive(ID param_1,SEL param_2)

{
  int iVar1;
  uint uVar2;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    FUN_00008790(0x40);
    DAT_0e4a2f18 = FUN_0433d1bc();
    thunk_FUN_0c44b760(&DAT_0e4a2f20);
  }
  uVar2 = 0;
  if (*(long *)(DAT_0e49d480 + 0x18) != 0) {
    uVar2 = (uint)*(byte *)(*(long *)(DAT_0e49d480 + 0x18) + 0x28);
  }
  return uVar2 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51872
// ==== 0x443c6c8  ArcPerformanceController::isBatterySaverModeTemporarilyDisabled  (108 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcPerformanceController::isBatterySaverModeTemporarilyDisabled(ID param_1,SEL param_2)

{
  int iVar1;
  uint uVar2;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    FUN_00008790(0x40);
    DAT_0e4a2f18 = FUN_0433d1bc();
    thunk_FUN_0c44b760(&DAT_0e4a2f20);
  }
  uVar2 = 0;
  if (*(long *)(DAT_0e49d480 + 0x18) != 0) {
    uVar2 = (uint)*(byte *)(*(long *)(DAT_0e49d480 + 0x18) + 0x29);
  }
  return uVar2 & 1;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51895
// ==== 0x443c734  ArcPerformanceController::simulateLowBatteryStateForDuration:  (120 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::simulateLowBatteryStateForDuration_
               (ID param_1,SEL param_2,unsigned_int param_3)

{
  int iVar1;
  
  if ((DAT_0e4a2f20 & 1) == 0) {
    iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20);
    if (iVar1 != 0) {
      FUN_00008790(0x40,param_3);
      DAT_0e4a2f18 = FUN_0433d1bc();
      thunk_FUN_0c44b760(&DAT_0e4a2f20);
    }
  }
  FUN_04208bd4(*(undefined8 *)(DAT_0e49d480 + 0x18),param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:51918
// ==== 0x443c7ac  ArcPerformanceController::setArcPerformanceObserver:  (348 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcPerformanceController::setArcPerformanceObserver_(ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined8 local_48 [2];
  code *local_38;
  undefined **local_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = thunk_FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    FUN_00008790(0x40);
    DAT_0e4a2f18 = FUN_0433d1bc();
    thunk_FUN_0c44b760(&DAT_0e4a2f20);
  }
  if (param_3 == 0) {
    local_38 = FUN_040490c0;
    local_30 = (undefined **)&DAT_0c4b3500;
    FUN_0433dcf0(DAT_0e4a2f18,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
  }
  else {
    _objc_initWeak(auStack_50,param_3);
    _objc_copyWeak(auStack_58,auStack_50);
    local_38 = FUN_0443c924;
    local_30 = &PTR_FUN_0df6c318;
    uVar2 = FUN_00008790(8);
    _objc_moveWeak(uVar2,auStack_58);
    local_48[0] = uVar2;
    _objc_destroyWeak(auStack_58);
    FUN_0433dcf0(DAT_0e4a2f18,local_48);
    if ((code *)local_30[1] != (code *)0x0) {
      (*(code *)local_30[1])(local_48[0]);
    }
    _objc_destroyWeak(auStack_50);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52162
// ==== 0x443cb9c  ArcSessionTab::initWithSessionTab:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSessionTab::initWithSessionTab_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcSessionTab;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0443cbfc;
  plVar4 = *(long **)(IVar2 + 0x18);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443cbf8;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443cbf8:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0443cbfc:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52221
// ==== 0x443ccc0  ArcSessionTab::isPinned  (28 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcSessionTab::isPinned(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (bool)*(byte *)(**(long **)(param_1 + 0x10) + 0x40);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ccdc);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52239
// ==== 0x443ccdc  ArcSessionTab::currentNavigationIndex  (28 bytes)

/* Function Stack Size: 0x10 bytes */

int ArcSessionTab::currentNavigationIndex(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return *(int *)(**(long **)(param_1 + 0x10) + 0xc);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ccf8);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52257
// ==== 0x443ccf8  ArcSessionTab::setCurrentNavigationIndex:  (60 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcSessionTab::setCurrentNavigationIndex_(ID param_1,SEL param_2,int param_3)

{
  code *pcVar1;
  ulong uVar2;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    if ((int)param_3 < 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x443cd30);
      (*pcVar1)();
    }
    uVar2 = *(ulong *)(**(long **)(param_1 + 0x10) + 0xb0);
    if (uVar2 >> 0x1f == 0) {
      if (param_3 < (uint)uVar2) {
        *(int *)(**(long **)(param_1 + 0x10) + 0xc) = param_3;
        return;
      }
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x443cd3c);
      (*pcVar1)();
    }
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443cd2c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52290
// ==== 0x443cd44  ArcSessionTab::lastActiveTime  (164 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSessionTab::lastActiveTime(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  double dVar5;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443cde8);
    (*pcVar2)();
  }
  lVar4 = *(long *)(**(long **)(param_1 + 0x10) + 0xa0);
  if (lVar4 == 0) {
    dVar5 = 0.0;
  }
  else if (lVar4 + 0x8000000000000001U < 2) {
    dVar5 = -INFINITY;
    if (-1 < lVar4) {
      dVar5 = INFINITY;
    }
  }
  else {
    lVar1 = -0x8000000000000000;
    if (!SCARRY8(lVar4,-0x295e9648864000)) {
      lVar1 = lVar4 + -0x295e9648864000;
    }
    if (lVar1 == -0x8000000000000000) {
      dVar5 = -INFINITY;
    }
    else {
      dVar5 = (double)lVar1 / 1000000.0;
    }
  }
  IVar3 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",dVar5);
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52336
// ==== 0x443cde8  ArcSessionTab::navigations  (484 bytes)

/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionTab::navigations(ID param_1,SEL param_2)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  ID IVar4;
  ID IVar5;
  code *pcVar6;
  long *plVar7;
  long lVar8;
  const_void__ local_78;
  long local_70;
  long local_68;
  
  IVar4 = *(ID *)(param_1 + 8);
  if (IVar4 == 0) {
    if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
LAB_0443cfc8:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x443cfcc);
      (*pcVar6)();
    }
    FUN_04340aa4(&local_78,*(undefined8 *)(param_1 + 0x10));
    IVar4 = NSMutableArray::arrayWithCapacity_
                      ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_70);
    pvVar1 = local_78;
    pvVar2 = local_78;
    for (lVar8 = local_70 << 4; local_78 = pvVar2, lVar8 != 0; lVar8 = lVar8 + -0x10) {
      IVar5 = _objc_alloc(&objc::class_t::ArcSessionNavigation);
      IVar5 = ArcSessionNavigation::initWithSessionNavigation_
                        (IVar5,"initWithSessionNavigation:",pvVar1);
      NSMutableArray::addObject_(IVar4,"addObject:",IVar5);
      pvVar1 = (const_void__)((long)pvVar1 + 0x10);
      pvVar2 = local_78;
    }
    *(ID *)(param_1 + 8) = IVar4;
    if (pvVar2 != (const_void__)0x0) {
      if (local_70 * 0x10 != 0) {
        lVar8 = local_70 * -0x10;
        pvVar1 = (const_void__)(local_70 * 0x10 + (long)pvVar2);
        do {
          if ((const_void__)((long)pvVar1 + -0x10) == (const_void__)0x0) goto LAB_0443cfc8;
          plVar7 = *(long **)((long)pvVar1 + -8);
          if (plVar7 != (long *)0x0) {
            LOAcquire();
            lVar3 = plVar7[1];
            plVar7[1] = lVar3 + -1;
            LORelease();
            if (lVar3 == 0) {
              (**(code **)(*plVar7 + 0x10))(plVar7);
              if (plVar7[2] != 0) {
                LOAcquire();
                lVar3 = plVar7[2];
                plVar7[2] = lVar3 + -1;
                LORelease();
                if (lVar3 != 0) goto LAB_0443cf4c;
              }
              pcVar6 = *(code **)(*plVar7 + 0x20);
              if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar7,0x20);
              }
              else {
                (*pcVar6)(plVar7);
              }
            }
          }
LAB_0443cf4c:
          lVar8 = lVar8 + 0x10;
          pvVar1 = (const_void__)((long)pvVar1 + -0x10);
        } while (lVar8 != 0);
      }
      local_70 = (long)pvVar2 - (long)local_78 >> 4;
      FUN_00005810(local_78,local_68 << 4);
    }
    IVar4 = *(ID *)(param_1 + 8);
  }
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52427
// ==== 0x443d0c8  ArcSessionTab::setNavigations:  (1108 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcSessionTab::setNavigations_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ID IVar4;
  ID self;
  undefined8 *puVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  ulong uVar9;
  long lVar10;
  long local_180;
  long local_178;
  long lStack_170;
  undefined8 local_168;
  long *local_160;
  byte local_158;
  undefined8 local_150;
  long lStack_148;
  long *local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  long local_108;
  long local_100;
  long lStack_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  *(ID *)(param_1 + 8) = param_3;
  local_108 = 0;
  local_100 = 0;
  lStack_f8 = 0;
  lStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  local_140 = (long *)0x0;
  uStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  uStack_120 = 0;
  IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_150,auStack_f0,
                        0x10);
  if (IVar4 != 0) {
    uVar9 = 0;
    lVar10 = *local_140;
    lVar2 = lVar10;
    do {
      if (lVar2 != lVar10) {
        _objc_enumerationMutation(param_3);
      }
      self = *(ID *)(lStack_148 + uVar9 * 8);
      if ((self == 0) || (_objc_msgSend(self,"session_navigation",&local_168), (local_158 & 1) == 0)
         ) goto LAB_0443d32c;
      if (local_100 == lStack_f8) {
        puVar5 = (undefined8 *)FUN_041e7c00(&local_108,&local_168);
      }
      else {
        puVar5 = (undefined8 *)(local_108 + local_100 * 0x10);
        if (puVar5 == (undefined8 *)0x0) goto LAB_0443d32c;
        *puVar5 = local_168;
        puVar5[1] = local_160;
        local_168 = 0;
        local_160 = (long *)0x0;
        puVar5 = puVar5 + 2;
      }
      plVar7 = local_160;
      local_100 = (long)puVar5 - local_108 >> 4;
      if ((long)puVar5 - local_108 == 0) goto LAB_0443d32c;
      if ((local_158 != 0) && (local_160 != (long *)0x0)) {
        LOAcquire();
        lVar2 = local_160[1];
        local_160[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          plVar8 = local_160 + 2;
          (**(code **)(*local_160 + 0x10))(local_160);
          if (*plVar8 != 0) {
            LOAcquire();
            lVar2 = plVar7[2];
            plVar7[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_0443d1f4;
          }
          pcVar6 = *(code **)(*plVar7 + 0x20);
          if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
            FUN_00005810(plVar7,0x20);
          }
          else {
            (*pcVar6)(plVar7);
          }
        }
      }
LAB_0443d1f4:
      uVar9 = uVar9 + 1;
      if (IVar4 <= uVar9) {
        IVar4 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_150,
                              auStack_f0,0x10);
        if (IVar4 == 0) break;
        uVar9 = 0;
      }
      lVar2 = *local_140;
    } while( true );
  }
  lVar10 = lStack_f8;
  lVar2 = local_100;
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
LAB_0443d32c:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x443d330);
    (*pcVar6)();
  }
  local_180 = local_108;
  local_108 = 0;
  local_100 = 0;
  lStack_f8 = 0;
  lStack_170 = lVar10;
  local_178 = lVar2;
  FUN_04340c10(param_1 + 0x10,&local_180);
  lVar2 = local_180;
  if (local_180 != 0) {
    if (local_178 * 0x10 != 0) {
      lVar10 = local_178 * -0x10;
      lVar1 = local_178 * 0x10 + local_180;
      do {
        if (lVar1 + -0x10 == 0) goto LAB_0443d32c;
        plVar7 = *(long **)(lVar1 + -8);
        if (plVar7 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar7[1];
          plVar7[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar7 + 0x10))(plVar7);
            if (plVar7[2] != 0) {
              LOAcquire();
              lVar3 = plVar7[2];
              plVar7[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_0443d33c;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_0443d33c:
        lVar10 = lVar10 + 0x10;
        lVar1 = lVar1 + -0x10;
      } while (lVar10 != 0);
    }
    local_178 = lVar2 - local_180 >> 4;
    FUN_00005810(local_180,lStack_170 << 4);
  }
  lVar2 = local_108;
  if (local_108 != 0) {
    if (local_100 * 0x10 != 0) {
      lVar10 = local_100 * -0x10;
      lVar1 = local_100 * 0x10 + local_108;
      do {
        if (lVar1 + -0x10 == 0) goto LAB_0443d32c;
        plVar7 = *(long **)(lVar1 + -8);
        if (plVar7 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar7[1];
          plVar7[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar7 + 0x10))(plVar7);
            if (plVar7[2] != 0) {
              LOAcquire();
              lVar3 = plVar7[2];
              plVar7[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_0443d428;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_0443d428:
        lVar10 = lVar10 + 0x10;
        lVar1 = lVar1 + -0x10;
      } while (lVar10 != 0);
    }
    local_100 = lVar2 - local_108 >> 4;
    FUN_00005810(local_108,lStack_f8 << 4);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_70) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:52647
// ==== 0x443d7b4  ArcSessionTab::groupId  (256 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionTab::groupId(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  long lVar3;
  ID IVar4;
  undefined1 auVar5 [16];
  undefined1 *local_70;
  long lStack_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_48;
  undefined *puStack_40;
  undefined8 local_38;
  undefined *puStack_30;
  long local_28;
  
  auVar5._8_8_ = param_2;
  auVar5._0_8_ = param_1;
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443d8b0);
    (*pcVar2)();
  }
  lStack_68 = 0;
  local_70 = (undefined1 *)0x0;
  local_58 = 0;
  local_60 = 0;
  lVar3 = **(long **)(param_1 + 0x10);
  if (*(char *)(lVar3 + 0x20) == '\0') {
    IVar4 = 0;
  }
  else {
    local_48 = *(undefined8 *)(lVar3 + 0x10);
    puStack_40 = &DAT_01115580;
    local_38 = *(undefined8 *)(lVar3 + 0x18);
    puStack_30 = &DAT_01115580;
    FUN_00331000(&local_70,"%016llX%016llX",0xe,&local_48,2);
    local_58 = CONCAT71(local_58._1_7_,1);
    ppuVar1 = (undefined1 **)local_70;
    if (-1 < (long)local_60._7_1_) {
      ppuVar1 = &local_70;
    }
    lVar3 = lStack_68;
    if (-1 < (long)local_60) {
      lVar3 = (long)local_60._7_1_;
    }
    auVar5 = FUN_01c391bc(ppuVar1,lVar3);
    IVar4 = auVar5._0_8_;
    if (((char)local_58 != '\0') && ((long)local_60 < 0)) {
      auVar5 = FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
    }
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return IVar4;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(auVar5._0_8_,auVar5._8_8_);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53027
// ==== 0x443dd58  ArcURLUtilities::effectiveTopLevelDomainFromHost:includePrivateRegistries:  (240 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x1c bytes */

ID ArcURLUtilities::effectiveTopLevelDomainFromHost_includePrivateRegistries_
             (ID param_1,SEL param_2,ID param_3,bool param_4)

{
  ulong uVar1;
  undefined8 *****pppppuVar2;
  uint uVar3;
  ID IVar4;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 ****local_38;
  ulong local_30;
  undefined8 local_28;
  
  local_38 = (undefined8 *****)0x0;
  local_30 = 0;
  local_28 = 0;
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
  }
  FUN_04345480(&local_38,&local_50,param_4);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  uVar3 = (uint)(char)local_28._7_1_;
  uVar1 = local_30;
  if (-1 < (int)uVar3) {
    uVar1 = (ulong)local_28._7_1_;
  }
  if (uVar1 == 0) {
    IVar4 = 0;
    if ((uVar3 >> 7 & 1) == 0) {
      return 0;
    }
  }
  else {
    pppppuVar2 = (undefined8 *****)local_38;
    if (-1 < (int)uVar3) {
      pppppuVar2 = &local_38;
    }
    IVar4 = FUN_01c391bc(pppppuVar2);
    if (-1 < (long)local_28) {
      return IVar4;
    }
  }
  FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
  return IVar4;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53092
// ==== 0x443de84  ArcURLUtilities::formatHostForDisplay:  (200 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcURLUtilities::formatHostForDisplay_(ID param_1,SEL param_2,ID param_3)

{
  undefined8 ***pppuVar1;
  ID IVar2;
  SEL extraout_x1;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if (param_3 == 0) {
    local_50 = 0;
    uStack_48 = 0;
    local_40 = 0;
  }
  else {
    FUN_01b356b8(&local_50,param_3);
    param_2 = extraout_x1;
  }
  FUN_0434563c(&local_38,&local_50,param_2);
  pppuVar1 = (undefined8 ***)local_38;
  if (-1 < (long)cStack_21) {
    pppuVar1 = &local_38;
  }
  if (-1 < cStack_21) {
    lStack_30 = (long)cStack_21;
  }
  IVar2 = FUN_01c391bc(pppuVar1,lStack_30);
  if (cStack_21 < '\0') {
    FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
  }
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53776
// ==== 0x443ebe4  ArcAddressEntrySaveRequest::initWithAddressEntrySaveRequest:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAddressEntrySaveRequest::initWithAddressEntrySaveRequest_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcAddressEntrySaveRequest;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0443ec44;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443ec40;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443ec40:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0443ec44:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53836
// ==== 0x443ed08  ArcAddressEntrySaveRequest::save  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAddressEntrySaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04330cdc(param_1 + 8);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ed1c);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53855
// ==== 0x443ed1c  ArcAddressEntrySaveRequest::cancel  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcAddressEntrySaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_04330d38(param_1 + 8);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ed30);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53874
// ==== 0x443ed30  ArcCreditCardInfoSaveRequest::initWithCreditCardInfoSaveRequest:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCreditCardInfoSaveRequest::initWithCreditCardInfoSaveRequest_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcCreditCardInfoSaveRequest;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0443ed90;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443ed8c;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443ed8c:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0443ed90:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53934
// ==== 0x443ee54  ArcCreditCardInfoSaveRequest::save  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCreditCardInfoSaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_0433487c(*(undefined8 *)(param_1 + 8));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443ee68);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53953
// ==== 0x443ee68  ArcCreditCardInfoSaveRequest::cancel  (148 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCreditCardInfoSaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443eefc);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 8);
  if ((((*(long *)(lVar2 + 0x20) != 0) && (*(char *)(*(long *)(lVar2 + 0x20) + 4) == '\0')) &&
      (*(long *)(lVar2 + 0x28) != 0)) &&
     (((*(long *)(lVar2 + 0x20) != 0 && (*(char *)(*(long *)(lVar2 + 0x20) + 4) == '\0')) &&
      (lVar2 = *(long *)(lVar2 + 0x28), lVar2 != 0)))) {
    FUN_07c72f90(lVar2);
    lVar2 = FUN_000805e0(lVar2 + 8,&DAT_0c9c03dc);
    if (lVar2 != 0 && lVar2 + -0x48 != 0) {
      FUN_07c7dca0(lVar2 + -0x48,2);
      return;
    }
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:53984
// ==== 0x443eefc  ArcLoginCredentialsSaveRequest::initWithLoginCredentialsSaveRequest:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcLoginCredentialsSaveRequest::initWithLoginCredentialsSaveRequest_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcLoginCredentialsSaveRequest;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0443ef5c;
  plVar4 = *(long **)(IVar2 + 0x10);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443ef58;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443ef58:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0443ef5c:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54044
// ==== 0x443f020  ArcLoginCredentialsSaveRequest::isUpdate  (24 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcLoginCredentialsSaveRequest::isUpdate(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    return (bool)*(byte *)(*(long *)(param_1 + 8) + 0x2c);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443f038);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54062
// ==== 0x443f038  ArcLoginCredentialsSaveRequest::save  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::save(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f058);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) {
    FUN_07d64988();
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54084
// ==== 0x443f058  ArcLoginCredentialsSaveRequest::cancel  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f080);
    (*pcVar1)();
  }
  if ((*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) &&
     (*(char *)(*(long *)(param_1 + 8) + 0x2c) != '\0')) {
    FUN_07d641d0();
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54107
// ==== 0x443f080  ArcLoginCredentialsSaveRequest::neverAskAgain  (32 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcLoginCredentialsSaveRequest::neverAskAgain(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443f0a0);
    (*pcVar1)();
  }
  if (*(long *)(*(long *)(param_1 + 8) + 0x30) != 0) {
    FUN_07d64b14();
    return;
  }
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54624
// ==== 0x443f8d4  ArcPermissionRequest::initWithPermissionRequest:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcPermissionRequest::initWithPermissionRequest_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcPermissionRequest;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0443f934;
  plVar4 = *(long **)(IVar2 + 0x18);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0443f930;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_0443f930:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0443f934:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:54683
// ==== 0x443f9f8  ArcPermissionRequest::type  (24 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcPermissionRequest::type(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    return (long)*(int *)(*(long *)(param_1 + 0x10) + 0x18);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x443fa10);
  (*pcVar1)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:55768
// ==== 0x4440c4c  ArcWebContents::initWithWebContents:  (300 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcWebContents::initWithWebContents_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcWebContents;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x128) == '\0') goto LAB_04440cac;
  plVar4 = *(long **)(IVar2 + 0x120);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar1 = plVar4[2];
        plVar4[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04440ca8;
      }
      pcVar3 = *(code **)(*plVar4 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar3)(plVar4);
      }
    }
  }
LAB_04440ca8:
  *(undefined1 *)(IVar2 + 0x128) = 0;
LAB_04440cac:
  *(undefined8 *)(IVar2 + 0x118) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x120) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x128) = 1;
  FUN_0440e174(IVar2);
  return IVar2;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:55887
// ==== 0x4440dec  ArcWebContents::setNavigationDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setNavigationDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsNavigationDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsNavigationDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsNavigationDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsNavigationDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e10;
    if (pAVar2 + 0x18 == (ArcWebContentsNavigationDelegateBridge *)0x0) goto LAB_04441128;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da88fe0;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsNavigationDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_04440ecc;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_navigation_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_navigation_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_04440ecc:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_navigation_delegate_bridge
                        (param_1,"web_contents_navigation_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345ad8(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345ad8(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_04441128;
  }
  sVar6 = web_contents_navigation_delegate_bridge(param_1,"web_contents_navigation_delegate_bridge")
  ;
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04440f9c;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_04440f9c:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04375b40(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_04441128:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x444112c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:56029
// ==== 0x444112c  ArcWebContents::setUIDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setUIDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsUIDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsUIDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsUIDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsUIDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e48;
    if (pAVar2 + 0x18 == (ArcWebContentsUIDelegateBridge *)0x0) goto LAB_04441468;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da89030;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsUIDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_0444120c;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_ui_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_ui_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_0444120c:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_ui_delegate_bridge(param_1,"web_contents_ui_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345ba0(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345ba0(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_04441468;
  }
  sVar6 = web_contents_ui_delegate_bridge(param_1,"web_contents_ui_delegate_bridge");
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044412dc;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_044412dc:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04375cdc(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_04441468:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x444146c);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:56169
// ==== 0x444146c  ArcWebContents::setActionsDelegate:  (832 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setActionsDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsActionsDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsActionsDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsActionsDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsActionsDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88e80;
    if (pAVar2 + 0x18 == (ArcWebContentsActionsDelegateBridge *)0x0) goto LAB_044417a8;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da88f58;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsActionsDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_0444154c;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_actions_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_actions_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_0444154c:
    if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
      sVar6 = web_contents_actions_delegate_bridge(param_1,"web_contents_actions_delegate_bridge");
      p_Var3 = sVar6.field1_0x8;
      if (p_Var3 == (__shared_weak_count *)0x0) {
        FUN_04345d10(param_1 + 0x118,sVar6.field0_0x0,0);
        return;
      }
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + 1;
      FUN_04345d10(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 8);
      *(long *)(p_Var3 + 8) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
        (*pcVar4)(p_Var3);
        return;
      }
      FUN_00005810(p_Var3,0x20);
      return;
    }
    goto LAB_044417a8;
  }
  sVar6 = web_contents_actions_delegate_bridge(param_1,"web_contents_actions_delegate_bridge");
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0444161c;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_0444161c:
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_04376050(*(undefined8 *)(param_1 + 0x118),0,0);
    return;
  }
LAB_044417a8:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x44417ac);
  (*pcVar4)();
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:56309
// ==== 0x44417ac  ArcWebContents::setTopBandColorDelegate:  (928 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContents::setTopBandColorDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ArcWebContentsTopBandColorDelegateBridge *pAVar2;
  __shared_weak_count *p_Var3;
  __shared_weak_count *in_x4;
  code *pcVar4;
  ArcWebContentsTopBandColorDelegateBridge *pAVar5;
  shared_ptr<ArcWebContentsTopBandColorDelegateBridge> sVar6;
  undefined8 local_50;
  undefined1 auStack_48 [8];
  
  if (param_3 != 0) {
    pAVar2 = (ArcWebContentsTopBandColorDelegateBridge *)FUN_00008790(0x30);
    pAVar5 = pAVar2 + 8;
    *(long *)pAVar5 = 0;
    *(long *)(pAVar2 + 0x10) = 0;
    *(undefined ***)pAVar2 = &PTR_viewWillStartLiveResize_0da88eb8;
    if (pAVar2 + 0x18 == (ArcWebContentsTopBandColorDelegateBridge *)0x0) goto LAB_04441b48;
    _objc_initWeak(&local_50,param_1);
    _objc_initWeak(auStack_48,local_50);
    *(undefined ***)(pAVar2 + 0x18) = &PTR_FUN_0da89008;
    _objc_copyWeak(pAVar2 + 0x20,auStack_48);
    _objc_initWeak(pAVar2 + 0x28,param_3);
    _objc_destroyWeak(auStack_48);
    _objc_destroyWeak(&local_50);
    if (param_1 == 0) {
      if (pAVar2 != (ArcWebContentsTopBandColorDelegateBridge *)0x0) {
        LOAcquire();
        lVar1 = *(long *)pAVar5;
        *(long *)pAVar5 = lVar1 + -1;
        LORelease();
        if (lVar1 == 0) {
          (**(code **)(*(long *)pAVar2 + 0x10))(pAVar2);
          if (*(long *)(pAVar2 + 0x10) != 0) {
            LOAcquire();
            lVar1 = *(long *)(pAVar2 + 0x10);
            *(long *)(pAVar2 + 0x10) = lVar1 + -1;
            LORelease();
            if (lVar1 != 0) goto LAB_0444188c;
          }
          pcVar4 = *(code **)(*(long *)pAVar2 + 0x20);
          if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
            FUN_00005810(pAVar2,0x20);
          }
          else {
            (*pcVar4)(pAVar2);
          }
        }
      }
    }
    else {
      sVar6.field1_0x8 = in_x4;
      sVar6.field0_0x0 = pAVar2;
      setWeb_contents_top_band_color_delegate_bridge_
                ((ArcWebContents *)param_1,(ID)"setWeb_contents_top_band_color_delegate_bridge:",
                 (SEL)(pAVar2 + 0x18),sVar6);
    }
LAB_0444188c:
    if ((*(byte *)(param_1 + 0x128) & 1) == 0) goto LAB_04441b48;
    sVar6 = web_contents_top_band_color_delegate_bridge
                      (param_1,"web_contents_top_band_color_delegate_bridge");
    p_Var3 = sVar6.field1_0x8;
    if (p_Var3 == (__shared_weak_count *)0x0) {
      FUN_04345e80(param_1 + 0x118,sVar6.field0_0x0,0);
      return;
    }
    lVar1 = *(long *)(p_Var3 + 0x10);
    *(long *)(p_Var3 + 0x10) = lVar1 + 1;
    FUN_04345e80(lVar1,param_1 + 0x118,sVar6.field0_0x0,p_Var3);
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
    (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
    if (*(long *)(p_Var3 + 0x10) != 0) {
      LOAcquire();
      lVar1 = *(long *)(p_Var3 + 0x10);
      *(long *)(p_Var3 + 0x10) = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
    }
    pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
    if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40))
    goto LAB_04441a54;
    goto LAB_044419d4;
  }
  sVar6 = web_contents_top_band_color_delegate_bridge
                    (param_1,"web_contents_top_band_color_delegate_bridge");
  p_Var3 = sVar6.field1_0x8;
  if (p_Var3 != (__shared_weak_count *)0x0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 8);
    *(long *)(p_Var3 + 8) = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*(long *)p_Var3 + 0x10))(p_Var3);
      if (*(long *)(p_Var3 + 0x10) != 0) {
        LOAcquire();
        lVar1 = *(long *)(p_Var3 + 0x10);
        *(long *)(p_Var3 + 0x10) = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04441994;
      }
      pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(p_Var3,0x20);
      }
      else {
        (*pcVar4)(p_Var3);
      }
    }
  }
LAB_04441994:
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
LAB_04441b48:
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x4441b4c);
    (*pcVar4)();
  }
  lVar1 = *(long *)(param_1 + 0x118);
  p_Var3 = *(__shared_weak_count **)(lVar1 + 0x280);
  *(undefined8 *)(lVar1 + 0x280) = 0;
  *(undefined8 *)(lVar1 + 0x278) = 0;
  if (p_Var3 == (__shared_weak_count *)0x0) {
    return;
  }
  if (*(long *)(p_Var3 + 0x10) != 0) {
    LOAcquire();
    lVar1 = *(long *)(p_Var3 + 0x10);
    *(long *)(p_Var3 + 0x10) = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
  }
  pcVar4 = *(code **)(*(long *)p_Var3 + 0x20);
  if (((pcVar4 != FUN_00f01bc0) && (pcVar4 != FUN_00f01bc0)) && (pcVar4 != FUN_013efb40)) {
LAB_04441a54:
    (*pcVar4)(p_Var3);
    return;
  }
LAB_044419d4:
  FUN_00005810(p_Var3,0x20);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:57579
// ==== 0x4442eb8  ArcWebContents::executeJavascript:completion:  (328 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

void ArcWebContents::executeJavascript_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined1 local_88;
  undefined7 uStack_87;
  undefined7 local_78;
  char cStack_71;
  undefined8 local_70;
  undefined8 uStack_68;
  ulong local_60;
  ID *local_58 [2];
  code *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4442ffc);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_70 = 0;
    uStack_68 = 0;
    local_60 = 0;
  }
  else {
    FUN_01b356b8(&local_70,param_3);
    param_2 = extraout_x1;
  }
  local_48 = FUN_04445338;
  local_40 = &PTR_FUN_0df6c478;
  local_58[0] = (ID *)FUN_00008790(8,param_2);
  *local_58[0] = param_4;
  cStack_71 = '\0';
  local_88 = 0;
  FUN_04377d70(*(undefined8 *)(param_1 + 0x118),&local_70,local_58,&local_88);
  if (cStack_71 < '\0') {
    FUN_00005810(CONCAT71(uStack_87,local_88),CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff);
  }
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:57643
// ==== 0x444302c  ArcWebContents::executeJavascript:reason:completion:  (396 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcWebContents::executeJavascript_reason_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 local_98;
  undefined8 uStack_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  code *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44431b4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_58 = FUN_04445338;
  local_50 = &PTR_FUN_0df6c498;
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  if (param_4 == 0) {
    local_98 = 0;
    uStack_90 = 0;
    local_88 = 0;
  }
  else {
    FUN_01b356b8(&local_98,param_4);
  }
  FUN_04377d70(*(undefined8 *)(param_1 + 0x118),&local_80,local_68,&local_98);
  if ((long)local_88 < 0) {
    FUN_00005810(local_98,local_88 & 0x7fffffffffffffff);
  }
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x44.c:59585
// ==== 0x4444b54  ArcWebContents::.cxx_destruct  (1416 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcWebContents::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x128) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x120), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_04444b88;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_04444b88:
  plVar3 = *(long **)(param_1 + 0x110);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444be0;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444be0:
  plVar3 = *(long **)(param_1 + 0x100);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444c38;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444c38:
  plVar3 = *(long **)(param_1 + 0xe0);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444c90;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444c90:
  plVar3 = *(long **)(param_1 + 0xd0);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444ca8;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444ca8:
  plVar3 = *(long **)(param_1 + 0xc0);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444cc0;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444cc0:
  plVar3 = *(long **)(param_1 + 0xb0);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444cd8;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444cd8:
  plVar3 = *(long **)(param_1 + 0xa0);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04444cf0;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_04444cf0:
  *(undefined8 *)(param_1 + 0x90) = 0;
  *(undefined8 *)(param_1 + 0x88) = 0;
  *(undefined8 *)(param_1 + 0x70) = 0;
  *(undefined8 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x60) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  _objc_destroyWeak(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:83873
// ==== 0x8371be0  ArcBrowserApplication::showTaskManager  (12 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserApplication::showTaskManager(ID param_1,SEL param_2)

{
  FUN_07d5c700(0,0);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:83885
// ==== 0x8371bec  ArcBrowserApplication::devToolsWebSocketDebugURL  (152 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserApplication::devToolsWebSocketDebugURL(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  ID IVar3;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  lStack_38 = 0;
  local_40 = (undefined1 *)0x0;
  local_28 = 0;
  local_30 = 0;
  FUN_05533b24(&local_40,**(long **)(param_1 + 0x20) + 0x128);
  if ((char)local_28 == '\0') {
    IVar3 = 0;
  }
  else {
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01c391bc(ppuVar1,lVar2);
    if (((char)local_28 != '\0') && ((long)local_30 < 0)) {
      thunk_FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return IVar3;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:84042
// ==== 0x8371eb8  ArcImageRep::encodeToJPEGWithQuality:  (192 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x14 bytes */

ID ArcImageRep::encodeToJPEGWithQuality_(ID param_1,SEL param_2,float param_3)

{
  uint uVar1;
  long lVar2;
  ulong uVar3;
  code *pcVar4;
  undefined8 uVar5;
  ID IVar6;
  long local_50;
  ulong uStack_48;
  undefined8 local_40;
  undefined8 local_38;
  
  uStack_48 = 0;
  local_50 = 0;
  local_38 = 0;
  local_40 = 0;
  uVar1 = (int)(param_3 * 100.0) & ((int)(param_3 * 100.0) >> 0x1f ^ 0xffffffffU);
  if (99 < (int)uVar1) {
    uVar1 = 100;
  }
  uVar5 = FUN_022f4af4(*(undefined8 *)(param_1 + 0x10));
  FUN_08b0218c(&local_50,uVar5,uVar1);
  uVar3 = uStack_48;
  lVar2 = local_50;
  if ((char)local_38 == '\0') {
    IVar6 = 0;
  }
  else {
    IVar6 = NSData::dataWithBytes_length_
                      ((ID)&_OBJC_CLASS___NSData,"dataWithBytes:length:",local_50,uStack_48);
    if (lVar2 != 0) {
      if ((ulong)-lVar2 < uVar3) {
                    /* WARNING: Does not return */
        pcVar4 = (code *)SoftwareBreakpoint(1,0x8371f68);
        (*pcVar4)();
      }
      thunk_FUN_00005810(lVar2,local_40);
    }
  }
  return IVar6;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:84277
// ==== 0x83723c0  ArcURLLoadConfiguration::setHeaders:  (532 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x18 bytes */

void ArcURLLoadConfiguration::setHeaders_(ID param_1,SEL param_2,ID param_3)

{
  ID IVar1;
  ID IVar2;
  long lVar3;
  long lVar4;
  ID IVar5;
  undefined8 local_1a0;
  undefined8 uStack_198;
  ulong local_190;
  undefined8 local_188;
  undefined8 uStack_180;
  ulong local_178;
  undefined8 local_170;
  undefined8 uStack_168;
  ulong local_160;
  undefined8 local_158;
  undefined8 uStack_150;
  ulong local_148;
  undefined8 local_140;
  long lStack_138;
  long *local_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  long local_f8;
  undefined1 auStack_f0 [128];
  long local_70;
  
  local_70 = *(long *)PTR____stack_chk_guard_0d824020;
  FUN_04b8b29c(param_1 + 0x68);
  uStack_118 = 0;
  local_120 = 0;
  uStack_108 = 0;
  uStack_110 = 0;
  lStack_138 = 0;
  local_140 = 0;
  uStack_128 = 0;
  local_130 = (long *)0x0;
  IVar1 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_140,auStack_f0,
                        0x10);
  if (IVar1 != 0) {
    lVar4 = *local_130;
    do {
      IVar5 = 0;
      do {
        if (*local_130 != lVar4) {
          _objc_enumerationMutation(param_3);
        }
        lVar3 = *(long *)(lStack_138 + IVar5 * 8);
        if (lVar3 == 0) {
          local_188 = 0;
          uStack_180 = 0;
          local_178 = 0;
        }
        else {
          FUN_01b356b8(&local_188,lVar3);
        }
        IVar2 = _objc_msgSend(param_3,"objectForKey:",lVar3);
        if (IVar2 == 0) {
          local_1a0 = 0;
          uStack_198 = 0;
          local_190 = 0;
        }
        else {
          FUN_01b356b8(&local_1a0,IVar2);
        }
        uStack_168 = uStack_180;
        local_170 = local_188;
        local_160 = local_178;
        uStack_150 = uStack_198;
        local_158 = local_1a0;
        local_148 = local_190;
        local_f8 = param_1 + 0x68;
        FUN_063823b8(&local_f8,&local_170,&local_170);
        if ((long)local_148 < 0) {
          thunk_FUN_00005810(local_158,local_148 & 0x7fffffffffffffff);
        }
        if ((long)local_160 < 0) {
          thunk_FUN_00005810(local_170,local_160 & 0x7fffffffffffffff);
        }
        IVar5 = IVar5 + 1;
      } while (IVar1 != IVar5);
      IVar1 = _objc_msgSend(param_3,"countByEnumeratingWithState:objects:count:",&local_140,
                            auStack_f0,0x10);
    } while (IVar1 != 0);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:84383
// ==== 0x8372628  ArcURLLoadConfiguration::.cxx_destruct  (8 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcURLLoadConfiguration::_cxx_destruct(ID param_1,SEL param_2)

{
  FUN_0836de40(param_1 + 8);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:86368
// ==== 0x8374b4c  ArcPerformanceController::setHighEfficiencyModeEnabled:  (116 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::setHighEfficiencyModeEnabled_(ID param_1,SEL param_2,bool param_3)

{
  int iVar1;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    thunk_FUN_00008790(0x40,param_3);
    DAT_0e4a2f18 = FUN_0433d1bc();
    FUN_0c44b760(&DAT_0e4a2f20);
  }
  FUN_07b02940(DAT_0e49d488,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0x83.c:86387
// ==== 0x8374bc0  ArcPerformanceController::temporarilyDisableBatterySaverMode:  (116 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcPerformanceController::temporarilyDisableBatterySaverMode_
               (ID param_1,SEL param_2,bool param_3)

{
  int iVar1;
  
  if (((DAT_0e4a2f20 & 1) == 0) && (iVar1 = FUN_0c44b610(&DAT_0e4a2f20), iVar1 != 0)) {
    thunk_FUN_00008790(0x40,param_3);
    DAT_0e4a2f18 = FUN_0433d1bc();
    FUN_0c44b760(&DAT_0e4a2f20);
  }
  FUN_07afc3a8(DAT_0e49d480,param_3);
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0xc.c:96692
// ==== 0xc7fdb4  ArcHSLAMatrix::m2  (8 bytes)

/* Function Stack Size: 0x10 bytes */

float ArcHSLAMatrix::m2(ID param_1,SEL param_2)

{
  return *(float *)(param_1 + 0x10);
}



// SOURCE /home/exedev/work/inputs/decompile/0xe.c:132779
// ==== 0xed0b14  ArcTracingConfiguration::privacyFilteringEnabled  (8 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcTracingConfiguration::privacyFilteringEnabled(ID param_1,SEL param_2)

{
  return (bool)*(byte *)(param_1 + 0x38);
}



// SOURCE /home/exedev/work/inputs/decompile/0xe.c:164162
// ==== 0xef938c  ArcURLLoadCancellationToken::token  (40 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcURLLoadCancellationToken::token
               (basic_string<char,_std::char_traits<char>,_std::allocator<char>>
                *__return_storage_ptr__,ID param_1,SEL param_2)

{
  char *pcVar1;
  
  if (-1 < *(char *)(param_1 + 0x1f)) {
    pcVar1 = *(char **)(param_1 + 8);
    (__return_storage_ptr__->field0_0x0).field1.field1_0x8 = *(unsigned_long_long *)(param_1 + 0x10)
    ;
    (__return_storage_ptr__->field0_0x0).field1.field0_0x0 = pcVar1;
    (__return_storage_ptr__->field0_0x0).field1.field2_0x10 =
         *(AnonymousBitField1 *)(param_1 + 0x18);
    return;
  }
  FUN_00060a60(__return_storage_ptr__,*(undefined8 *)(param_1 + 8),*(undefined8 *)(param_1 + 0x10));
  return;
}



// SOURCE /home/exedev/work/inputs/decompile/0xf.c:110368
// ==== 0xf90b90  ArcCpuInfo::armPartNumber  (8 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcCpuInfo::armPartNumber(ID param_1,SEL param_2)

{
  return *(unsigned_long_long *)(param_1 + 0x68);
}



