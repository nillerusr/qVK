#!/bin/sh

mkdir -p build
cd build

qmake .. || exit 1
make -j$(nproc --all) $* || exit 1
