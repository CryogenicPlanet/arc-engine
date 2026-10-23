// Copyright 2026 ArcMod. Native input for Arc's WebContents AI agent.

#import "arc/api/arc_internal.h"

#include <cstdint>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"

namespace {

// The original agent uses WebContentsImpl's primary main RenderFrameHost,
// then that frame's widget view (0x4354de4 / 0x4354ea0 / 0x10f9b60).
NSView* NativeInputView(base::WeakPtr<content::WebContents> contents) {
  if (!contents) {
    return nil;
  }
  content::RenderFrameHost* frame = contents->GetPrimaryMainFrame();
  content::RenderWidgetHostView* view = frame ? frame->GetView() : nullptr;
  return view ? view->GetNativeView().GetNativeNSView() : nil;
}

void SendKeyDown(NSView* view, NSString* characters, unsigned short key_code) {
  NSEvent* event =
      [NSEvent keyEventWithType:NSEventTypeKeyDown
                      location:NSZeroPoint
                 modifierFlags:0
                     timestamp:NSProcessInfo.processInfo.systemUptime
                  windowNumber:view.window.windowNumber
                       context:nil
                    characters:characters
   charactersIgnoringModifiers:characters
                     isARepeat:NO
                       keyCode:key_code];
  [view keyDown:event];
}

void SendTextInput(base::WeakPtr<content::WebContents> contents,
                   std::string text) {
  NSView* view = NativeInputView(contents);
  if (!view) {
    return;
  }
  NSString* string = ArcNSString(text);
  // 0x473dff4 deliberately iterates NSString UTF-16 units, including separate
  // surrogate units, and emits only keyDown without changing focus.
  for (NSUInteger index = 0; index < string.length; ++index) {
    SendKeyDown(view, [string substringWithRange:NSMakeRange(index, 1)], 0);
  }
}

void SendKeyPress(base::WeakPtr<content::WebContents> contents,
                  uint32_t key) {
  NSView* view = NativeInputView(contents);
  if (!view) {
    return;
  }
  NSString* characters = @"";
  unsigned short key_code = 0;
  // 0x473e230 narrows the public NSInteger to 32 bits before selecting the
  // character table at 0xe21b208 and the packed key codes at 0x473e25c.
  switch (key) {
    case 2:
      characters = @"\x7f";
      key_code = 51;
      break;
    case 3:
      characters = @"\r";
      key_code = 36;
      break;
    case 4:
      characters = @"\x1b";
      key_code = 53;
      break;
    case 5:
      characters = @"\t";
      key_code = 48;
      break;
  }
  SendKeyDown(view, characters, key_code);
}

void RunOnUI(base::OnceClosure task) {
  if (content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    std::move(task).Run();
  } else {
    content::GetUIThreadTaskRunner({})->PostTask(FROM_HERE, std::move(task));
  }
}

}  // namespace

ARC_EXPORT @interface ArcWebContentsAIAgent : NSObject {
  base::WeakPtr<content::WebContents> _contents;
}
- (instancetype)initWithWebContentsAIAgent:(const void*)agent;
- (void)sendTextInput:(NSString*)text;
- (void)sendKeyPress:(NSInteger)key;
@end

@interface ArcWebContentsAIAgent ()
// Runtime entry point used by ArcWebContents' per-native-object service cache.
- (instancetype)initWithArcWebContents:(ArcWebContents*)contents;
@end

@implementation ArcWebContentsAIAgent

- (instancetype)initWithWebContentsAIAgent:(const void*)agent {
  if ((self = [super init])) {
    // Engine-internal pointer adaptation: our native handle is WebContents,
    // rather than reproducing Arc's private shared_ptr<AIAgent::Impl> ABI.
    content::WebContents* contents =
        const_cast<content::WebContents*>(
            static_cast<const content::WebContents*>(agent));
    if (contents) {
      _contents = contents->GetWeakPtr();
    }
  }
  return self;
}

- (instancetype)initWithArcWebContents:(ArcWebContents*)contents {
  return [self initWithWebContentsAIAgent:[contents webContents]];
}

- (void)sendTextInput:(NSString*)text {
  RunOnUI(base::BindOnce(&SendTextInput, _contents, ArcUTF8(text)));
}

- (void)sendKeyPress:(NSInteger)key {
  RunOnUI(base::BindOnce(&SendKeyPress, _contents, static_cast<uint32_t>(key)));
}

@end
