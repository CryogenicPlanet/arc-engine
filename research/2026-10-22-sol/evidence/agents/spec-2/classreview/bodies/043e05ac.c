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


