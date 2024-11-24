#pragma once
#include "Common.h"

namespace Driver::Input {
    class SecurityValidator {
    public:
        static bool ValidateAccess(void* address, size_t size);
        static bool ValidateIOBuffer(PIRP irp);
        static bool ValidateDeviceRequest(PDEVICE_OBJECT DeviceObject, PIRP Irp);
        
        // Anti-cheat related validations
        static bool ValidateProcessContext();
        static bool ValidateMemoryIntegrity();
    };
} 