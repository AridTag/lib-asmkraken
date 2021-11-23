#ifndef LIB_ASMKRAKEN_MEM_H
#define LIB_ASMKRAKEN_MEM_H

#include <cstdint>

namespace asmkraken::mem {
    void* AllocatePageNearAddress(uintptr_t targetAddr, int32_t* pOutPageSize);
}

#endif //LIB_ASMKRAKEN_MEM_H
