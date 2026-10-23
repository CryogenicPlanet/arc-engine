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


