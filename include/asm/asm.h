#ifndef LIB_ASMKRAKEN_ASM_H
#define LIB_ASMKRAKEN_ASM_H

#include <cstdint>
#include <memory>

namespace asmkraken::assembly {
    std::unique_ptr<uint8_t[]> PatchAsm(void *pDest, const void *pSrc, size_t size);
    std::unique_ptr<uint8_t[]> Nop(void *pDest, size_t size);

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

    static const uint8_t JmpTemplate[] = {
            0xE9, 0x00, 0x00, 0x00, 0x00, // jmp addr
    };

#ifdef x86

    static const uint8_t RelayFunctionPrologue[] = {
            0x50,                                                       // push eax
            0x51,                                                       // push ecx
            0x52,                                                       // push edx
            0x53,                                                       // push ebx
            0x55,                                                       // push ebp
            0x56,                                                       // push esi
            0x57,                                                       // push edi
            0x89, 0xE9,                                                 // move ecx, ebp   ; Allows the stack to be de-referenced as the Registers struct
    };

    constexpr ptrdiff_t RelayFunctionTargetAddrOffset = 1;
    static const uint8_t RelayFunctionTemplate[] = {
            0xBF, 0x00, 0x00, 0x00, 0x00,                               // mov edi hookFuncAddr
            0xFF, 0xD7                                                  // call edi
    };

    static const uint8_t RelayFunctionEpilogue[] = {
            0x5F,                                                       // pop rdi
            0x5E,                                                       // pop rsi
            0x5D,                                                       // pop rbp
            0x5B,                                                       // pop rbx
            0x5A,                                                       // pop rdx
            0x59,                                                       // pop rcx
            0x58                                                        // pop rax
    };
#endif
#ifdef x64
    static const uint8_t RelayFunctionPrologue[]{
            0x50,                                                       // push rax
            0x51,                                                       // push rcx
            0x52,                                                       // push rdx
            0x53,                                                       // push rbx
            0x55,                                                       // push rbp
            0x56,                                                       // push rsi
            0x57,                                                       // push rdi
            0x41, 0x50,                                                 // push r8
            0x41, 0x51,                                                 // push r9
            0x41, 0x52,                                                 // push r10
            0x41, 0x53,                                                 // push r11
            0x41, 0x54,                                                 // push r12
            0x41, 0x55,                                                 // push r13
            0x41, 0x56,                                                 // push r14
            0x41, 0x57,                                                 // push r15
            0x48, 0x89, 0xE1                                            // mov rcx, rsp              ; Allows the stack to be de-referenced as the Registers struct
    };

    constexpr ptrdiff_t RelayFunctionTargetAddrOffset = 2;
    static const uint8_t RelayFunctionTemplate[] = {
            0x48, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs rdi, hookFuncAddr
            0xFF, 0xD7,                                                 // call rdi
    };

    static const uint8_t RelayFunctionEpilogue[]{
            0x41, 0x5F,                                                 // pop r15
            0x41, 0x5E,                                                 // pop r14
            0x41, 0x5D,                                                 // pop r13
            0x41, 0x5C,                                                 // pop r12
            0x41, 0x5B,                                                 // pop r11
            0x41, 0x5A,                                                 // pop r10
            0x41, 0x59,                                                 // pop r9
            0x41, 0x58,                                                 // pop r8
            0x5F,                                                       // pop rdi
            0x5E,                                                       // pop rsi
            0x5D,                                                       // pop rbp
            0x5B,                                                       // pop rbx
            0x5A,                                                       // pop rdx
            0x59,                                                       // pop rcx
            0x58,                                                       // pop rax
    };

#endif

    constexpr size_t JmpTemplateSize = sizeof(JmpTemplate);
    constexpr size_t RelayFunctionPrologueSize = sizeof(RelayFunctionPrologue);
    constexpr size_t RelayFunctionContentSize = sizeof(RelayFunctionTemplate);
    constexpr size_t RelayFunctionEpilogueSize = sizeof(RelayFunctionEpilogue);

    void WriteRelayFunction(void* pDest, intptr_t hookTargetAddr, intptr_t hookFuncAddr, uint8_t* additionalInstructions, size_t additionalInstructionsSize);
}

#endif //LIB_ASMKRAKEN_ASM_H
