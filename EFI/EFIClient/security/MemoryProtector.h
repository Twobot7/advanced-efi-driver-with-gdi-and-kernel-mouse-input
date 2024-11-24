#pragma once
#include <Windows.h>
#include <memoryapi.h>
#include <enclaveapi.h>

class AdvancedMemoryProtector {
private:
    struct SecureRegion {
        PVOID baseAddress;
        SIZE_T size;
        HANDLE sectionHandle;
        BYTE entropy[32];
    };

    std::vector<SecureRegion> protectedRegions;
    
public:
    bool CreateSecureMemoryRegion(SIZE_T size, SecureRegion& region);
    bool ProtectWithVirtualization(void* address, size_t size);
    bool EnableControlFlowGuard(void* function);
    bool ApplyXFGProtection(void* address);
    bool CreateEncryptedPageRegion(void* address, size_t size);
}; 