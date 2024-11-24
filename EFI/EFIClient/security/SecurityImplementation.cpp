#include "SecurityCore.h"

bool AdvancedCryptoManager::InitializeSecureEnclave() {
    sgx_attributes_t attributes = {0};
    sgx_misc_attribute_t misc = {0};
    
    attributes.flags = SGX_FLAGS_MODE64BIT;
    attributes.xfrm = SGX_XFRM_LEGACY;

    sgx_status_t ret = sgx_create_enclave(
        "SecureEnclave.signed.dll",
        SGX_DEBUG_FALSE,
        &enclave.token,
        &enclave.tokenUpdated,
        &enclave.enclaveId,
        &misc);

    if (ret != SGX_SUCCESS) return false;

    // Initialize hardware-backed encryption
    NTSTATUS status = BCryptOpenAlgorithmProvider(
        &cryptoContext.hAlg,
        BCRYPT_CHACHA20_POLY1305_ALGORITHM,
        MS_PRIMITIVE_PROVIDER,
        BCRYPT_ALG_HANDLE_HMAC_FLAG);

    return BCRYPT_SUCCESS(status);
} 