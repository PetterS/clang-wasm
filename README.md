# Webassemly with only Clang 8

I did not find instructions for this anywhere online, so here they are. We can now build small wasm files with nothing other than Clang (e.g. Emscripten is not needed).

1. Install Clang 8 or later. Linux packages are [here](https://apt.llvm.org/).
2. `make`
3. To try the built wasm file, run `python3 server.py`.

Optional: To disassemble the wasm file, you can install [wabt](https://github.com/WebAssembly/wabt) and run `make wat`.

## Docker

The following command builds an ubuntu:18.04 image containing the dependencies needed to build the working example.

```bash
$ docker build -t clang-wasm-ubuntu:18.04 .
```

Then, you can mount the source files in a container running the image and follow the instructions above.


```bash
docker run --rm -it -v`pwd`:/opt/src -w /opt/src -p4242:4242 clang-wasm-ubuntu:18.04

root@74a067d2c9e6:/opt/src# make
.... <snip> ...
root@74a067d2c9e6:/opt/src# python3 server.py
serving at port 4242
```
