// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// Profiling helper: chain PoC utilities
//

#include "common.h"
#ifdef PROFILING_SUPPORTED
#include "profilinghelper.h"
#include "eetoprofinterfaceimpl.h"

// Export signatures
typedef void (STDAPICALLTYPE *PFN_ProfilerChain_VirtualDetach)(ULONGLONG, DWORD);

static PFN_ProfilerChain_VirtualDetach s_pfnChainVirtualDetach = NULL;
static ULONGLONG s_chainSessionId = 0;

// static
void ProfilingAPIUtility::ChainRecordSession(HMODULE hResident, ULONGLONG sessionId)
{
    if (hResident == NULL || sessionId == 0)
        return;

    PFN_ProfilerChain_VirtualDetach pfn = (PFN_ProfilerChain_VirtualDetach)::GetProcAddress(hResident, "ProfilerChain_VirtualDetach");
    if (pfn != NULL)
    {
        s_pfnChainVirtualDetach = pfn;
        s_chainSessionId = sessionId;
    }
}

// static
void ProfilingAPIUtility::ChainVirtualDetachIfAny(DWORD reason)
{
    if (s_pfnChainVirtualDetach != NULL && s_chainSessionId != 0)
    {
        EX_TRY
        {
            s_pfnChainVirtualDetach(s_chainSessionId, reason);
        }
        EX_CATCH
        {
        }
        EX_END_CATCH(SwallowAllExceptions);
        s_pfnChainVirtualDetach = NULL;
        s_chainSessionId = 0;
    }
}

#endif // PROFILING_SUPPORTED
