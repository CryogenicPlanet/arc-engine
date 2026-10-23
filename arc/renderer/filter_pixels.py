#!/usr/bin/env python3
"""Read measured swatch pixels from native filter probe screenshots.

No screenshot editing, pixel normalization, or assertion about implementation
coverage is performed. A paired run compares the measured histograms exactly.
"""
import collections
import json
from pathlib import Path
import sys

from PIL import Image

root = Path(sys.argv[1]).resolve()
runs = {}
for name in ('trace156', 'ours156'):
    path = root / name / 'filter-observations.json'
    if not path.exists():
        continue
    observations = json.loads(path.read_text())
    measured = []
    for observation in observations:
        geometry = observation['geometry']
        with Image.open(path.parent / observation['screenshot']) as image:
            image = image.convert('RGBA')
            scale_x = image.width / geometry['width']
            scale_y = image.height / geometry['height']
            swatches = {}
            for swatch in geometry['swatches']:
                # Exclude two CSS pixels of the edge; retain actual inner pixels.
                box = (round((swatch['x'] + 2) * scale_x),
                       round((swatch['y'] + 2) * scale_y),
                       round((swatch['x'] + swatch['width'] - 2) * scale_x),
                       round((swatch['y'] + swatch['height'] - 2) * scale_y))
                if not (0 <= box[0] < box[2] <= image.width and
                        0 <= box[1] < box[3] <= image.height):
                    raise SystemExit(f'{name}: swatch outside screenshot: {box}')
                colors = collections.Counter(image.crop(box).getdata())
                swatches[swatch['name']] = [
                    {'rgba': list(color), 'count': count}
                    for color, count in sorted(colors.items())]
            measured.append({'step': observation['step'], 'swatches': swatches})
    runs[name] = measured
    (path.parent / 'filter-pixels.json').write_text(json.dumps(measured, indent=2) + '\n')
    print(f'{name}: {len(measured)} screenshots measured')
if not runs:
    raise SystemExit('no filter observations found')
paired = set(runs) == {'trace156', 'ours156'}
report = {'runs': runs, 'paired': paired,
          'exact_pixel_histograms': runs['trace156'] == runs['ours156'] if paired else None}
(root / 'filter-pixel-comparison.json').write_text(json.dumps(report, indent=2) + '\n')
if paired and not report['exact_pixel_histograms']:
    raise SystemExit('filter pixels differ; inspect filter-pixel-comparison.json')
