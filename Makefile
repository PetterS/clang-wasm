DEPS = 
OBJ = library.o
OUTPUT = library.wasm

$(OUTPUT): $(OBJ) library.syms Makefile
	wasm-ld-8 \
		-o $(OUTPUT) \
		--no-entry \
		-allow-undefined-file library.syms \
		--strip-all \
		--export-dynamic \
		--initial-memory=131072 \
		$(OBJ)

%.o: %.c $(DEPS) Makefile
	clang-8 \
		-c \
		-Wall \
		-Werror \
		--target=wasm32 \
		-Os \
		-o $@ \
		-nostdlib \
		-fvisibility=hidden \
		$<

library.wat: $(OUTPUT) Makefile
	~/build/wabt/wasm2wat -o library.wat $(OUTPUT)

wat: library.wat

clean:
	rm -f $(OBJ) $(OUTPUT) library.wat
