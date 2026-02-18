CXX = clang++
LD = wasm-ld
WASM2WAT = ~/build/wabt/wasm2wat

OUTPUT = library.wasm
OBJ = library.o
NANOLIBC_OBJ = $(patsubst %.cpp,%.o,$(wildcard nanolibc/*.cpp))
ALL_OBJ = $(OBJ) $(NANOLIBC_OBJ)
DEPS = $(ALL_OBJ:.o=.d)

CXXFLAGS = -Wall \
		--target=wasm32 \
		-Os \
		-nostdlib \
		-fvisibility=hidden \
		-std=c++14 \
		-ffunction-sections \
		-fdata-sections \
		-DPRINTF_DISABLE_SUPPORT_FLOAT=1 \
		-DPRINTF_DISABLE_SUPPORT_LONG_LONG=1 \
		-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T=1 \
		-MMD -MP

LDFLAGS = --no-entry \
		--strip-all \
		--export-dynamic \
		--allow-undefined \
		--initial-memory=131072 \
		--error-limit=0 \
		--lto-O3 \
		-O3 \
		--gc-sections

.PHONY: all clean wat

all: $(OUTPUT)

$(OUTPUT): $(ALL_OBJ)
	$(LD) -o $@ $(LDFLAGS) $^

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

-include $(DEPS)

library.wat: $(OUTPUT)
	$(WASM2WAT) -o $@ $<

wat: library.wat

clean:
	rm -f $(ALL_OBJ) $(DEPS) $(OUTPUT) library.wat
