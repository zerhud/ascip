#!/usr/bin/env bash

g++ -std=c++23 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/vec_list.cpp -o /tmp/ascip_vec_list &

g++ -std=c++23 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/version.cpp -o /tmp/version &

g++ -std=c++23 -ftemplate-backtrace-limit=0 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/type.cpp -o /tmp/type &

g++ -std=c++23 -ftemplate-backtrace-limit=0 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/inheritance.cpp -o /tmp/inheritance &

g++ -std=c++23 -ftemplate-backtrace-limit=0 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/left_recursion.cpp -o /tmp/left_recursion &
clang++ -std=c++23 -ftemplate-backtrace-limit=0 -march=native -fdiagnostics-color=always -I. examples/left_recursion.cpp -o /tmp/left_recursion_clang &

wait $(jobs -rp)

/tmp/ascip_vec_list
/tmp/version
/tmp/type
/tmp/inheritance
/tmp/left_recursion

echo ""
echo "==========="
echo "clang tests"
echo "==========="
echo ""

/tmp/left_recursion_clang
