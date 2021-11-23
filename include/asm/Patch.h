#ifndef LIB_ASMKRAKEN_PATCH_H
#define LIB_ASMKRAKEN_PATCH_H

#include <memory>
#include "../Toggleable.h"
#include "../mem/Pointer.h"

namespace asmkraken::assembly {
    using mem::Pointer;

    typedef std::unique_ptr<uint8_t, std::default_delete<uint8_t[]>> PatchPtr;

    class Patch : Toggleable {
    public:
        Patch();
        Patch(Pointer targetAddress, PatchPtr patchBytes, size_t patchSize);
        Patch(Pointer targetAddress, size_t patchSize);
        Patch(Patch&& p) noexcept;
        ~Patch();

        bool IsEnabled() final;
        void Toggle() final;
        void Enable() final;
        void Disable() final;

    private:
        Pointer patchTargetAddress;
        PatchPtr patchBytes;
        size_t patchSize;
        PatchPtr originalBytes;
    };
}

#endif //LIB_ASMKRAKEN_PATCH_H
