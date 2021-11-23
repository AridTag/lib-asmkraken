#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winnt.h>
#include <minwindef.h>
#include <TlHelp32.h>

#include "../../include/asm/asm.h"

namespace asmkraken::assembly {
    std::unique_ptr<uint8_t[]> PatchAsm(void* pDest, const void* pSrc, size_t size) {
        auto* pOriginalBytes = new uint8_t[size];
        DWORD oldProtection;
        if (VirtualProtect(pDest, size, PAGE_EXECUTE_READWRITE, &oldProtection) == FALSE) {
            return nullptr;
        }

        memcpy(pOriginalBytes, pDest, size);
        memcpy(pDest, pSrc, size);

        DWORD oldProtection2;
        VirtualProtect(pDest, size, oldProtection, &oldProtection2);

        return std::unique_ptr<uint8_t[]>(pOriginalBytes);
    }

    std::unique_ptr<uint8_t[]> Nop(void* pDest, size_t size) {
        auto* nopArray = new uint8_t[size];
        memset(nopArray, 0x90, size);
        auto originalBytes = PatchAsm(pDest, nopArray, size);
        delete[] nopArray;

        return originalBytes;
    }

    void WriteRelayFunction(void* pDest, intptr_t hookTargetAddr, intptr_t hookFuncAddr, uint8_t* additionalInstructions, size_t additionalInstructionsSize) {
        auto writeAddr = (intptr_t) pDest;

        memcpy((void*) writeAddr, RelayFunctionPrologue, RelayFunctionPrologueSize);
        writeAddr += RelayFunctionPrologueSize;

        memcpy((void*) writeAddr, RelayFunctionTemplate, RelayFunctionContentSize);
        *(intptr_t*) (writeAddr + RelayFunctionTargetAddrOffset) = hookFuncAddr;
        writeAddr += RelayFunctionContentSize;

        memcpy((void*) writeAddr, RelayFunctionEpilogue, RelayFunctionEpilogueSize);
        writeAddr += RelayFunctionEpilogueSize;

        if (additionalInstructions != nullptr) {
            memcpy((void*) writeAddr, additionalInstructions, additionalInstructionsSize);
            writeAddr += (intptr_t) additionalInstructionsSize;
        }

        memcpy((void*) writeAddr, JmpTemplate, JmpTemplateSize);
        auto offsetToRelayFunc = (int32_t) abs(writeAddr - hookTargetAddr);
        if (hookTargetAddr < writeAddr) {
            offsetToRelayFunc = -offsetToRelayFunc;
        }
        *(int32_t*) (writeAddr + 1) = offsetToRelayFunc;
    }

    void SuspendOtherThreads(bool suspend) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            THREADENTRY32 te;
            te.dwSize = sizeof(THREADENTRY32);
            if (Thread32First(hSnapshot, &te)) {
                do {
                    if (te.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(DWORD))
                        && te.th32OwnerProcessID == GetCurrentProcessId()
                        && te.th32ThreadID != GetCurrentThreadId()) {

                        HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                        if (thread != nullptr) {
                            if (suspend) {
                                SuspendThread(thread);
                            } else {
                                ResumeThread(thread);
                            }
                            CloseHandle(thread);
                        }
                    }
                } while (Thread32Next(hSnapshot, &te));
            }
        }
    }
}
