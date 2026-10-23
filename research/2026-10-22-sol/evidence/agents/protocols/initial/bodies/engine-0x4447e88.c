// ==== 0x4447e88  FUN_04447e88  (116 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

undefined8 FUN_04447e88(long param_1,undefined8 param_2,undefined8 *param_3)

{
  _objc_msgSend(*(ID *)(param_1 + 0x10),"webContents:updateDevToolsToolbox:",(double)(float)*param_3
                ,(double)(float)((ulong)*param_3 >> 0x20),(double)(float)param_3[1],
                (double)(float)((ulong)param_3[1] >> 0x20),*(undefined8 *)(param_1 + 8));
  return 0;
}


