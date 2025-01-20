mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
path := $(mkfile_dir)
build_path := $(path)/build
dep := g++ -MMD -pipe -std=c++23
gcc := g++ -MMD -pipe -fwhole-program -march=native -std=gnu++23 -fdiagnostics-color=always
clang := clang++ -MMD -march=native -std=c++23 -fdiagnostics-color=always

example_files := $(shell find . -ipath './examples/*.cpp' | sed 's/^..//g')
tests_files := $(shell find . -ipath './tests/*.cpp' | sed 's/^..//g')
base_example = $(basename $(subst examples/,,$(1)))
base_tests = $(basename $(subst tests/,,$(1)))

.PHONY: all clean test

all: test $(build_path)/ascip.hpp examples_gcc examples_clang $(build_path)/ascip.hpp $(build_path)/ascip_clang.hpp
	@echo -e "\e[7;32mAll Done\e[0m"

$(build_path): makefile
	mkdir -p $(build_path)

-include $(build_path)/ascip.hpp.d
$(build_path)/ascip.hpp.d: makefile ascip.hpp | $(build_path)
	gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp -MM | sed 's#ascip.o: ascip.hpp#$(build_path)/ascip.hpp:#' > $(build_path)/ascip.hpp.d
$(build_path)/ascip.hpp: makefile ascip.hpp $(build_path)/ascip.hpp.d | $(build_path)
	gcc -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp 2>/dev/null |awk '/namespace ascip_details/{DO_PRINT=1;} (DO_PRINT && !/#/){print;}' > $(build_path)/ascip.hpp
$(build_path)/ascip_clang.hpp: makefile ascip.hpp $(build_path)/ascip.hpp | $(build_path)
	clang++ -fdirectives-only -DINCLUDE_STD=1 -E ascip.hpp 2>/dev/null |awk '/# 1 "ascip.hpp"/{DO_PRINT=1;} (DO_PRINT && !/#/){print;}' > $(build_path)/ascip_clang.hpp

inc_file: $(build_path)/ascip.hpp

define create_examples_template
-include $(build_path)/example_$(call base_example,$(1))_gcc.d
$(build_path)/example_$(call base_example,$(1))_gcc: makefile $(1) | $(build_path)
	$(gcc) -I. $(1) -o $$@ && $$@
-include $(build_path)/example_$(call base_example,$(1))_clang.d
$(build_path)/example_$(call base_example,$(1))_clang: makefile $(1) | $(build_path)
	$(clang) -I. $(1) -o $$@ && $$@

examples_gcc:: $(build_path)/example_$(call base_example,$(1))_gcc 
examples_clang:: $(build_path)/example_$(call base_example,$(1))_clang
clean::
	rm -f $(build_path)/example_$(call base_example,$(1))_{gcc,clang}{,.d}

endef
$(foreach src_file,$(example_files),$(eval $(call create_examples_template,$(src_file))))

define create_tests_template
-include $(build_path)/test_$(call base_tests,$(1))_gcc.d
$(build_path)/test_$(call base_tests,$(1))_gcc: makefile $(1) | $(build_path)
	$(gcc) -I. $(1) -o $$@ && $$@
-include $(build_path)/test_$(call base_tests,$(1))_clang.d
$(build_path)/test_$(call base_tests,$(1))_clang: makefile $(1) | $(build_path)
	$(clang) -I. $(1) -o $$@ && $$@

tests_gcc:: $(build_path)/test_$(call base_tests,$(1))_gcc 
tests_clang:: $(build_path)/test_$(call base_tests,$(1))_clang
clean::
	rm -f $(build_path)/test_$(call base_tests,$(1))_{gcc,clang}{,.d}

endef
$(foreach src_file,$(tests_files),$(eval $(call create_tests_template,$(src_file))))

test: $(build_path)/ascip.hpp tests_gcc tests_clang
	$(build_path)/test_rt_error_handling_gcc
	echo "====== clang ======"
	$(build_path)/test_rt_error_handling_clang

clean::
	rm -f $(build_path)/ascip{,_clang}.hpp
	rm -f $(build_path)/ascip.hpp.d

