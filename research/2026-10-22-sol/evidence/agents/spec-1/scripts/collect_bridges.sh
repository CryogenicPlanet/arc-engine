#!/bin/bash
set -euo pipefail
python3 /home/exedev/work/read-function.py 0x47390dc 0x4739114 0x47391c8 0x4376580 0x437854c 0x440e174 0x4330cdc 0x4330d38 0x43328cc 0x4332814 > specs/evidence/bridges-embedding.txt
python3 /home/exedev/work/read-function.py 0x436c99c 0x436cb2c 0x436ccbc 0x436cec4 0x436d054 0x42f0254 0x42e6790 0x42eb484 0x4337e00 > specs/evidence/bridges-services.txt
python3 /home/exedev/work/read-function.py 0x4737074 0x4737eb4 0x4738a30 0x4738bb0 0x435f150 0x435f1ac 0x42efb64 0x836ba70 0x32b5dd0 > specs/evidence/bridges-deep.txt
python3 /home/exedev/work/read-function.py 0x444510c 0x4445338 0x4405db4 0x4377d70 0x43770c0 0x4332a34 0x42eb57c 0x7c6a394 0x7c6a4f4 > specs/evidence/bridges-callbacks.txt
python3 /home/exedev/work/read-function.py 0x1c391bc 0x1d9fb90 0x43abd34 0x3c2b700 0x4440dac 0x4341964 > specs/evidence/bridges-conversions.txt
python3 /home/exedev/work/read-function.py 0x1b92718 0x1941cb4 > specs/evidence/bridges-bootstrap.txt
