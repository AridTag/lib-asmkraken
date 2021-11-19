#include "../../include/asm/Patch.h"
#include "../../include/asm/asm.h"

#include <utility>

namespace asmkraken::assembly {
    Patch::Patch() : Patch(Pointer(0), nullptr, 0) {

    }

    Patch::Patch(Pointer targetAddress, PatchPtr patchBytes, size_t patchSize) :
            patchTargetAddress(std::move(targetAddress)),
            patchBytes(std::move(patchBytes)),
            patchSize(patchSize) {

    }

    Patch::Patch(Pointer targetAddress, size_t patchSize) :
            Patch(std::move(targetAddress), nullptr, patchSize) {
    }

    Patch::Patch(Patch&& p) noexcept :
            patchTargetAddress(std::move(p.patchTargetAddress)),
            patchBytes(std::move(p.patchBytes)),
            patchSize(std::exchange(p.patchSize, 0)) {
    }

    Patch::~Patch() {
        Disable();
    }

    bool Patch::IsEnabled() {
        return originalBytes != nullptr;
    }

    void Patch::Toggle() {
        if (IsEnabled()) {
            Disable();
        } else {
            Enable();
        }
    }

    void Patch::Enable() {
        if (IsEnabled()) {
            return;
        }

        void* pTarget = (void*)patchTargetAddress.Resolve();
        uint8_t* pPatchBytes = patchBytes.get();
        if (pPatchBytes == nullptr) {
            originalBytes = Nop(pTarget, patchSize);
        } else {
            originalBytes = PatchAsm(pTarget, pPatchBytes, patchSize);
        }
    }

    void Patch::Disable() {
        if (!IsEnabled()) {
            return;
        }

        PatchAsm((void*)patchTargetAddress.Resolve(), originalBytes.get(), patchSize);
        originalBytes.reset(nullptr);
    }
}
