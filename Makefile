DEPS = 
OBJ = library.o
OUTPUT = library.wasm

$(OUTPUT): $(OBJ) Makefile
	wasm-ld-8 \
		-o $(OUTPUT) \
		--no-entry \
		--export=petter \
		$(OBJ)

%.o: %.c $(DEPS)
	clang-8 \
		-c \
		--target=wasm32 \
		-Os \
		-o $@ \
		-nostdlib \
		$<

library.wat: $(OUTPUT) Makefile
	~/build/wabt/wasm2wat -o library.wat $(OUTPUT)

wat: library.wat

clean:
	rm -f $(OBJ) $(OUTPUT) library.wat
