#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace ProcessUtils
{
    uintptr_t GetProcessBaseAddress(DWORD ProcessId);
    std::string GetExecutableName(DWORD pid);
}

namespace MemoryUtils
{
    uintptr_t PatternScan(HANDLE ProcessHandle, uintptr_t BaseAddress, const std::string& Pattern);
    uintptr_t GetRipRelativeAddress(HANDLE ProcessHandle, uintptr_t InstructionAddress);
    uintptr_t GetOffsetsFromPattern(HANDLE ProcessHandle, uintptr_t BaseAddress, const std::string& Pattern, bool ShouldDereference);
    uintptr_t DereferencePointer(HANDLE ProcessHandle, uintptr_t Address);
}

struct PatternInfo
{
    const char* Name;
    const char* Signature;
    bool ShouldDereference;
};
