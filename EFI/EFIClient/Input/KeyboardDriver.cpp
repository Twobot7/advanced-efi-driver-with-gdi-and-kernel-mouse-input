#include "KeyboardDriver.h"
#include <ntddkbd.h>

namespace Driver::Input {
    bool KeyboardDriver::Initialize(DWORD pollRate) {
        UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\KeyboardClass0");
        OBJECT_ATTRIBUTES objAttr;
        IO_STATUS_BLOCK ioStatus;

        InitializeObjectAttributes(&objAttr, &deviceName, 
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

        NTSTATUS status = ZwCreateFile(&deviceHandle,
            GENERIC_READ | GENERIC_WRITE,
            &objAttr, &ioStatus, NULL,
            FILE_ATTRIBUTE_NORMAL,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            FILE_OPEN,
            FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
            NULL, 0);

        return NT_SUCCESS(status);
    }
} 