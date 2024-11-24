#include "MouseDriver.h"
#include <ntddmou.h>

namespace Driver::Input {
    bool MouseDriver::Initialize(DWORD pollRate) {
        UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\PointerClass0");
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

        if (!NT_SUCCESS(status)) {
            return false;
        }

        // Set up mouse attributes
        MOUSE_ATTRIBUTES mouseAttrib = {0};
        mouseAttrib.MouseIdentifier = MOUSE_ABSOLUTE_POINTER;
        mouseAttrib.NumberOfButtons = 5;  // Support for 5 buttons
        mouseAttrib.SampleRate = pollRate;

        return NT_SUCCESS(ZwDeviceIoControlFile(deviceHandle,
            NULL, NULL, NULL, &ioStatus,
            IOCTL_MOUSE_SET_ATTRIBUTES,
            &mouseAttrib, sizeof(mouseAttrib),
            NULL, 0));
    }
} 