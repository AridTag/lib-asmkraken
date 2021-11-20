#include "../../include/mem/Pointer.h"

namespace asmkraken::mem {
    Pointer::Pointer(intptr_t baseAddress) : BaseAddress(baseAddress), offsets() {
    }

    Pointer::Pointer(intptr_t baseAddress, ptrdiff_t offset) : Pointer(baseAddress) {
        offsets.push_back(offset);
    }

    Pointer::Pointer(Pointer&& p) noexcept:
            BaseAddress(p.BaseAddress),
            offsets(std::move(p.offsets)) {
    }

    const std::vector<ptrdiff_t>& Pointer::GetOffsets() const {
        return offsets;
    }

    uintptr_t Pointer::Resolve() const {
        uintptr_t addr = BaseAddress;
        for (const auto& off: offsets) {
            addr = *(uintptr_t*) addr;
            addr += off;
        }

        return addr;
    }
}