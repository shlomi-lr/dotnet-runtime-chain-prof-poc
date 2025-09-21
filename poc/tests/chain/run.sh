#!/usr/bin/env bash
set -euo pipefail

# PoC chain-host E2E harness
# - Builds managed harness (ChainTestApp + Attacher)
# - Builds skeletal native profilers (ChainHost + NopProfiler)
# - Runs ChainTestApp on a specified dotnet (prefer patched testhost)
# - Attaches NopProfiler twice to validate reattach handoff

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
CONFIG=${CONFIG:-Release}

# Determine platform extension
UNAME_S=$(uname)
if [[ "$UNAME_S" == "Darwin" ]]; then EXT="dylib"; else EXT="so"; fi

# Resolve dotnet to run the target app
# Prefer a locally built testhost, but allow override via DOTNET env
if [[ -z "${DOTNET:-}" ]]; then
  while IFS= read -r -d '' candidate; do
    DOTNET="$candidate"; break
  done < <(find "$ROOT_DIR/artifacts/bin/testhost" -type f -name dotnet -path "*net6.0-*-$CONFIG-*" -print0 2>/dev/null || true)
fi
DOTNET=${DOTNET:-$(command -v dotnet)}

# Build managed harness
( cd "$ROOT_DIR/poc/ChainTestApp" && dotnet build -c "$CONFIG" )
( cd "$ROOT_DIR/poc/Attacher"     && dotnet build -c "$CONFIG" )

# Build skeletal profilers (unless skipped)
if [[ -z "${SKIP_NATIVE:-}" ]]; then
  ( cd "$ROOT_DIR/poc/profilers" && make clean all )
fi

APP_DLL="$ROOT_DIR/poc/ChainTestApp/bin/$CONFIG/net6.0/ChainTestApp.dll"
ATT_DLL="$ROOT_DIR/poc/Attacher/bin/$CONFIG/net6.0/Attacher.dll"
HOST_LIB="${HOST_LIB:-$ROOT_DIR/poc/profilers/ChainHost.$EXT}"
NOP_LIB="${NOP_LIB:-$ROOT_DIR/poc/profilers/NopProfiler.$EXT}"

HOST_GUID="{2A2A2A2A-2A2A-2A2A-2A2A-2A2A2A2A2A2A}"
NOP_GUID="{3B3B3B3B-3B3B-3B3B-3B3B-3B3B3B3B3B3B}"

# Launch the target app under the resident chain host
LOG_DIR="$ROOT_DIR/poc/tests/chain/_run"
mkdir -p "$LOG_DIR"
APP_LOG="$LOG_DIR/app.log"
ATT_LOG="$LOG_DIR/attach.log"
: >"$APP_LOG"; : >"$ATT_LOG"

echo "Using DOTNET: $DOTNET"
echo "App: $APP_DLL"
echo "Resident: $HOST_LIB ($HOST_GUID)"
echo "Hosted:  $NOP_LIB ($NOP_GUID)"

if [[ ! -f "$HOST_LIB" || ! -f "$NOP_LIB" ]]; then
  echo "Profiler libraries not found."
  echo "  HOST_LIB=$HOST_LIB"
  echo "  NOP_LIB=$NOP_LIB"
  echo "Build them (default) or set HOST_LIB/NOP_LIB to existing paths."
  exit 2
fi

COMPLUS=COMPlus_ProfilerChain
# Use either COMPlus_ProfilerChain or CORECLR_PROFILER_CHAIN
CHAIN_ENV="COMPlus_ProfilerChain=1"

set +e
COMPlus_ProfilerChain=1 CORECLR_ENABLE_PROFILING=1 CORECLR_PROFILER="$HOST_GUID" CORECLR_PROFILER_PATH="$HOST_LIB" \
  "$DOTNET" exec "$APP_DLL" >"$APP_LOG" 2>&1 &
PID=$!
set -e

echo "Started ChainTestApp pid=$PID"
# Give it a moment to initialize and load the resident profiler
sleep 2

# First attach attempt
set +e
"$DOTNET" exec "$ATT_DLL" "$PID" "$NOP_GUID" "$NOP_LIB" | tee -a "$ATT_LOG"
E1=$?
# Second attach attempt
"$DOTNET" exec "$ATT_DLL" "$PID" "$NOP_GUID" "$NOP_LIB" | tee -a "$ATT_LOG"
E2=$?
set -e

# Cleanup app
kill "$PID" >/dev/null 2>&1 || true
wait "$PID" 2>/dev/null || true

# Summarize
echo
echo "---- Summary ----"
echo "First attach exit:  $E1"
echo "Second attach exit: $E2"

if [[ "$DOTNET" == *"/artifacts/bin/testhost/"* ]]; then
  # Expect success on patched testhost
  if [[ $E1 -eq 0 && $E2 -eq 0 ]]; then
    echo "PASS: Attach + Reattach succeeded under patched runtime."
    exit 0
  else
    echo "FAIL: Expected both attaches to succeed with patched runtime."
    echo "Inspect logs: $APP_LOG and $ATT_LOG"
    exit 1
  fi
else
  # Likely unpatched stock runtime - first attach should fail with 'already active'
  if [[ $E1 -ne 0 ]]; then
    echo "NOTE: First attach failed under stock runtime (expected)."
    echo "To validate chain mode, set DOTNET to your patched testhost."
    echo "Logs: $APP_LOG and $ATT_LOG"
    exit 0
  else
    echo "WARN: First attach unexpectedly succeeded on stock runtime."
    echo "Verify DOTNET points to patched testhost."
    exit 1
  fi
fi
