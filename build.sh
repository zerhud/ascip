#!/usr/bin/env bash

#g++ -std=c++23 -fwhole-program -march=native ./test.cpp -o ascip_test 2>&1 | pee xsel less
#./ascip_test 2>&1 |less

gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip2.hpp 2>/dev/null |awk '/# 1 "ascip2.hpp"/{DO_PRINT=1;} (DO_PRINT && !/#/){print;}' > build/ascip.hpp 

./run_tests.sh 2>&1 | less

g++ -std=c++23 -fwhole-program -march=native \
	./test2.cpp -o ascip2_test \
	-fconcepts-diagnostics-depth=3 -fdiagnostics-color=always \
	2>&1 | pee xsel less
./ascip2_test 2>&1 |less
