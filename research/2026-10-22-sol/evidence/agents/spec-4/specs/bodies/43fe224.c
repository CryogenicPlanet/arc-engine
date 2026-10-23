// ==== 0x43fe224  ArcWebContentsDevTools::bridge  (32 bytes)

/* Function Stack Size: 0x10 bytes */

shared_ptr<(anonymous_namespace)::ArcWebContentsDevToolsDelegateBridge>
ArcWebContentsDevTools::bridge(ID param_1,SEL param_2)

{
  long *plVar1;
  shared_ptr<(anonymous_namespace)::ArcWebContentsDevToolsDelegateBridge> sVar2;
  
  sVar2 = *(shared_ptr<(anonymous_namespace)::ArcWebContentsDevToolsDelegateBridge> *)
           (param_1 + 0x10);
  if (*(long *)(param_1 + 0x18) != 0) {
    plVar1 = (long *)(*(long *)(param_1 + 0x18) + 8);
    *plVar1 = *plVar1 + 1;
  }
  return sVar2;
}


