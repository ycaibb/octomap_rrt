#!/bin/bash
mkdir -p build
cd build
cmake ..
make -j4
cp ../share/test_map_complex.bt ./
