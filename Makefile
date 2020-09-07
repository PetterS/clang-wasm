DEPS = 
#LLVM_VERSION ="-8"
OBJ = library.o
NANOLIBC_OBJ = $(patsubst %.cpp,%.o,$(wildcard nanolibc/*.cpp))
OUTPUT = library.wasm

COMPILE_FLAGS = -Wall \
		--target=wasm32 \
		-I. \
		-Os \
		-flto \
		-nostdlib \
		-fvisibility=hidden \
		-std=c++14 \
		-ffunction-sections \
		-fdata-sections \
		-DPRINTF_DISABLE_SUPPORT_FLOAT=1 \
		-DPRINTF_DISABLE_SUPPORT_LONG_LONG=1 \
		-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T=1

$(OUTPUT): $(OBJ) $(NANOLIBC_OBJ) Makefile
	wasm-ld$(LLVM_VERSION) \
		-o $(OUTPUT) \
		--no-entry \
		--strip-all \
		--export-dynamic \
		--initial-memory=131072 \
		-error-limit=0 \
		--lto-O3 \
		-O3 \
		--gc-sections \
		--allow-undefined \
		$(OBJ) \
		$(LIBCXX_OBJ) \
		$(NANOLIBC_OBJ)


%.o: %.cpp $(DEPS) Makefile
	clang++$(LLVM_VERSION) \
		-c \
		$(COMPILE_FLAGS) \
		-o $@ \
		$<

library.wat: $(OUTPUT) Makefile
	~/build/wabt/wasm2wat -o library.wat $(OUTPUT)

wat: library.wat

clean:
	rm -f $(OBJ) $(NANOLIBC_OBJ) $(OUTPUT) library.wat
