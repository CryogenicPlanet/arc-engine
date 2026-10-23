// Copyright 2026 ArcMod.
#ifndef ARC_BROWSER_WEBEXTRAS_MEDIA_H_
#define ARC_BROWSER_WEBEXTRAS_MEDIA_H_
#include <memory>
namespace content {
class VideoOverlayWindow;
class VideoPictureInPictureWindowController;
}
namespace arc {
std::unique_ptr<content::VideoOverlayWindow> CreateVideoOverlayWindow(
    content::VideoPictureInPictureWindowController* controller);
}
#endif
