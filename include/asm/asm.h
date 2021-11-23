#ifndef LIB_ASMKRAKEN_ASM_H
#define LIB_ASMKRAKEN_ASM_H

#include <cstdint>
#include <memory>

namespace asmkraken::assembly {
    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> PatchAsm(void *pDest, const void *pSrc, size_t size);
    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> Nop(void *pDest, size_t size);

#ifdef x86
    struct Registers {
        uint32_t ESI;
        uint32_t EBP;
        uint32_t EBX;
        uint32_t EDX;
        uint32_t ECX;
        uint32_t EAX;
    };
#endif

#ifdef x64
    struct Registers {
        uint64_t R15;
        uint64_t R14;
        uint64_t R13;
        uint64_t R12;
        uint64_t R11;
        uint64_t R10;
        uint64_t R9;
        uint64_t R8;
        uint64_t RDI;
        uint64_t RSI;
        uint64_t RBP;
        uint64_t RBX;
        uint64_t RDX;
        uint64_t RCX;
        uint64_t RAX;
    };
#endif

}

#endif //LIB_ASMKRAKEN_ASM_H
