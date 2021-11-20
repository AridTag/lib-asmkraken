#include "../../include/asm/MidFunctionHook.h"
#include "../../include/asm/asm.h"

namespace asmkraken::assembly {

#ifdef x86

    static const uint8_t MidFunctionHookTemplate[] = {
            0xE9, 0x00, 0x00, 0x00, 0x00, // jmp pFunc
    };

#endif
#ifdef x64

    static const uint8_t MidFunctionHookTemplate[] = {
            0x50,                                                       // push rax        ; to preserve the register in case it's necessary
            0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, pFunc
            0xFF, 0xE0,                                                 // jmp rax
            0x58                                                        // pop rax         ; restore the rax register
    };

#endif

    constexpr size_t MIN_MIDF_HOOK_SIZE = sizeof(MidFunctionHookTemplate);

    std::optional<MidFunctionHook> CreateMidFunctionHook(void* pDest, void* pFunc, size_t hookSize) {
        if (hookSize < MIN_MIDF_HOOK_SIZE) {
            return std::nullopt;
        }

        // 1. Ensure we can write to the memory location
        // 2. Write NOPs of length hookSize at pDest
        // 3. x86) Calculate relative offset to pFunc. x64) Use absolute address
        // 4. jmp to pFunc
        // 5. Restore memory protection
        // 6. Put the address that should be jumped back to in pJmpBackAddr

        auto* instructions = new uint8_t[hookSize];
        memset(instructions, 0x90, hookSize);
        memcpy(instructions, MidFunctionHookTemplate, MIN_MIDF_HOOK_SIZE);

#ifdef x86
        *(int32_t*) (instructions + 1) = ((intptr_t) pFunc - (intptr_t) pDest) - MIN_MIDF_HOOK_SIZE;
        intptr_t jmpBackAddr = ((intptr_t) pDest + MIN_MIDF_HOOK_SIZE);
#endif

#ifdef x64
        *(int64_t*) (instructions + 3) = (intptr_t) pFunc + hookSize;
        intptr_t jmpBackAddr = ((intptr_t) pDest + MIN_MIDF_HOOK_SIZE - 1); // -1 because we want to execute the last instruction of our detour (pop rax)
#endif

        auto patchBytes = PatchPtr(instructions);
        auto patch = Patch(mem::Pointer((intptr_t) pDest), std::move(patchBytes), hookSize);
        return MidFunctionHook{
                .HookPatch = std::move(patch),
                .JmpBackAddress = jmpBackAddr
        };
    }
}