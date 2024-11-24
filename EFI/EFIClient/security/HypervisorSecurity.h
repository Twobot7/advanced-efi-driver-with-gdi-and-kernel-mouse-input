#pragma once
#include <Windows.h>
#include <intrin.h>
#include <HvLoader.h>
#include <vtl.h>
#include <sgx_urts.h>
#include <sgx_capable.h>

class HypervisorSecurity {
private:
    struct SecureVTL {
        HANDLE vtlHandle;
        PVOID secureRegion;
        SIZE_T regionSize;
        DWORD vtlLevel;
    };

    struct EnclaveContext {
        sgx_enclave_id_t enclaveId;
        sgx_launch_token_t token;
        sgx_misc_attribute_t attributes;
        void* enclaveBase;
    };

    SecureVTL vtlContext;
    EnclaveContext sgxContext;
    
public:
    bool InitializeVTL();
    bool CreateSecureEnclave();
    bool ProtectWithVirtualization();
    bool EnableSecurityFeatures();
}; 