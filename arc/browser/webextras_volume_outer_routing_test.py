#!/usr/bin/env python3
"""Held root-routing regression; actual extracted methods, mock tree/observers."""
from pathlib import Path
import subprocess
import tempfile
import unittest
from webextras_volume_notification_test import ROOT, apply_proposal, body

PATCH = ROOT / 'arc/browser/webextras_volume_outer_routing.patch'


class OuterVolumeTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-volume-root-')
        cls.directory = Path(cls.temp.name)
        cls.stage = apply_proposal(cls.directory / 'src')
        for args in (['--check', '--whitespace=error'], ['--whitespace=error']):
            subprocess.run(['git', 'apply', *args, str(PATCH)], cwd=cls.stage,
                           check=True, capture_output=True, text=True)
        source = (cls.stage / 'content/browser/web_contents/web_contents_impl.cc').read_text()
        cls.source = r'''
#include <cassert>
#include <vector>
struct WebContentsObserver {
  std::vector<double> values;
  void MediaVolumeChanged(double value) { values.push_back(value); }
};
struct List {
  WebContentsObserver* observer;
  template<class Method> void NotifyObservers(Method method, double value) {
    (observer->*method)(value);
  }
};
struct WebContentsImpl {
  WebContentsImpl* outer;
  List observers_;
  WebContentsImpl* GetOuterWebContents() { return outer; }
  WebContentsImpl* GetOutermostWebContents();
  void MediaVolumeChanged(double);
};
''' + body(source, 'WebContentsImpl* WebContentsImpl::GetOutermostWebContents()') + '\n' + body(source, 'void WebContentsImpl::MediaVolumeChanged(double volume)') + r'''
int main() {
  WebContentsObserver root_events, middle_events, inner_events;
  WebContentsImpl root{nullptr, {&root_events}};
  WebContentsImpl middle{nullptr, {&middle_events}};
  WebContentsImpl inner{&middle, {&inner_events}};
  // Two levels: middle is root, and only its observer sees the exact input.
  inner.MediaVolumeChanged(.625);
  assert((middle_events.values == std::vector<double>{.625}));
  assert(root_events.values.empty() && inner_events.values.empty());
  middle_events.values.clear();
  // Three levels: equal values from different senders remain two events in order.
  middle.outer = &root;
  inner.MediaVolumeChanged(.375);
  middle.MediaVolumeChanged(.375);
  inner.MediaVolumeChanged(.123456789123);
  root.MediaVolumeChanged(0);
  assert((root_events.values == std::vector<double>{.375,.375,.123456789123,0}));
  assert(middle_events.values.empty() && inner_events.values.empty());
  // Routing follows the genuine current chain at receipt, without a root cache.
  middle.outer = nullptr;
  inner.MediaVolumeChanged(.875);
  assert((middle_events.values == std::vector<double>{.875}));
  assert(root_events.values.size() == 4 && inner_events.values.empty());
}
'''

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def run_source(self, source, name):
        cc = self.directory / (name + '.cc')
        exe = self.directory / name
        cc.write_text(source)
        result = subprocess.run(['/usr/bin/clang++', '-std=c++20', '-Wall',
                                 '-Wextra', '-Werror', str(cc), '-o', str(exe)],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        return subprocess.run([str(exe)], capture_output=True).returncode

    def test_actual_root_walk_and_volume_body(self):
        self.assertEqual(self.run_source(self.source, 'actual'), 0)

    def test_local_only_mutation_rejected(self):
        altered = self.source.replace('GetOutermostWebContents()->observers_', 'observers_')
        self.assertNotEqual(self.run_source(altered, 'local'), 0)

    def test_boolean_conversion_mutation_rejected(self):
        altered = self.source.replace('&WebContentsObserver::MediaVolumeChanged, volume);',
                                      '&WebContentsObserver::MediaVolumeChanged, volume == 0);')
        self.assertNotEqual(self.run_source(altered, 'boolean'), 0)

    def test_local_and_root_fanout_mutation_rejected(self):
        altered = self.source.replace('  GetOutermostWebContents()->observers_.NotifyObservers(',
            '  observers_.NotifyObservers(&WebContentsObserver::MediaVolumeChanged, volume);\n'
            '  GetOutermostWebContents()->observers_.NotifyObservers(')
        self.assertNotEqual(self.run_source(altered, 'fanout'), 0)


if __name__ == '__main__':
    unittest.main(verbosity=2)
