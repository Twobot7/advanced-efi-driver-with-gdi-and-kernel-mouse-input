#pragma once
#include <Windows.h>
#include <memory>
#include <array>

class AdvancedMemoryProtection {
private:
    struct ProtectedRegion {
        PVOID baseAddress;
        SIZE_T size;
        DWORD protection;
        bool isEncrypted;
        std::array<uint8_t, 32> keyMaterial;
    };

    std::vector<ProtectedRegion> regions;
    
public:
    bool ProtectMemoryWithVirtualization();
    bool EncryptMemoryRegions();
    bool CreateMemoryTraps();
    bool SetupMemoryHooks();
}; 