#ifndef LIB_ASMKRAKEN_ASM_H
#define LIB_ASMKRAKEN_ASM_H

#include <cstdint>
#include <memory>

namespace asmkraken::assembly {
    std::unique_ptr<uint8_t[]> PatchAsm(void *pDest, const void *pSrc, size_t size);
    std::unique_ptr<uint8_t[]> Nop(void *pDest, size_t size);

#ifdef ASMKRAKEN_x86
    struct Registers {
        uint32_t ESI;
        uint32_t EBP;
        uint32_t EBX;
        uint32_t EDX;
        uint32_t ECX;
        uint32_t EAX;
    };
#endif

#ifdef ASMKRAKEN_x64
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

#ifdef ASMKRAKEN_x86

    static const uint8_t RelayFunctionPrologue[] = {
            0x50,                                                       // push eax
            0x51,                                                       // push ecx
            0x52,                                                       // push edx
            0x53,                                                       // push ebx
            0x55,                                                       // push ebp
            0x56,                                                       // push esi
            0x57,                                                       // push edi
            0x89, 0xE9,                                                 // mov ecx, ebp   ; Allows the stack to be de-referenced as the Registers struct
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
#ifdef ASMKRAKEN_x64
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
            0x48, 0x89, 0xE5,                                           // mov rbp, rsp     ; move the current stack position to rbp.


            0x48, 0x81, 0xEC, 0x00, 0x02, 0x00, 0x00,                   // sub rsp, 32*16              ; allocate space on the stack
            0xC5, 0xFE, 0x7F, 0x04, 0x24,                               // vmovdqu [rsp + 32*0],ymm0
            0xC5, 0xFE, 0x7F, 0x4C, 0x24, 0x20,                         // vmovdqu [rsp + 32*1],ymm1
            0xC5, 0xFE, 0x7F, 0x54, 0x24, 0x40,                         // vmovdqu [rsp + 32*2],ymm2
            0xC5, 0xFE, 0x7F, 0x5C, 0x24, 0x60,                         // vmovdqu [rsp + 32*3],ymm3
            0xC5, 0xFE, 0x7F, 0xA4, 0x24, 0x80, 0x00, 0x00, 0x00,       // vmovdqu [rsp + 32*4],ymm4
            0xC5, 0xFE, 0x7F, 0xAC, 0x24, 0xA0, 0x00, 0x00, 0x00,       // vmovdqu [rsp + 32*5],ymm5
            0xC5, 0xFE, 0x7F, 0xB4, 0x24, 0xC0, 0x00, 0x00, 0x00,       // vmovdqu [rsp + 32*6],ymm6
            0xC5, 0xFE, 0x7F, 0xBC, 0x24, 0xE0, 0x00, 0x00, 0x00,       // vmovdqu [rsp + 32*7],ymm7
            0xC5, 0x7E, 0x7F, 0x84, 0x24, 0x00, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*8],ymm8
            0xC5, 0x7E, 0x7F, 0x8C, 0x24, 0x20, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*9],ymm9
            0xC5, 0x7E, 0x7F, 0x94, 0x24, 0x40, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*10],ymm10
            0xC5, 0x7E, 0x7F, 0x9C, 0x24, 0x60, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*11],ymm11
            0xC5, 0x7E, 0x7F, 0xA4, 0x24, 0x80, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*12],ymm12
            0xC5, 0x7E, 0x7F, 0xAC, 0x24, 0xA0, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*13],ymm13
            0xC5, 0x7E, 0x7F, 0xB4, 0x24, 0xC0, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*14],ymm14
            0xC5, 0x7E, 0x7F, 0xBC, 0x24, 0xE0, 0x01, 0x00, 0x00,       // vmovdqu [rsp + 32*15],ymm15


            0x48, 0x89, 0xE9                                            // mov rcx, rbp              ; Grabs the stack position we saved earlier. Allows the stack to be de-referenced as the Registers struct
    };

    constexpr ptrdiff_t RelayFunctionTargetAddrOffset = 2;
    static const uint8_t RelayFunctionTemplate[] = {
            0x48, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs rdi, hookFuncAddr
            0xFF, 0xD7,                                                 // call rdi
    };

    static const uint8_t RelayFunctionEpilogue[]{
            0xC5, 0x7E, 0x6F, 0xBC, 0x24, 0xE0, 0x01, 0x00, 0x00,       // vmovdqu ymm15,[rsp + 32*15]
            0xC5, 0x7E, 0x6F, 0xB4, 0x24, 0xC0, 0x01, 0x00, 0x00,       // vmovdqu ymm14,[rsp + 32*14]
            0xC5, 0x7E, 0x6F, 0xAC, 0x24, 0xA0, 0x01, 0x00, 0x00,       // vmovdqu ymm13,[rsp + 32*13]
            0xC5, 0x7E, 0x6F, 0xA4, 0x24, 0x80, 0x01, 0x00, 0x00,       // vmovdqu ymm12,[rsp + 32*12]
            0xC5, 0x7E, 0x6F, 0x9C, 0x24, 0x60, 0x01, 0x00, 0x00,       // vmovdqu ymm11,[rsp + 32*11]
            0xC5, 0x7E, 0x6F, 0x94, 0x24, 0x40, 0x01, 0x00, 0x00,       // vmovdqu ymm10,[rsp + 32*10]
            0xC5, 0x7E, 0x6F, 0x8C, 0x24, 0x20, 0x01, 0x00, 0x00,       // vmovdqu ymm9, [rsp + 32*9]
            0xC5, 0x7E, 0x6F, 0x84, 0x24, 0x00, 0x01, 0x00, 0x00,       // vmovdqu ymm8, [rsp + 32*8]
            0xC5, 0xFE, 0x6F, 0xBC, 0x24, 0xE0, 0x00, 0x00, 0x00,       // vmovdqu ymm7, [rsp + 32*7]
            0xC5, 0xFE, 0x6F, 0xB4, 0x24, 0xC0, 0x00, 0x00, 0x00,       // vmovdqu ymm6, [rsp + 32*6]
            0xC5, 0xFE, 0x6F, 0xAC, 0x24, 0xA0, 0x00, 0x00, 0x00,       // vmovdqu ymm5, [rsp + 32*5]
            0xC5, 0xFE, 0x6F, 0xA4, 0x24, 0x80, 0x00, 0x00, 0x00,       // vmovdqu ymm4, [rsp + 32*4]
            0xC5, 0xFE, 0x6F, 0x5C, 0x24, 0x60,                         // vmovdqu ymm3, [rsp + 32*3]
            0xC5, 0xFE, 0x6F, 0x54, 0x24, 0x40,                         // vmovdqu ymm2, [rsp + 32*2]
            0xC5, 0xFE, 0x6F, 0x4C, 0x24, 0x20,                         // vmovdqu ymm1, [rsp + 32*1]
            0xC5, 0xFE, 0x6F, 0x04, 0x24,                               // vmovdqu ymm0, [rsp + 32*0]
            0x48, 0x81, 0xC4, 0x00, 0x02, 0x00, 0x00,                   // add rsp, 32*16               ; Forget about the ymm values stored and move to the general purpose regs

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

    void SuspendOtherThreads(bool suspend);
}

#endif //LIB_ASMKRAKEN_ASM_H
