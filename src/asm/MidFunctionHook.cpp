#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memoryapi.h>
#include "../../include/asm/MidFunctionHook.h"
#include "../../include/mem/mem.h"

namespace asmkraken::assembly {
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
            0x48, 0x89, 0xE9                                            // mov rcx, rbp              ; Allows the stack to be de-referenced as the Registers struct
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
            0x58                                                        // pop rax
    };

#endif

    constexpr size_t JmpTemplateSize = sizeof(JmpTemplate);
    constexpr size_t RelayFunctionPrologueSize = sizeof(RelayFunctionPrologue);
    constexpr size_t RelayFunctionContentSize = sizeof(RelayFunctionTemplate);
    constexpr size_t RelayFunctionEpilogueSize = sizeof(RelayFunctionEpilogue);

    void WriteRelayFunction(void* pDest, intptr_t hookTargetAddr, intptr_t hookFuncAddr, uint8_t* additionalInstructions, size_t additionalInstructionsSize) {
        auto writeAddr = (intptr_t) pDest;

        memcpy((void*) writeAddr, RelayFunctionPrologue, RelayFunctionPrologueSize);
        writeAddr += RelayFunctionPrologueSize;

        memcpy((void*) writeAddr, RelayFunctionTemplate, RelayFunctionContentSize);
        *(intptr_t*) (writeAddr + RelayFunctionTargetAddrOffset) = hookFuncAddr;
        writeAddr += RelayFunctionContentSize;

        memcpy((void*) writeAddr, RelayFunctionEpilogue, RelayFunctionEpilogueSize);
        writeAddr += RelayFunctionEpilogueSize;

        if (additionalInstructions != nullptr) {
            memcpy((void*) writeAddr, additionalInstructions, additionalInstructionsSize);
            writeAddr += (intptr_t) additionalInstructionsSize;
        }

        memcpy((void*) writeAddr, JmpTemplate, JmpTemplateSize);
        auto offsetToRelayFunc = (int32_t) abs(writeAddr - hookTargetAddr);
        *(int32_t*) (writeAddr + 1) = -offsetToRelayFunc;
    }

    MidFunctionHook::MidFunctionHook() :
            hookPatch(),
            allocatedPage(nullptr),
            pageSize(0) {
    }

    MidFunctionHook::MidFunctionHook(Patch& patch, void* allocatedPage, int32_t pageSize) :
            hookPatch(std::move(patch)),
            allocatedPage(allocatedPage),
            pageSize(pageSize) {
    }

    MidFunctionHook::~MidFunctionHook() {
        if (allocatedPage != nullptr) {
            VirtualFree(allocatedPage, pageSize, MEM_FREE);
            allocatedPage = nullptr;
        }
    }

    bool MidFunctionHook::IsEnabled() {
        return hookPatch.IsEnabled();
    }

    void MidFunctionHook::Toggle() {
        if (IsEnabled()) {
            Disable();
        } else {
            Enable();
        }
    }

    void MidFunctionHook::Enable() {
        if (IsEnabled()) {
            return;
        }

        hookPatch.Enable();
    }

    void MidFunctionHook::Disable() {
        if (!IsEnabled()) {
            return;
        }

        hookPatch.Disable();
    }

    template<typename T>
    std::optional<MidFunctionHook> MidFunctionHook::TryCreate(void* pDest, std::function<void(Registers)> hookFunc, size_t hookSize, size_t replacedInstructionsStartOffset) {
        if (hookSize < JmpTemplateSize) {
            return std::nullopt;
        }

        int32_t relayFuncMemSize = 0;
        auto* relayFuncMem = mem::AllocatePageNearAddress((uintptr_t) pDest, &relayFuncMemSize);
        if (relayFuncMem == nullptr) {
            return std::nullopt;
        }

        auto offsetToRelayFunc = (intptr_t) relayFuncMem - (intptr_t) pDest;
        if (offsetToRelayFunc < INT32_MIN || offsetToRelayFunc > INT32_MAX) {
            VirtualFree(relayFuncMem, 0, MEM_RELEASE);
            return std::nullopt;
        }

        auto hookFuncAddr = reinterpret_cast<intptr_t>(*hookFunc.target<T*>());

        WriteRelayFunction(relayFuncMem, (intptr_t) pDest, hookFuncAddr, (uint8_t*) ((uintptr_t) pDest + replacedInstructionsStartOffset), hookSize - replacedInstructionsStartOffset);

        auto* replacementBytes = new uint8_t[hookSize];
        memset(replacementBytes, 0x90, hookSize);
        memcpy(replacementBytes, JmpTemplate, JmpTemplateSize);

        auto patchBytes = PatchPtr(replacementBytes);
        auto patch = Patch(mem::Pointer((intptr_t) pDest), std::move(patchBytes), hookSize);
        auto opt = std::optional<MidFunctionHook>(MidFunctionHook(patch, relayFuncMem, relayFuncMemSize));
        return opt;
    }
}