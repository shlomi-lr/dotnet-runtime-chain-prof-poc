CXX ?= clang++
OS := $(shell uname)

# Choose shared library extension and linker flags per platform
ifeq ($(OS),Darwin)
  EXT=dylib
  SHARED_FLAG=-dynamiclib
else
  EXT=so
  SHARED_FLAG=-shared
endif

INCLUDES= \
  -I../../src/coreclr/pal/prebuilt/inc \
  -I../../src/coreclr/pal/inc/rt \
  -I../../src/coreclr/pal/inc \
  -I../../src/coreclr/inc

CXXFLAGS ?= -O2 -fPIC -std=c++17 $(INCLUDES)
LDFLAGS ?= $(SHARED_FLAG)
LDLIBS ?=

HOST_LIB=ChainHost.$(EXT)
NOP_LIB=NopProfiler.$(EXT)

.PHONY: all clean
all: $(HOST_LIB) $(NOP_LIB)

$(HOST_LIB): chain_host.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(NOP_LIB): nop_profiler.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(HOST_LIB) $(NOP_LIB)
