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


