mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
path := $(mkfile_dir)
build_path := $(path)/build
dep := g++ -MMD -pipe -std=c++23
gcc := g++ -MMD -pipe -fwhole-program -march=native -std=gnu++23 -fdiagnostics-color=always
clang := clang++ -march=native -std=c++23 -fdiagnostics-color=always

.PHONY: all clean test

all: test $(build_path)/ascip.hpp examples $(build_path)/ascip.hpp

-include $(build_path)/main_test.d
$(build_path)/main_test: makefile $(path)/test.cpp
	$(gcc) $(path)/test.cpp -o $(build_path)/main_test
$(build_path)/main_test_clang: makefile $(path)/test.cpp
	$(clang) $(path)/test.cpp -o $(build_path)/main_test_clang

-include $(build_path)/ascip.hpp.d
$(build_path)/ascip.hpp: makefile ascip.hpp
	gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp -MM | sed 's#ascip.o: ascip.hpp#$(build_path)/ascip.hpp:#' > $(build_path)/ascip.hpp.d
	gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp 2>/dev/null |awk '/# 1 "ascip.hpp"/{DO_PRINT=1;} (DO_PRINT && !/#/){print;}' > $(build_path)/ascip.hpp

inc_file: $(build_path)/ascip.hpp

examples: $(build_path)/vec_list $(build_path)/version $(build_path)/type $(build_path)/inheritance $(build_path)/left_reqursion $(build_path)/left_reqursion_clang

-include $(build_path)/vec_list.d
$(build_path)/vec_list: makefile examples/vec_list.cpp
	$(gcc) -I. examples/vec_list.cpp -o $(build_path)/vec_list
-include $(build_path)/version.d
$(build_path)/version: makefile examples/version.cpp
	$(gcc) -I. examples/version.cpp -o $(build_path)/version
-include $(build_path)/type.d
$(build_path)/type: makefile examples/type.cpp
	$(gcc) -I. examples/type.cpp -o $(build_path)/type
-include $(build_path)/inheritance.d
$(build_path)/inheritance: makefile examples/inheritance.cpp
	$(gcc) -I. examples/inheritance.cpp -o $(build_path)/inheritance
-include $(build_path)/left_reqursion.d
$(build_path)/left_reqursion: makefile examples/left_reqursion.cpp
	$(gcc) -I. examples/left_reqursion.cpp -o $(build_path)/left_reqursion
$(build_path)/left_reqursion_clang: makefile examples/left_reqursion.cpp
	$(clang) -I. examples/left_reqursion.cpp -o $(build_path)/left_reqursion_clang

test: $(build_path)/main_test $(build_path)/main_test_clang $(build_path)/ascip.hpp
	$(build_path)/main_test
	echo "====== clang ======"
	$(build_path)/main_test_clang

clean:
	rm -f $(build_path)/main_test.d
	rm -f $(build_path)/main_test
	rm -f $(build_path)/main_test_clang
	rm -f $(build_path)/vec_list{,.d}
	rm -f $(build_path)/version{,.d}
	rm -f $(build_path)/type{,.d}
	rm -f $(build_path)/inheritance{,.d}
	rm -f $(build_path)/left_reqursion{,.d}
	rm -f $(build_path)/left_reqursion_clang{,.d}
	rm -f $(build_path)/ascip.hpp.d

