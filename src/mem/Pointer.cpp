#include "Pointer.h"

namespace asmkraken::mem {
    Pointer::Pointer(intptr_t baseAddress) : BaseAddress(baseAddress), offsets(8) {
    }

    Pointer::Pointer(intptr_t baseAddress, int32_t offset) : Pointer(baseAddress) {
        offsets.push_back(offset);
    }

    const std::vector<int32_t> &Pointer::GetOffsets() const {
        return offsets;
    }

    uintptr_t Pointer::Resolve() const {
        uintptr_t addr = BaseAddress;
        for (const auto &off: offsets) {
            addr = *(uintptr_t *) addr;
            addr += off;
        }

        return addr;
    }
}