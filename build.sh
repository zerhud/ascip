#!/usr/bin/env bash

tmpfile=$(mktemp)
exec 3>"$tmpfile"
echo "wite to $tmpfile"

gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp 2>/dev/null |awk '/# 1 "ascip.hpp"/{DO_PRINT=1;} (DO_PRINT && !/#/){print;}' > build/ascip.hpp  &

g++ -std=c++23 -fwhole-program -march=native \
	./test.cpp -o ascip_test \
	-ftemplate-backtrace-limit=0 \
	-fconcepts-diagnostics-depth=3 -fdiagnostics-color=always \
	2>&1 2>&3 >&3 &
	#2>&1 | pee xsel less

./run_tests.sh 2>&1 | less

wait
cat "$tmpfile" | pee xsel less
rm "$tmpfile"

./ascip_test 2>&1 |less
