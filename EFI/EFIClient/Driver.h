#pragma once
#include <Windows.h>
#include <string>
#include "SpinLock.h"
#include "ThreadManager.h"
#include "security/CryptoManager.h"
#include "MemoryProtector.h"
#include "overlay/OverlaySystem.h"

// Constants for driver communication
#define VARIABLE_NAME L"VirtualAddressMap64"
#define COMMAND_MAGIC 0x81726354
#define ENCRYPTION_KEY 0x9A3B5C7D

// GUID must match the one in EFI driver
static const GUID DummyGuid = 
    { 0x2f84893e, 0xfd5e, 0x2038, {0x8d, 0x9e, 0x20, 0xa7, 0xaf, 0x9c, 0x32, 0xf1} };

struct MemoryLayout {
    DWORD64 processId;
    DWORD64 baseAddress;
    SIZE_T regionSize;
};

namespace Driver {
    extern MemoryLayout currentMemoryLayout;
    extern SpinLock driverLock;  // Global lock for driver operations
    
    bool initialize();
    bool ValidateCommand(const std::string& cmd);
    bool ValidateMemoryAccess(void* address, size_t size);
    uintptr_t GetBaseAddress(int pid);
    
    template<typename T>
    T read(int pid, uintptr_t address, NTSTATUS* status = nullptr) {
        T buffer;
        read_memory(pid, address, (uintptr_t)&buffer, sizeof(T));
        return buffer;
    }

    bool read_memory(int pid, uintptr_t address, uintptr_t buffer, size_t size);
    
    // Add other necessary declarations here
}
