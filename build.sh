#!/usr/bin/env bash

g++ -std=c++23 -fwhole-program -march=native ./test.cpp -o ascip_test 2>&1 | pee xsel less
./ascip_test 2>&1 |less
