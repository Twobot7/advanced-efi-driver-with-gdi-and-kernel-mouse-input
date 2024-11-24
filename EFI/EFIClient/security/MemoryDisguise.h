#pragma once
#include <Windows.h>

class MemoryDisguise {
public:
    bool DisguiseMemoryRegions();
    bool ModifyMemoryAttributes();
    bool CreateFakeDriverEntry();
    bool HideMemoryPages();

private:
    bool ModifyMemoryProtection(PVOID address, SIZE_T size);
    bool CreateDecoyStructures();
    bool ScrambleMemorySignature();
}; 