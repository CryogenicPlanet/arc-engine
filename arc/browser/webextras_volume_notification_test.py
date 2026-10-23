#!/usr/bin/env python3
"""Apply the held proposal privately and execute extracted production bodies.

This does not run Chromium tests or a browser. Transport/observer containers in
this small fixture are mocks; real generated Mojo bindings are checked separately.
"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PATCH = ROOT / 'arc/browser/webextras_volume_notification.patch'


def body(source, signature):
    start = source.index(signature)
    opening = source.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


def apply_proposal(directory):
    patch = PATCH.read_text()
    files = re.findall(r'^\+\+\+ b/(.+)$', patch, re.M)
    if not files or len(files) != len(set(files)):
        raise AssertionError('missing/duplicate proposal paths')
    for rel in files:
        target = directory / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(ROOT / 'chromium/src' / rel, target)
    for args in (['--check', '--whitespace=error'], ['--whitespace=error']):
        subprocess.run(['git', 'apply', *args, str(PATCH)], cwd=directory,
                       check=True, capture_output=True, text=True)
    return directory


def fixture(stage):
    def read(rel):
        return (stage / rel).read_text()
    player = read('third_party/blink/renderer/platform/media/web_media_player_impl.cc')
    element = read('third_party/blink/renderer/core/html/media/html_media_element.cc')
    host = read('content/browser/media/media_web_contents_observer.cc')
    wc = read('content/browser/web_contents/web_contents_impl.cc')
    functions = '\n'.join([
        body(player, 'void WebMediaPlayerImpl::SetVolume(double volume)'),
        body(player, 'void WebMediaPlayerImpl::SetVolumeMultiplier(double multiplier)'),
        body(element, 'void HTMLMediaElement::DidPlayerVolumeChange(double volume)'),
        body(element, 'double HTMLMediaElement::EffectiveMediaVolume() const'),
        body(host, 'void MediaWebContentsObserver::MediaPlayerObserverHostImpl::OnVolumeChanged('),
        body(wc, 'void WebContentsImpl::MediaVolumeChanged(double volume)'),
    ])
    return r'''
#include <cassert>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
struct Log { template<class T> Log& operator<<(const T&) { return *this; } };
#define DVLOG(x) if (false) Log()
#define DCHECK(x) assert(x)
struct Event { char kind; double value; bool operator==(const Event&) const = default; };
struct WebContentsObserver {
  std::vector<double> volumes;
  void MediaVolumeChanged(double v) { volumes.push_back(v); }
};
struct ObserverList {
  std::vector<WebContentsObserver*> entries;
  template<class M> void NotifyObservers(M method, double v) {
    for (auto* observer : entries) (observer->*method)(v);
  }
};
struct WebContentsImpl { ObserverList observers_; void MediaVolumeChanged(double); };
struct MediaWebContentsObserver {
  WebContentsImpl* contents;
  WebContentsImpl* web_contents_impl() { return contents; }
  struct MediaPlayerObserverHostImpl {
    MediaWebContentsObserver* media_web_contents_observer_;
    void OnVolumeChanged(double);
  };
};
struct RemoteSet {
  std::vector<MediaWebContentsObserver::MediaPlayerObserverHostImpl*> entries;
  auto& Value() { return entries; }
};
struct HTMLMediaElement {
  RemoteSet* media_player_observer_remote_set_;
  bool muted_ = false;
  double volume_ = 1;
  bool muted() const { return muted_; }
  double EffectiveMediaVolume() const;
  void DidPlayerVolumeChange(double);
};
struct Client {
  std::vector<Event> events;
  HTMLMediaElement* element;
  void DidPlayerMutedStatusChange(bool m) { events.push_back({'m', double(m)}); }
  void DidPlayerVolumeChange(double v) {
    events.push_back({'v', v}); element->DidPlayerVolumeChange(v);
  }
  void ResumePlayback() { assert(false); }
};
struct Pipeline { double value = 1; void SetVolume(double v) { value = v; } };
struct Watch { void OnVolumeChange(double) {} };
struct Runner { bool BelongsToCurrentThread() { return true; } };
struct WebMediaPlayerImpl {
  double volume_ = 1, volume_multiplier_ = 1;
  Pipeline* pipeline_controller_; Client* client_; Runner* main_task_runner_;
  Watch* watch_time_reporter_ = nullptr;
  bool delegate_has_audio_ = false;
  std::unique_ptr<int> visibility_pause_reason_;
  bool HasUnmutedAudio() { return false; }
  bool IsPausedBecausePageHidden() { return false; }
  void DidMediaMetadataChange() { assert(false); }
  void UpdatePlayState() {}
  void SetVolume(double); void SetVolumeMultiplier(double);
};
''' + functions + r'''
int main() {
  WebContentsImpl wc; WebContentsObserver observer; wc.observers_.entries = {&observer};
  MediaWebContentsObserver media{&wc};
  MediaWebContentsObserver::MediaPlayerObserverHostImpl first{&media}, second{&media};
  RemoteSet remotes{{&first}}; HTMLMediaElement element{&remotes};
  Client client{{}, &element}; Pipeline pipeline; Runner runner;
  WebMediaPlayerImpl player; player.pipeline_controller_ = &pipeline;
  player.client_ = &client; player.main_task_runner_ = &runner;
  player.SetVolume(.625); player.SetVolume(.625); player.SetVolume(.375);
  player.SetVolumeMultiplier(.25);
  assert(pipeline.value == .09375);
  player.SetVolume(0); player.SetVolumeMultiplier(.5);
  assert(pipeline.value == 0);
  assert((client.events == std::vector<Event>{{'m',0},{'v',.625},{'m',0},
     {'m',0},{'v',.375},{'m',0},{'m',1},{'v',0},{'m',1}}));
  assert((observer.volumes == std::vector<double>{.625,.375,0}));
  constexpr double precise = .123456789123;
  player.SetVolume(precise);
  assert(observer.volumes.back() == precise);
  assert(pipeline.value == precise * .5);
  // Distinct players and repeated browser messages are never globally deduped.
  first.OnVolumeChanged(.375); second.OnVolumeChanged(.375);
  assert(observer.volumes[4] == .375 && observer.volumes[5] == .375);
  // Every renderer remote receives each emission, including exact nonzero doubles.
  remotes.entries.push_back(&second);
  element.DidPlayerVolumeChange(precise);
  assert(observer.volumes[6] == precise && observer.volumes[7] == precise);
  // Actual HTML effective-volume body preserves raw input while muted is zero.
  element.volume_ = .625; assert(element.EffectiveMediaVolume() == .625);
  element.muted_ = true; assert(element.EffectiveMediaVolume() == 0);
  element.volume_ = .375; assert(element.EffectiveMediaVolume() == 0);
  element.muted_ = false; assert(element.EffectiveMediaVolume() == .375);
}
'''


class VolumeProposalTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-volume-regression-')
        cls.directory = Path(cls.temp.name)
        cls.stage = apply_proposal(cls.directory / 'src')
        cls.source = fixture(cls.stage)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def compile_run(self, source, name):
        cc = self.directory / (name + '.cc')
        executable = self.directory / name
        cc.write_text(source)
        compiled = subprocess.run(['/usr/bin/clang++', '-std=c++20', '-Wall', '-Wextra',
                                   '-Werror', str(cc), '-o', str(executable)],
                                  capture_output=True, text=True)
        self.assertEqual(compiled.returncode, 0, compiled.stderr)
        return subprocess.run([str(executable)], capture_output=True).returncode

    def test_actual_extracted_changed_input_chain(self):
        self.assertEqual(self.compile_run(self.source, 'actual'), 0)

    def test_equal_input_notification_negative_control(self):
        self.assertIn('if (previous_volume != volume)', self.source)
        altered = self.source.replace('if (previous_volume != volume)',
                                      'if (true)'
                                      '\n    (void)previous_volume;')
        self.assertNotEqual(self.compile_run(altered, 'equal-input'), 0)

    def test_boolean_value_negative_control(self):
        altered = self.source.replace('client_->DidPlayerVolumeChange(volume);',
                                      'client_->DidPlayerVolumeChange(volume == 0);')
        self.assertNotEqual(self.compile_run(altered, 'boolean-value'), 0)

    def test_muted_order_negative_control(self):
        old = ('  client_->DidPlayerMutedStatusChange(volume == 0.0);\n'
               '  if (previous_volume != volume) {\n'
               '    client_->DidPlayerVolumeChange(volume);\n  }')
        new = ('  if (previous_volume != volume) {\n'
               '    client_->DidPlayerVolumeChange(volume);\n  }\n'
               '  client_->DidPlayerMutedStatusChange(volume == 0.0);')
        self.assertIn(old, self.source)
        self.assertNotEqual(self.compile_run(self.source.replace(old, new),
                                             'muted-order'), 0)

    def test_mojom_appends_preserving_existing_ordinals(self):
        rel = Path('media/mojo/mojom/media_player.mojom')
        before = (ROOT / 'chromium/src' / rel).read_text()
        after = (self.stage / rel).read_text()
        insertion = ('\n  // Reports a changed renderer player volume input. HTML element mute is\n'
                     '  // already applied; the player volume multiplier is not.\n'
                     '  OnVolumeChanged(double volume);\n')
        self.assertEqual(after.replace(insertion, ''), before)
        self.assertIn('OnVideoFrameAvailabilityChanged(bool available);\n' + insertion,
                      after)


if __name__ == '__main__':
    unittest.main(verbosity=2)
