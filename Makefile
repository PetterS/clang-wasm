
library.wasm: library.c Makefile
	# sudo ln -s /usr/lib/llvm-8/bin/wasm-ld /usr/bin/wasm-ld
	clang-8 \
		--target=wasm32 \
		-Os \
		-o library.wasm \
		-nostdlib \
		--for-linker=--no-entry \
		--for-linker=-export=petter \
		library.c

library.wat: library.wasm Makefile
	~/build/wabt/wasm2wat -o library.wat library.wasm

wat: library.wat

clean:
	rm library.wasm
