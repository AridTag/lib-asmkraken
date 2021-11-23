#ifndef LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
#define LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H

#include "../Toggleable.h"
#include "Patch.h"
#include "asm.h"
#include "../mem/mem.h"

#include <optional>
#include <functional>

namespace asmkraken::assembly {
    class MidFunctionHook : Toggleable {
    public:
        MidFunctionHook();
        MidFunctionHook(Patch& patch, std::shared_ptr<void> allocatedPage);
        MidFunctionHook(const MidFunctionHook& m) = default;
        MidFunctionHook(MidFunctionHook&& m) noexcept;

        bool IsEnabled() final;
        void Toggle() final;
        void Enable() final;
        void Disable() final;

        /**
         * Creates a mid-function hook at the given address.
         *
         * The minimum hook size on all platforms is 5 bytes. An attempt will be made to allocate a page of memory within a 4 byte jump of pDest.
         * That jump will go to a relay function that stores all the register values and then calls the hook function.
         * When the hook function returns the state of the registers will be restored and the original instructions will be executed at the given offset.
         * The relay function will then jump back to the hooked function
         */
        template<typename T = void(Registers)>
        static std::optional<asmkraken::assembly::MidFunctionHook> TryCreateMidFunctionHook(void* pDest, std::function<void(asmkraken::assembly::Registers&)> hookFunc, size_t hookSize, size_t replacedInstructionsStartOffset) {
            if (hookSize < asmkraken::assembly::JmpTemplateSize) {
                return std::nullopt;
            }

            uint32_t relayFuncMemSize = 0;
            auto relayFuncMem = mem::AllocatePageNearAddress((uintptr_t) pDest, &relayFuncMemSize);
            if (relayFuncMem == nullptr) {
                return std::nullopt;
            }

            auto offsetToRelayFunc = (intptr_t) relayFuncMem - (intptr_t) pDest;
            if (offsetToRelayFunc < INT32_MIN || offsetToRelayFunc > INT32_MAX) {
                return std::nullopt;
            }

            auto hookFuncAddr = reinterpret_cast<intptr_t>(*hookFunc.target<T*>());

            std::shared_ptr<void> relayFuncShared(relayFuncMem, [&relayFuncMemSize](void* p) { VirtualFree(p, relayFuncMemSize, MEM_FREE); });
            asmkraken::assembly::WriteRelayFunction(relayFuncShared.get(), (intptr_t) pDest, hookFuncAddr, (uint8_t*) ((uintptr_t) pDest + replacedInstructionsStartOffset), hookSize - replacedInstructionsStartOffset);

            auto* replacementBytes = new uint8_t[hookSize];
            memset(replacementBytes, 0x90, hookSize);
            memcpy(replacementBytes, asmkraken::assembly::JmpTemplate, asmkraken::assembly::JmpTemplateSize);
            *(int32_t*)(replacementBytes + 1) = (int32_t)((intptr_t)relayFuncShared.get() - ((intptr_t)pDest + (int32_t)asmkraken::assembly::JmpTemplateSize));

            auto patchBytes = asmkraken::assembly::PatchPtr(replacementBytes);
            auto patch = asmkraken::assembly::Patch(asmkraken::mem::Pointer((intptr_t) pDest), std::move(patchBytes), hookSize);
            auto opt = std::optional<asmkraken::assembly::MidFunctionHook>(asmkraken::assembly::MidFunctionHook(patch, relayFuncShared));
            return opt;
        }

    private:
        Patch hookPatch;
        std::shared_ptr<void> allocatedPage;
    };
}

#endif //LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
