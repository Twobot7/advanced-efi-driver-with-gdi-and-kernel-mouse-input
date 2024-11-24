#pragma once
#include <Windows.h>
#include <cstdint>

class MemoryProtector {
public:
    static bool ValidateMemoryAccess(void* address, size_t size) {
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
            return false;
        }
        return (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE));
    }

    static bool ValidateMemoryRange(void* address, size_t size) {
        return ValidateMemoryAccess(address, size);
    }
};

#define VALIDATE_MEMORY_RANGE(addr, size) MemoryProtector::ValidateMemoryRange(addr, size) 