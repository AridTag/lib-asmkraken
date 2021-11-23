#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memoryapi.h>

#include <utility>
#include "../../include/asm/MidFunctionHook.h"
#include "../../include/mem/mem.h"

namespace asmkraken::assembly {
    MidFunctionHook::MidFunctionHook() :
            hookPatch(),
            allocatedPage(nullptr) {
    }

    MidFunctionHook::MidFunctionHook(Patch& patch, std::shared_ptr<void> allocatedPage) :
            hookPatch(std::move(patch)),
            allocatedPage(std::move(allocatedPage)) {
    }

    MidFunctionHook::MidFunctionHook(MidFunctionHook&& m) noexcept :
        hookPatch(std::move(m.hookPatch)),
        allocatedPage(std::move(m.allocatedPage)) {
    }

    bool MidFunctionHook::IsEnabled() {
        return hookPatch.IsEnabled();
    }

    void MidFunctionHook::Toggle() {
        if (IsEnabled()) {
            Disable();
        } else {
            Enable();
        }
    }

    void MidFunctionHook::Enable() {
        if (IsEnabled()) {
            return;
        }

        hookPatch.Enable();
    }

    void MidFunctionHook::Disable() {
        if (!IsEnabled()) {
            return;
        }

        hookPatch.Disable();
    }
}