#ifndef LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
#define LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H

#include <cstdint>
#include <optional>
#include <functional>
#include "../Toggleable.h"
#include "Patch.h"
#include "asm.h"

namespace asmkraken::assembly {
    class MidFunctionHook : Toggleable {
    public:
        MidFunctionHook();
        MidFunctionHook(Patch& patch, void* allocatedPage, int32_t pageSize);
        ~MidFunctionHook();

        bool IsEnabled() final;
        void Toggle() final;
        void Enable() final;
        void Disable() final;

        /**
         * Creates a mid function hook at the given address.
         *
         * The minimum hook size on all platforms is 5 bytes. An attempt will be made to allocate a page of memory within a 4 byte jump of pDest.
         * That jump will go to a relay function that stores all the register values and then calls the hook function.
         * When the hook function returns the state of the registers will be restored and the original instructions will be executed at the given offset.
         * The relay function will then jump back to the hooked function
         */
        template <typename T = void(Registers)>
        static std::optional<MidFunctionHook> TryCreate(void* pDest, std::function<void(Registers)> hookFunc, size_t hookSize, size_t replacedInstructionsStartOffset);

    private:
        Patch hookPatch;
        void* allocatedPage;
        int32_t pageSize;
    };
}

#endif //LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
