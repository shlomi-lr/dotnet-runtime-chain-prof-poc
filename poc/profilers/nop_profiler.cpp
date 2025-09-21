#include <atomic>

#include "corprof.h"

// NOP CLSID: {3B3B3B3B-3B3B-3B3B-3B3B-3B3B3B3B3B3B}
static const GUID CLSID_Nop = {0x3b3b3b3b,0x3b3b,0x3b3b,{0x3b,0x3b,0x3b,0x3b,0x3b,0x3b,0x3b,0x3b}};

class NopProfiler : public ICorProfilerCallback3 {
    std::atomic<ULONG> _ref{1};
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (!ppv) return E_POINTER;
        if (riid == __uuidof(IUnknown) || riid == __uuidof(ICorProfilerCallback) ||
            riid == __uuidof(ICorProfilerCallback2) || riid == __uuidof(ICorProfilerCallback3)) {
            *ppv = static_cast<ICorProfilerCallback3*>(this); AddRef(); return S_OK;
        }
        *ppv = nullptr; return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++_ref; }
    ULONG STDMETHODCALLTYPE Release() override { ULONG v = --_ref; if (!v) delete this; return v; }
    HRESULT STDMETHODCALLTYPE Initialize(IUnknown*) override { return S_OK; }
    HRESULT STDMETHODCALLTYPE Shutdown() override { return S_OK; }
    HRESULT STDMETHODCALLTYPE InitializeForAttach(IUnknown*, void*, UINT){ return S_OK; }
    HRESULT STDMETHODCALLTYPE ProfilerAttachComplete(){ return S_OK; }
    HRESULT STDMETHODCALLTYPE ProfilerDetachSucceeded(){ return S_OK; }
    // remaining callbacks no-op
    HRESULT STDMETHODCALLTYPE ThreadCreated(ThreadID){return S_OK;} HRESULT STDMETHODCALLTYPE ThreadDestroyed(ThreadID){return S_OK;}
    HRESULT STDMETHODCALLTYPE AppDomainCreationStarted(AppDomainID){return S_OK;} HRESULT STDMETHODCALLTYPE AppDomainCreationFinished(AppDomainID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE AppDomainShutdownStarted(AppDomainID){return S_OK;} HRESULT STDMETHODCALLTYPE AppDomainShutdownFinished(AppDomainID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE AssemblyLoadStarted(AssemblyID){return S_OK;} HRESULT STDMETHODCALLTYPE AssemblyLoadFinished(AssemblyID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE AssemblyUnloadStarted(AssemblyID){return S_OK;} HRESULT STDMETHODCALLTYPE AssemblyUnloadFinished(AssemblyID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE ModuleLoadStarted(ModuleID){return S_OK;} HRESULT STDMETHODCALLTYPE ModuleLoadFinished(ModuleID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE ModuleUnloadStarted(ModuleID){return S_OK;} HRESULT STDMETHODCALLTYPE ModuleUnloadFinished(ModuleID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE ModuleAttachedToAssembly(ModuleID,AssemblyID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ClassLoadStarted(ClassID){return S_OK;} HRESULT STDMETHODCALLTYPE ClassLoadFinished(ClassID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE ClassUnloadStarted(ClassID){return S_OK;} HRESULT STDMETHODCALLTYPE ClassUnloadFinished(ClassID,HRESULT){return S_OK;}
    HRESULT STDMETHODCALLTYPE FunctionUnloadStarted(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITCompilationStarted(FunctionID,BOOL){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITCompilationFinished(FunctionID,HRESULT,BOOL){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITCachedFunctionSearchStarted(FunctionID,BOOL*){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITCachedFunctionSearchFinished(FunctionID,COR_PRF_JIT_CACHE){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITFunctionPitched(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE JITInlining(FunctionID,FunctionID,BOOL*){return S_OK;}
    HRESULT STDMETHODCALLTYPE UnmanagedToManagedTransition(FunctionID,COR_PRF_TRANSITION_REASON){return S_OK;}
    HRESULT STDMETHODCALLTYPE ManagedToUnmanagedTransition(FunctionID,COR_PRF_TRANSITION_REASON){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeSuspendFinished(){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeSuspendAborted(){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeResumeStarted(){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeResumeFinished(){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeThreadSuspended(ThreadID){return S_OK;}
    HRESULT STDMETHODCALLTYPE RuntimeThreadResumed(ThreadID){return S_OK;}
    HRESULT STDMETHODCALLTYPE MovedReferences(ULONG,ObjectID[],ObjectID[],ULONG[]){return S_OK;}
    HRESULT STDMETHODCALLTYPE ObjectAllocated(ObjectID,ClassID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ObjectsAllocatedByClass(ULONG,ClassID[],ULONG[]){return S_OK;}
    HRESULT STDMETHODCALLTYPE ObjectReferences(ObjectID,ClassID,ULONG,ObjectID[]){return S_OK;}
    HRESULT STDMETHODCALLTYPE RootReferences(ULONG,ObjectID[]){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionThrown(ObjectID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionSearchFunctionEnter(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionSearchFunctionLeave(){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionSearchFilterEnter(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionSearchFilterLeave(){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionSearchCatcherFound(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionOSHandlerEnter(UINT_PTR){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionOSHandlerLeave(UINT_PTR){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionUnwindFunctionEnter(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionUnwindFunctionLeave(){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionUnwindFinallyEnter(FunctionID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionUnwindFinallyLeave(){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionCatcherEnter(FunctionID,ObjectID){return S_OK;}
    HRESULT STDMETHODCALLTYPE ExceptionCatcherLeave(){return S_OK;}
    HRESULT STDMETHODCALLTYPE COMClassicVTableCreated(ClassID,REFGUID,void*,ULONG){return S_OK;}
    HRESULT STDMETHODCALLTYPE COMClassicVTableDestroyed(ClassID,REFGUID,void*){return S_OK;}
    HRESULT STDMETHODCALLTYPE FinalizeableObjectQueued(DWORD,ObjectID){return S_OK;}
    HRESULT STDMETHODCALLTYPE RootReferences2(ULONG,ObjectID[],COR_PRF_GC_ROOT_KIND[],COR_PRF_GC_ROOT_FLAGS[],UINT_PTR[]){return S_OK;}
    HRESULT STDMETHODCALLTYPE HandleCreated(GCHandleID,ObjectID){return S_OK;}
    HRESULT STDMETHODCALLTYPE HandleDestroyed(GCHandleID){return S_OK;}
};

class NopFactory : public IClassFactory {
    std::atomic<ULONG> _ref{1};
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) override {
        if (!ppv) return E_POINTER;
        if (riid == __uuidof(IUnknown) || riid == __uuidof(IClassFactory)) { *ppv=this; AddRef(); return S_OK; }
        *ppv=nullptr; return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return ++_ref; }
    ULONG STDMETHODCALLTYPE Release() override { ULONG v=--_ref; if(!v) delete this; return v; }
    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv) override {
        if (pUnkOuter) return CLASS_E_NOAGGREGATION;
        NopProfiler* p = new NopProfiler();
        return p->QueryInterface(riid, ppv);
    }
    HRESULT STDMETHODCALLTYPE LockServer(BOOL){ return S_OK; }
};

extern "C" HRESULT STDAPICALLTYPE DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {
    if (rclsid != CLSID_Nop) return CLASS_E_CLASSNOTAVAILABLE;
    NopFactory* f = new NopFactory();
    return f->QueryInterface(riid, ppv);
}

extern "C" HRESULT STDAPICALLTYPE DllCanUnloadNow() { return S_FALSE; }
