Chain Mode PoC Test (CLR-only)

This test exercises the CLR-only chain-host changes by:
- Launching a simple .NET 6 app with a resident chain host profiler
- Attaching a second profiler (NOP or another) via DiagnosticsClient
- Attaching a second time to validate re-attach behavior

Contents:
- ../profilers/chain_host.cpp: skeletal chain host with ProfilerChain_AttachOffer / VirtualDetach
- ../profilers/nop_profiler.cpp: skeletal hosted NOP profiler
- ../Attacher: C# console that calls DiagnosticsClient.AttachProfiler
- ../ChainTestApp: simple net6.0 app that sleeps

Quick run (using existing ProfilerChain.so + Lightrun profiler as reference):

export HOST_GUID={C67C1FAC-843F-4C7A-9545-E09660CF10AB}
export LIGHT_GUID={FC15CFC2-6CE8-45DF-A754-079254E0077B}
CHAIN=/home/shlomi/repo/dotnet/athena/dotnet-agent-v2/native/dist/linux-x64/ProfilerChain.so
LIGHT=/home/shlomi/repo/dotnet/athena/dotnet-agent-v2/native/dist/linux-x64/Lightrun.ClrProfiler.Native.so
DOTNET=/home/shlomi/repo/dotnet/.dotnet/dotnet
APP=/home/shlomi/tmp/chainpoc/ChainTestApp/bin/Release/net6.0/ChainTestApp.dll
ATT=/home/shlomi/tmp/chainpoc/Attacher/bin/Release/net6.0/Attacher.dll

CORECLR_ENABLE_PROFILING=1 \
CORECLR_PROFILER=$HOST_GUID \
CORECLR_PROFILER_PATH=$CHAIN \
CORECLR_PROFILER_CHAIN=1 \
LIGHTRUN_CHAIN_ALLOW_CHAIN_ONLY=true \
$DOTNET exec "$APP" & export PID=$!

sleep 2
$DOTNET exec "$ATT" $PID $LIGHT_GUID $LIGHT
$DOTNET exec "$ATT" $PID $LIGHT_GUID $LIGHT
kill $PID

To build and use the skeletal profilers instead of ProfilerChain/Lightrun, see ../profilers/Makefile (adjust include paths if needed).
