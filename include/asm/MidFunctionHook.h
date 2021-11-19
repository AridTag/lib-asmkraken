#ifndef LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
#define LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H

#include <cstdint>
#include <optional>
#include "Patch.h"

namespace asmkraken::assembly {
    struct MidFunctionHook {
        Patch HookPatch;
        intptr_t JmpBackAddress;
    };

    std::optional<MidFunctionHook> CreateMidFunctionHook(void* pDest, void* pFunc, size_t hookSize);
}

#endif //LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
