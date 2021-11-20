#ifndef LIB_ASMKRAKEN_POINTER_H
#define LIB_ASMKRAKEN_POINTER_H

#include <cstdint>
#include <vector>

namespace asmkraken::mem {
    class Pointer {
    public:
        const intptr_t BaseAddress;

        explicit Pointer(intptr_t baseAddress);
        Pointer(Pointer&& p) noexcept;

        Pointer(intptr_t baseAddress, ptrdiff_t offset);

        [[nodiscard]] uintptr_t Resolve() const;

        [[nodiscard]] const std::vector<ptrdiff_t> &GetOffsets() const;

    private:
        std::vector<ptrdiff_t> offsets;
    };
}

#endif //LIB_ASMKRAKEN_POINTER_H
