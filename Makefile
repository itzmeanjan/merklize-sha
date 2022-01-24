CXX = dpcpp
CXX_FLAGS = -Wall -std=c++20
SYCL_FLAGS = -fsycl
OPT_FLAGS = -O3
IFLAGS = -I./include

all: test_impl

test/a.out: test/main.cpp include/*.hpp
	$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(IFLAGS) $< -o $@

test_impl: test/a.out
	./test/a.out

clean:
	find . -name 'a.out' -o -name 'run' -o -name '*.o' | xargs rm -f
