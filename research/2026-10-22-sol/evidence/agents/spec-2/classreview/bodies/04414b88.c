// ==== 0x4414b88  ArcWebContentsConfiguration::setDisallowDownloads:  (8 bytes)

/* Function Stack Size: 0x14 bytes */

void ArcWebContentsConfiguration::setDisallowDownloads_(ID param_1,SEL param_2,bool param_3)

{
  *(char *)(param_1 + 0xd) = (char)param_3;
  return;
}


