#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sysinfoapi.h>
#include <memoryapi.h>
#include <functional>
#include "../../include/mem/mem.h"

namespace asmkraken::mem {
    void* AllocatePageNearAddress(uintptr_t targetAddr, uint32_t* pOutPageSize) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        const uint32_t pageSize = sysInfo.dwPageSize;
        *pOutPageSize = pageSize;
        uintptr_t startAddr = (targetAddr & ~((uintptr_t)pageSize - 1));
        uintptr_t minAddr = min(startAddr - 0x7FFFFF00, (uintptr_t)sysInfo.lpMinimumApplicationAddress);
        uintptr_t maxAddr = max(startAddr + 0x7FFFFF00, (uintptr_t)sysInfo.lpMaximumApplicationAddress);
        uintptr_t startPage = startAddr - (startAddr % pageSize);

        uintptr_t pageOffset = 1;
        while (true)
        {
            uintptr_t byteOffset = pageOffset * pageSize;
            uintptr_t highAddr = startPage + byteOffset;
            uintptr_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

            bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

            if (highAddr < maxAddr)
            {
                auto* outAddr = VirtualAlloc((void*)highAddr, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                if (outAddr != nullptr) {
                    return outAddr;
                }
            }

            if (lowAddr > minAddr)
            {
                auto* outAddr = VirtualAlloc((void*)lowAddr, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
                if (outAddr != nullptr) {
                    return outAddr;
                }
            }

            pageOffset++;

            if (needsExit)
            {
                break;
            }
        }

        return nullptr;
    }
}