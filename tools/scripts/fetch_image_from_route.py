#!/usr/bin/env python3
import sys

if len(sys.argv) < 4:
  print(f"{sys.argv[0]} <route> <segment> <frame number> [front|wide|driver]")
  print('example: ./fetch_image_from_route.py "02c45f73a2e5c6e9|2020-06-01--18-03-08" 3 500 driver')
  exit(0)

cameras = {
  "front": "cameras",
  "wide": "ecameras",
  "driver": "dcameras"
}

import requests
from PIL import Image
from openpilot.tools.lib.auth_config import get_token
from openpilot.tools.lib.framereader import FrameReader

jwt = get_token()

route = sys.argv[1]
segment = int(sys.argv[2])
frame = int(sys.argv[3])
camera = cameras[sys.argv[4]] if len(sys.argv) > 4 and sys.argv[4] in cameras else "cameras"

url = f'https://api.commadotai.com/v1/route/{route}/files'
r = requests.get(url, headers={"Authorization": f"JWT {jwt}"}, timeout=10)
assert r.status_code == 200
print("got api response")

segments = r.json()[camera]
if segment >= len(segments):
  raise Exception("segment %d not found, got %d segments" % (segment, len(segments)))

fr = FrameReader(segments[segment])
if frame >= fr.frame_count:
  raise Exception("frame %d not found, got %d frames" % (frame, fr.frame_count))

im = Image.fromarray(fr.get(frame, count=1, pix_fmt="rgb24")[0])
fn = f"uxxx_{route.replace('|', '_')}_{segment}_{frame}.png"
im.save(fn)
print(f"saved {fn}")
