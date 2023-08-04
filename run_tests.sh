#!/usr/bin/env bash

g++ -std=c++23 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/vec_list.cpp -o /tmp/ascip_vec_list 2>&1
/tmp/ascip_vec_list 2>&1

g++ -std=c++23 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/version.cpp -o /tmp/version 2>&1
/tmp/version 2>&1

g++ -std=c++23 -ftemplate-backtrace-limit=0 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/type.cpp -o /tmp/type 2>&1

g++ -std=c++23 -ftemplate-backtrace-limit=0 -fwhole-program -march=native -fdiagnostics-color=always -I. examples/inheritance.cpp -o /tmp/inheritance 2>&1
/tmp/type 2>&1
