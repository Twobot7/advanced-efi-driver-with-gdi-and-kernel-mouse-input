#include "Driver.h"
#include <iostream>

namespace Driver {
    MemoryLayout currentMemoryLayout = {0};
    SpinLock driverLock;

    bool initialize() {
        // Check if driver is loaded by trying to read a test value
        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        NTSTATUS status;
        
        // Create encrypted test command
        struct {
            int magic;
            int operation;
            DWORD64 data[10];
        } cmd;

        cmd.magic = COMMAND_MAGIC;
        cmd.operation = COMMAND_MAGIC * 0x612; // Init operation
        
        // Encrypt command
        BYTE* rawData = (BYTE*)&cmd;
        for (size_t i = 0; i < sizeof(cmd); i++) {
            rawData[i] ^= ((BYTE*)&ENCRYPTION_KEY)[i % sizeof(ENCRYPTION_KEY)];
        }

        // Try to communicate with driver
        status = NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            &cmd,
            sizeof(cmd),
            ATTRIBUTE_NON_VOLATILE
        );

        return NT_SUCCESS(status);
    }

    bool ValidateCommand(const std::string& cmd) {
        SpinLock::lock_guard guard(driverLock);
        return !cmd.empty();
    }

    bool ValidateMemoryAccess(void* address, size_t size) {
        SpinLock::lock_guard guard(driverLock);
        return address != nullptr && size > 0;
    }

    bool read_memory(int pid, uintptr_t address, uintptr_t buffer, size_t size) {
        if (!ValidateMemoryAccess((void*)address, size)) {
            return false;
        }

        struct {
            int magic;
            int operation;
            DWORD64 data[10];
        } cmd;

        cmd.magic = COMMAND_MAGIC;
        cmd.operation = COMMAND_MAGIC * 0x823; // Read operation
        cmd.data[0] = pid;
        cmd.data[1] = address;
        cmd.data[2] = GetCurrentProcessId();
        cmd.data[3] = buffer;
        cmd.data[4] = size;

        // Encrypt command
        BYTE* rawData = (BYTE*)&cmd;
        for (size_t i = 0; i < sizeof(cmd); i++) {
            rawData[i] ^= ((BYTE*)&ENCRYPTION_KEY)[i % sizeof(ENCRYPTION_KEY)];
        }

        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        return NT_SUCCESS(NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            &cmd,
            sizeof(cmd),
            ATTRIBUTE_NON_VOLATILE
        ));
    }

    uintptr_t GetBaseAddress(int pid) {
        struct {
            int magic;
            int operation; 
            DWORD64 data[10];
        } cmd;

        uintptr_t result = 0;

        cmd.magic = COMMAND_MAGIC;
        cmd.operation = COMMAND_MAGIC * 0x913; // Get base operation
        cmd.data[0] = pid;
        cmd.data[1] = (DWORD64)&result;

        // Encrypt command
        BYTE* rawData = (BYTE*)&cmd;
        for (size_t i = 0; i < sizeof(cmd); i++) {
            rawData[i] ^= ((BYTE*)&ENCRYPTION_KEY)[i % sizeof(ENCRYPTION_KEY)];
        }

        UNICODE_STRING VariableName = RTL_CONSTANT_STRING(VARIABLE_NAME);
        if (NT_SUCCESS(NtSetSystemEnvironmentValueEx(
            &VariableName,
            &DummyGuid,
            &cmd,
            sizeof(cmd),
            ATTRIBUTE_NON_VOLATILE
        ))) {
            return result;
        }
        return 0;
    }
}
