#include "ThreadManager.h"
#include "../utils/Logger.h"

bool ThreadManager::HideThread(HANDLE thread) {
    SpinLockGuard guard(threadLock);
    
    HiddenThread hiddenThread = {};
    hiddenThread.threadHandle = thread;
    
    // Copy legitimate system thread attributes
    CopySystemThreadAttributes(thread);
    
    // Spoof timing data
    SpoofThreadTiming(thread);
    
    // Unlink from CID table using PsUnlinkThreadCidEntry
    if (!UnlinkThreadFromCidTable(thread, &hiddenThread.originalCid)) {
        Logger::Log("Failed to unlink thread from CID table");
        return false;
    }
    
    hiddenThreads.push_back(hiddenThread);
    return true;
}

void ThreadManager::SpoofThreadTiming(HANDLE thread) {
    LARGE_INTEGER systemTime;
    KeQuerySystemTime(&systemTime);
    
    // Randomize thread timing data
    THREAD_BASIC_INFORMATION threadInfo = {};
    if (NT_SUCCESS(ZwQueryInformationThread(thread, 
                                          ThreadBasicInformation,
                                          &threadInfo,
                                          sizeof(threadInfo),
                                          nullptr))) {
        // Modify timing data with randomized but plausible values
        // Implementation details hidden for brevity
    }
}

bool ThreadManager::UnlinkThreadFromCidTable(HANDLE thread, ULONG_PTR* originalCid) {
    // Use PsGetThreadCidEntry to get current entry
    PVOID cidEntry = PsGetThreadCidEntry(thread);
    if (!cidEntry) {
        return false;
    }
    
    // Store original CID for later restoration
    *originalCid = *(ULONG_PTR*)((PUCHAR)cidEntry + CID_OFFSET);
    
    // Unlink using PsUnlinkThreadCidEntry
    return NT_SUCCESS(PsUnlinkThreadCidEntry(thread));
} 