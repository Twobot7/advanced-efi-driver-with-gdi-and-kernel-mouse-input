#pragma once
#include "SecurityCore.h"
#include <Windows.h>
#include <bcrypt.h>

class AdvancedCryptoManager {
private:
    struct SecureContext {
        BCRYPT_ALG_HANDLE hAlg;
        BCRYPT_KEY_HANDLE hKey;
        std::vector<uint8_t> keyMaterial;
    };

    SecureContext secureCtx;
    HardwareSecurityProvider hwSecurity;
    
    // Advanced encryption contexts
    struct {
        BCRYPT_ALG_HANDLE hAlg;
        BCRYPT_KEY_HANDLE hKey;
        BCRYPT_SECRET_HANDLE hSecret;
        std::vector<uint8_t> keyMaterial;
    } cryptoContext;

public:
    bool InitializeSecureContext();
    bool PerformSecureOperation(const std::vector<uint8_t>& input, 
                              std::vector<uint8_t>& output,
                              bool (*operation)(void*));
}; 