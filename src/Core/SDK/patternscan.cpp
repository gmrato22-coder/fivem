#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <TlHelp32.h>
#include <Psapi.h>
#include "patternscan.h"

#pragma comment(lib, "Psapi.lib")

namespace ProcessUtils
{
    uintptr_t GetProcessBaseAddress(DWORD ProcessId)
    {
        HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessId);
        if (ProcessHandle == NULL)
            return 0;

        HMODULE ModuleHandles[1024];
        DWORD BytesNeeded;
        if (EnumProcessModules(ProcessHandle, ModuleHandles, sizeof(ModuleHandles), &BytesNeeded))
        {
            CloseHandle(ProcessHandle);
            return (uintptr_t)ModuleHandles[0];
        }

        CloseHandle(ProcessHandle);
        return 0;
    }

    std::string GetExecutableName(DWORD pid)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProcess) return "";

        HMODULE hMod;
        DWORD cbNeeded;
        char szProcessName[MAX_PATH] = { 0 };

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
        }

        CloseHandle(hProcess);
        return std::string(szProcessName);
    }
}

namespace MemoryUtils
{
    uintptr_t PatternScan(HANDLE ProcessHandle, uintptr_t BaseAddress, const std::string& Pattern)
    {
        std::vector<int> PatternBytes;
        std::stringstream StringStream(Pattern);
        std::string ByteString;

        while (StringStream >> ByteString)
        {
            if (ByteString == "?" || ByteString == "??")
                PatternBytes.push_back(-1);
            else
                PatternBytes.push_back(std::stoi(ByteString, nullptr, 16));
        }

        if (ProcessHandle == NULL)
            return 0;

        MEMORY_BASIC_INFORMATION MemoryInfo;
        std::vector<char> Buffer;
        uintptr_t CurrentAddress = BaseAddress;

        while (VirtualQueryEx(ProcessHandle, (LPCVOID)CurrentAddress, &MemoryInfo, sizeof(MemoryInfo)))
        {
            if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & PAGE_GUARD) == 0)
            {
                Buffer.resize(MemoryInfo.RegionSize);
                SIZE_T BytesRead;

                if (ReadProcessMemory(ProcessHandle, MemoryInfo.BaseAddress, Buffer.data(), MemoryInfo.RegionSize, &BytesRead))
                {
                    for (size_t i = 0; i < (BytesRead - PatternBytes.size()); ++i)
                    {
                        bool Found = true;
                        for (size_t j = 0; j < PatternBytes.size(); ++j)
                        {
                            if (PatternBytes[j] != -1 && PatternBytes[j] != (unsigned char)Buffer[i + j])
                            {
                                Found = false;
                                break;
                            }
                        }

                        if (Found)
                            return (uintptr_t)MemoryInfo.BaseAddress + i;
                    }
                }
            }
            CurrentAddress = (uintptr_t)MemoryInfo.BaseAddress + MemoryInfo.RegionSize;
        }

        return 0;
    }

    uintptr_t GetRipRelativeAddress(HANDLE ProcessHandle, uintptr_t InstructionAddress)
    {
        if (InstructionAddress == 0)
            return 0;

        int32_t RelativeOffset = 0;
        SIZE_T BytesRead = 0;

        if (!ReadProcessMemory(ProcessHandle, (LPCVOID)(InstructionAddress + 3), &RelativeOffset, sizeof(RelativeOffset), &BytesRead) || BytesRead != sizeof(RelativeOffset))
            return 0;

        return InstructionAddress + 7 + RelativeOffset;
    }

    uintptr_t DereferencePointer(HANDLE ProcessHandle, uintptr_t Address)
    {
        if (Address == 0)
            return 0;

        uintptr_t DereferencedAddress = 0;
        SIZE_T BytesRead = 0;

        if (!ReadProcessMemory(ProcessHandle, (LPCVOID)Address, &DereferencedAddress, sizeof(DereferencedAddress), &BytesRead) || BytesRead != sizeof(DereferencedAddress))
            return 0;

        return DereferencedAddress;
    }

    uintptr_t GetOffsetsFromPattern(HANDLE ProcessHandle, uintptr_t BaseAddress, const std::string& Pattern, bool ShouldDereference) {
        uintptr_t InstructionAddress = MemoryUtils::PatternScan(ProcessHandle, BaseAddress, Pattern);

        uintptr_t ResolvedAddress = MemoryUtils::GetRipRelativeAddress(ProcessHandle, InstructionAddress);
        uintptr_t OffsetFix = ResolvedAddress - BaseAddress;
        uintptr_t FinalAddress = ShouldDereference
            ? MemoryUtils::DereferencePointer(ProcessHandle, ResolvedAddress)
            : ResolvedAddress;

        return OffsetFix;
    }
}


//int main()
//{
//    HWND WindowHandle = FindWindowA("grcWindow", NULL);
//    if (!WindowHandle)
//    {
//        return 1;
//    }
//
//    DWORD ProcessId;
//    GetWindowThreadProcessId(WindowHandle, &ProcessId);
//    if (!ProcessId)
//    {
//        return 1;
//    }
//
//    std::string exeName = ProcessUtils::GetExecutableName(ProcessId);
//    int build = 0;
//    size_t bPos = exeName.find("_b");
//    if (bPos != std::string::npos)
//    {
//        std::string buildStr = exeName.substr(bPos + 2);
//        try
//        {
//            build = std::stoi(buildStr);
//            std::cout << "FiveM build: " << build << std::endl;
//        }
//        catch (...)
//        {
//            std::cout << "[removed]" << buildStr << std::endl;
//        }
//    }
//
//
//    HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessId);
//    if (!ProcessHandle)
//    {
//        return 1;
//    }
//
//    uintptr_t BaseAddress = ProcessUtils::GetProcessBaseAddress(ProcessId);
//    if (!BaseAddress)
//    {
//        CloseHandle(ProcessHandle);
//        return 1;
//    }
//
//    std::cout << "Base address: 0x" << std::hex << BaseAddress << std::endl;
//
//    PatternInfo Patterns[] = {
//        {"World", "48 8B 05 ? ? ? ? 33 D2 48 8B 40 08 8A CA 48 85 C0 74 16 48 8B", true},
//        {"ReplayInterFace", "48 8D 0D ? ? ? ? 48 ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 74 BC 8B 8F", false},
//        {"ViewPort", "48 8B 15 ? ? ? ? 48 8D 2D ? ? ? ? 48 8B CD", true},
//        {"BlipList", "4C 8D 05 ? ? ? ? 0F B7 C1", false},
//        {"CamGameplayDirector", "4C 8B 35 ? ? ? ? 33 FF 32 DB", true},
//        {"Camera", "48 8B 05 ? ? ? ? 38 98 ? ? ? ? 8A C3", true},
//        {"Bullet instance", "F3 41 0F 10 19 F3 41 0F 10 41 04", true}
//    };
//
//    for (const auto& Pattern : Patterns)
//    {
//        uintptr_t InstructionAddress = MemoryUtils::PatternScan(ProcessHandle, BaseAddress, Pattern.Signature);
//        if (!InstructionAddress)
//        {
//            std::cout << Pattern.Name << ": Pattern not found." << std::endl;
//            continue;
//        }
//
//        uintptr_t ResolvedAddress = MemoryUtils::GetRipRelativeAddress(ProcessHandle, InstructionAddress);
//        uintptr_t OffsetFix = ResolvedAddress - BaseAddress;
//
//        uintptr_t FinalAddress = Pattern.ShouldDereference
//            ? MemoryUtils::DereferencePointer(ProcessHandle, ResolvedAddress)
//            : ResolvedAddress;
//
//        std::cout << Pattern.Name << ":";
//        printf(" (RIP-relative): 0x%p\n", OffsetFix);
//
//    }
//
//    CloseHandle(ProcessHandle);
//    std::cin.get();
//    return 0;
//}