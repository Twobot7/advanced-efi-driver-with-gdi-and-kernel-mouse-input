# EFI Memory Driver

A UEFI-based driver for direct memory access and process manipulation, with built-in security features and stealth capabilities.

## Features

- Direct memory read/write capabilities through UEFI
- Process base address detection
- Encrypted command communication
- Anti-detection mechanisms
- Hypervisor-based security protections
- Stealth driver cloaking
- Mouse input handling support
- Secure memory access validation
- Administrator privilege enforcement
- Overlay System
  - Real-time transparent overlay rendering
  - Multiple blend modes (Normal, Additive, Multiply, ColorKey)
  - FPS limiting and frame timing control
  - Window attachment and automatic positioning
  - Transparency control
  - Logging system for debugging
- Input System
  - High-precision mouse input (1000Hz polling)
  - Full keyboard state tracking
  - Input event buffering
  - Thread-safe input processing
  - Security validation for input operations
  - Raw input device access

## Prerequisites

- Visual Studio 2019 or later
- Windows SDK
- UEFI development environment
- Intel SGX SDK (for security features)
- Administrator privileges

## Building

1. Open `EFI_Driver_Access.sln` in Visual Studio
2. Set build configuration to `Release|x64`
3. Build the solution

Note: The project requires the following dependencies:
- sgx.h (Intel SGX SDK)
- Windows SDK headers
- GNU-EFI headers

## Usage

### Driver Installation

1. Disable Secure Boot in UEFI settings
2. Boot into UEFI shell
3. Load the compiled `memory.efi` driver:
```shell
fs0:
cd EFI
load memory.efi
```

### Client Application

The client application (`EFIClient.exe`) provides the following functionality:

1. Get process base address by PID
2. Read process memory by PID
3. Exit

To use:
1. Run as Administrator
2. Select desired operation from menu
3. Input required parameters (PID, address, size)

### Overlay System Implementation

1. **Basic Setup**

## Security Features

The driver implements multiple security layers:

- Command encryption using XOR cipher
- Magic number validation
- Call count limiting
- Hypervisor-based protection
- Memory region cloaking
- Anti-debugging mechanisms
- System call hooks

## Security Implementation

### Core Security Features

1. **Hypervisor Protection**
- VTL (Virtual Trust Level) initialization
- SGX enclave creation
- Processor-based security controls

2. **Memory Protection**
- Secure memory regions with encryption
- Control Flow Guard implementation
- XFG (eXtended Flow Guard) protection
- Encrypted page regions

3. **Driver Cloaking**
- PEB unlinking
- System query hooking
- Driver object concealment
- Memory region disguise

### Advanced Security Mechanisms

1. **Memory Disguise System**
```cpp
// Example implementation
MemoryDisguise memDisguise;
if (memDisguise.DisguiseMemoryRegions()) {
    memDisguise.ModifyMemoryAttributes();
    memDisguise.CreateFakeDriverEntry();
    memDisguise.HideMemoryPages();
}
```

2. **Thread Security**
- Virtualization-based thread protection
- Security policy enforcement
- CET (Control-flow Enforcement Technology)
- Thread integrity monitoring
```cpp:EFIClient/security/ThreadManager.h
startLine: 6
endLine: 23
```

3. **System Call Protection**
- Hook installation for system queries
- Virtual memory query protection
- System call table modification
- Hook protection mechanisms
```cpp:EFIClient/security/SystemHooks.h
startLine: 4
endLine: 15
```

### Security Implementation Guide

1. **Initialize Core Security**
```cpp
StealthDriver security;
if (security.InitializeStealth()) {
    security.ConcealDriver();
    security.ProtectMemoryRegions();
    security.MonitorSystemCalls();
}
```

2. **Memory Protection Setup**
```cpp
AdvancedMemoryProtection memProtection;
memProtection.ProtectMemoryWithVirtualization();
memProtection.EncryptMemoryRegions();
memProtection.CreateMemoryTraps();
memProtection.SetupMemoryHooks();
```

3. **System Call Hooks**
```cpp
SystemCallHook sysHooks;
sysHooks.HookNtQuerySystemInformation();
sysHooks.HookNtQueryVirtualMemory();
sysHooks.ModifySystemCallTable();
```

### Security Validation

1. **Input Validation**
```cpp:EFIClient/Input/SecurityValidator.h
startLine: 4
endLine: 15
```

2. **Memory Integrity**
- Continuous memory validation
- Anti-tampering checks
- Process context verification
```cpp:EFIClient/security/SecurityImplementation.cpp
startLine: 3
endLine: 28
```

### Security Best Practices

1. **Initialization**
- Always initialize security features before any other operations
- Verify successful initialization of all security components
- Implement fallback mechanisms for failed security initializations

2. **Runtime Protection**
- Continuously monitor system integrity
- Implement real-time threat detection
- Use hardware-backed security features when available

3. **Memory Safety**
- Encrypt sensitive memory regions
- Implement memory access validation
- Use secure memory allocation practices

4. **Error Handling**
- Implement secure error logging
- Avoid exposing sensitive information in error messages
- Handle security failures gracefully

### Security Considerations

1. **Anti-Debug Protection**
- Implement timing checks
- Detect debugging attempts
- Monitor execution environment

2. **Anti-Cheat Measures**
- Validate process integrity
- Monitor memory modifications
- Detect injection attempts

3. **System Integrity**
- Verify system call integrity
- Monitor critical system structures
- Implement secure boot validation

## Warning

This driver operates at a very low level and can potentially cause system instability if misused. Use with caution and only on test systems.


## Credits

- Original Driver: Samuel Tulach
- Direct Calling Implementation: The CruZ
- Thanks to @Mattiwatti (EfiGuard)
- Thanks to Roderick W. Smith (rodsbooks.com)
