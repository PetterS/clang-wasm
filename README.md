# Webassemly with only Clang 8

I did not find instructions for this anywhere online, so here they are. We can now build small wasm files with nothing other than Clang (e.g. Emscripten is not needed).

1. Install Clang 8 or later. Linux packages are [here](https://apt.llvm.org/).
2. `make`
3. To try the built wasm file, run `python3 server.py`.

Optional: To disassemble the wasm file, you can install [wabt](https://github.com/WebAssembly/wabt) and run `make wat`.
