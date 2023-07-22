#!/usr/bin/env bash

#g++ -std=c++23 -fwhole-program -march=native ./test.cpp -o ascip_test 2>&1 | pee xsel less
#./ascip_test 2>&1 |less

g++ -std=c++23 -fwhole-program -march=native \
	./test2.cpp -o ascip2_test \
	-fconcepts-diagnostics-depth=3 -fdiagnostics-color=always \
	2>&1 | pee xsel less
./ascip2_test 2>&1 |less
