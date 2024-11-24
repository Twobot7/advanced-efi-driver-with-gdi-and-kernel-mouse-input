#pragma once
#include <Windows.h>
#include <winternl.h>

class SystemCallHook {
private:
    struct HookContext {
        PVOID originalFunction;
        PVOID hookFunction;
        BYTE originalBytes[16];
        DWORD protection;
    };

    std::vector<HookContext> hooks;
    
public:
    bool HookNtQuerySystemInformation();
    bool HookNtQueryVirtualMemory();
    bool HookZwQuerySystemInformation();
    bool ModifySystemCallTable();
}; 