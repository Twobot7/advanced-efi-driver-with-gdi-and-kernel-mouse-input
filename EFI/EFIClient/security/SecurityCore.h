#pragma once
#include <Windows.h>
#include <bcrypt.h>
#include <tbs.h>
#include <intrin.h>
#include <winhvplatform.h>
#include <vector>
#include <array>
#include <memory>

namespace SecureCore {
    class HardwareSecurityProvider {
    private:
        TBS_HCONTEXT tpmContext;
        std::array<uint8_t, 32> entropyPool;
        BCRYPT_ALG_HANDLE hRNG;

    public:
        bool InitializeTPM();
        bool GetHardwareEntropy(std::vector<uint8_t>& entropy);
        bool VerifySecureBootState();
        bool CheckHypervisorPresence();
    };
} 