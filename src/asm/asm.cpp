#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winnt.h>
#include <minwindef.h>

#include "asm.h"

namespace asmkraken::__identifier(asm) {
    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> Patch(void *pDest, const void *pSrc, size_t size) {
        auto* pOriginalBytes = new uint8_t[size];
        DWORD oldProtection;
        if (VirtualProtect(pDest, size, PAGE_EXECUTE_READWRITE, &oldProtection) == FALSE)
        {
            return nullptr;
        }

        memcpy(pOriginalBytes, pDest, size);
        memcpy(pDest, pSrc, size);

        VirtualProtect(pDest, size, oldProtection, &oldProtection);

        return std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>>(pOriginalBytes);
    }

    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> Nop(void *pDest, size_t size) {
        auto* nopArray = new uint8_t[size];
        memset(nopArray, 0x90, size);
        auto originalBytes = Patch(pDest, nopArray, size);
        delete[] nopArray;

        return originalBytes;
    }
}
