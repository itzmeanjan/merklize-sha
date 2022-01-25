# takes quite some motivation from https://github.com/itzmeanjan/blake3/blob/6b2ca43f6713b873dd26da2004241d894b2df729/Makefile

CXX = dpcpp
CXX_FLAGS = -Wall -std=c++20
SYCL_FLAGS = -fsycl
SYCL_CUDA_FLAGS = -fsycl-targets=nvptx64-nvidia-cuda
SYCL_CPU_FLAGS = -fsycl-targets=spir64_x86_64
SYCL_GPU_FLAGS = $(SYCL_GPU_FLAGS)
OPT_FLAGS = -O3
IFLAGS = -I./include
SHA_VARIANT = -D$(shell echo $(or $(SHA),sha2_256) | tr a-z A-Z)

all: test_impl

test/a.out: test/main.cpp include/*.hpp
	$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $< -o $@

test_impl: test/a.out
	./test/a.out

clean:
	find . -name 'a.out' -o -name 'run' -o -name '*.o' | xargs rm -f

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style=Mozilla

bench/a.out: bench/main.cpp include/*.hpp
	$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $< -o $@

benchmark: bench/a.out
	./bench/a.out

aot_cpu:
	@if lscpu | grep -q 'avx512'; then \
		echo "Using avx512"; \
		$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $(SYCL_CPU_FLAGS) -Xs "-march=avx512" bench/main.cpp -o bench/a.out; \
	elif lscpu | grep -q 'avx2'; then \
		echo "Using avx2"; \
		$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $(SYCL_CPU_FLAGS) -Xs "-march=avx2" bench/main.cpp -o bench/a.out; \
	elif lscpu | grep -q 'avx'; then \
		echo "Using avx"; \
		$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $(SYCL_CPU_FLAGS) -Xs "-march=avx" bench/main.cpp -o bench/a.out; \
	elif lscpu | grep -q 'sse4.2'; then \
		echo "Using sse4.2"; \
		$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $(SYCL_CPU_FLAGS) -Xs "-march=sse4.2" bench/main.cpp -o bench/a.out; \
	else \
		echo "Can't AOT compile using avx, avx2, avx512 or sse4.2"; \
	fi
	./bench/a.out

aot_gpu:
	# you may want to replace `device` identifier with `0x3e96` if you're targeting *Intel(R) UHD Graphics P630*
	#
	# otherwise, let it be what it's if you're targeting *Intel(R) Iris(R) Xe MAX Graphics*
	$(CXX) $(CXX_FLAGS) $(SYCL_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) $(SYCL_GPU_FLAGS) -Xs "-device 0x4905" bench/main.cpp -o bench/a.out
	./bench/a.out

cuda:
	clang++ $(CXX_FLAGS) $(SYCL_FLAGS) $(SYCL_CUDA_FLAGS) $(OPT_FLAGS) $(SHA_VARIANT) $(IFLAGS) bench/main.cpp -o bench/a.out
	./bench/a.out
