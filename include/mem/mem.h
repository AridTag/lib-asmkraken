#ifndef LIB_ASMKRAKEN_MEM_H
#define LIB_ASMKRAKEN_MEM_H

#include <cstdint>
#include <memory>
#include <functional>

namespace asmkraken::mem {
    void* AllocatePageNearAddress(uintptr_t targetAddr, uint32_t* pOutPageSize);
}

#endif //LIB_ASMKRAKEN_MEM_H
