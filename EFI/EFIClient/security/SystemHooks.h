#pragma once
#include <Windows.h>

class SystemHooks {
public:
    bool InstallHooks();
    bool HookNtQuerySystemInformation();
    bool HookNtQueryVirtualMemory();
    bool HookZwQuerySystemInformation();

private:
    bool ModifySystemCallTable();
    bool CreateHookedFunction(PVOID original, PVOID hook);
    bool ProtectHooks();
}; 