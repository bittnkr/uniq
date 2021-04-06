#!/bin/bash
cmake -S tests -B out
cd out
make -j8
cd ..
./out/tests