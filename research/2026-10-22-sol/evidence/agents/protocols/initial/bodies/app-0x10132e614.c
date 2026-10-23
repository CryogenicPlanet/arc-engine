// ==== 0x10132e614  _TtCC15URLLoaderClient19URLLoaderClientImpl14StreamDelegate::-[URLLoaderClientImpl.StreamDelegate_didFinish:]  (104 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void _TtCC15URLLoaderClient19URLLoaderClientImpl14StreamDelegate::
     __URLLoaderClientImpl_StreamDelegate_didFinish__(ID param_1,SEL param_2,ID param_3)

{
  undefined8 local_28;
  
  FUN_10132e2bc(param_1);
  local_28 = 0;
  FUN_100005134(&DAT_1045499f8,&DAT_1036fc430);
  Swift::AsyncThrowingStream::Continuation::_finish((Continuation_conflict)&local_28);
  return;
}


