#!/bin//bash
set +x

pip install -r requirements.txt

make makefiles
make -j `nproc` MODE=release
