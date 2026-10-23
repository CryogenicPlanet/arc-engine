// ==== 0x10055e6f0  _TtC3ADK28TablessWebContentsUIDelegate::-[TablessWebContentsUIDelegate_webContents:promptForArtifactLinkPermissionToURL:willNavigate:forSessionID:folderName:completion:]  (152 bytes)

/* Function Stack Size: 0x3c bytes */

void _TtC3ADK28TablessWebContentsUIDelegate::
     __TablessWebContentsUIDelegate_webContents_promptForArtifactLinkPermissionToURL_willNavigate_forSessionID_folderName_completion__
               (ID param_1,SEL param_2,ID param_3,ID param_4,bool param_5,ID param_6,ID param_7,
               ID param_8,undefined4 param_9)

{
  URL UVar1;
  long extraout_x8;
  long lVar2;
  
  UVar1 = Foundation::URL::typeMetadataAccessor();
  lVar2 = *(long *)(UVar1.unknown + -8);
  (*(code *)PTR____chkstk_darwin_10415f308)(*(undefined8 *)(lVar2 + 0x40));
  Foundation::URL::__unconditionallyBridgeFromObjectiveC((URL)param_4);
  (**(code **)(param_8 + 0x10))(param_8,0);
  (**(code **)(lVar2 + 8))
            (&stack0xffffffffffffffc0 + -(extraout_x8 + 0xfU & 0xfffffffffffffff0),UVar1.unknown);
  return;
}


