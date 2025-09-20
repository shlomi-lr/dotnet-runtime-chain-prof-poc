CXX=clang++
CXXFLAGS=-fPIC -I../../src/coreclr/inc -I../../src/coreclr/pal/prebuilt/inc -O2 -std=c++17
LDFLAGS=-shared
LDLIBS=

all: ChainHost.so NopProfiler.so

ChainHost.so: chain_host.cpp
	  -o  $^  

NopProfiler.so: nop_profiler.cpp
	  -o  $^  

clean:
	rm -f ChainHost.so NopProfiler.so
