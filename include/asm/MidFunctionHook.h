#ifndef LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
#define LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H

#include <cstdint>
#include "Patch.h"

namespace asmkraken::assembly {
    std::tuple<bool, Patch, intptr_t> CreateMidFunctionHook(void* pDest, void* pFunc, size_t hookSize);
}

#endif //LIB_ASMKRAKEN_MIDFUNCTIONHOOK_H
