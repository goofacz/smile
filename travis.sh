#!/bin//bash
set +x

cd /root/smile
pip install -r requirements.txt

make makefiles
make -j `nproc` MODE=release
