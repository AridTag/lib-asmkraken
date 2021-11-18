#ifndef LIB_ASMKRAKEN_ASM_H
#define LIB_ASMKRAKEN_ASM_H

#include <cstdint>
#include <memory>

namespace asmkraken::__identifier(asm) {
    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> Patch(void *pDest, const void *pSrc, size_t size);
    std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> Nop(void *pDest, size_t size);
}

#endif //LIB_ASMKRAKEN_ASM_H
