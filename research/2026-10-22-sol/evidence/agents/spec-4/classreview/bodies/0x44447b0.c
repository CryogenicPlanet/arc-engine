// ==== 0x44447b0  ArcWebContents::setPendingRenderFrameViewportSize:  (8 bytes)

/* Function Stack Size: 0x20 bytes */

void ArcWebContents::setPendingRenderFrameViewportSize_(ID param_1,SEL param_2,CGSize param_3)

{
  *(double *)(param_1 + 0xe8) = param_3.field0_0x0;
  *(double *)(param_1 + 0xf0) = param_3.field1_0x8;
  return;
}


