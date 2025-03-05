#include "memory.hpp"
#include <tlhelp32.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>



// -------------------------------------------
// Constructor / Destructor
// -------------------------------------------

Memory::Memory(const std::wstring& processName, const std::wstring& moduleName) {
    procID_ = GetProcID(processName);
    //if (!procID_) {
    //    std::cerr << "Could not find process: " + std::string(processName.begin(), processName.end());
    //}

    hProcess_ = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID_);
    //if (!hProcess_ || hProcess_ == INVALID_HANDLE_VALUE) {
    //    std::cerr << "OpenProcess failed. Win32 error: " + std::to_string(GetLastError());
    //}

    moduleBase_ = GetModuleBaseAddress(procID_, moduleName);
    //if (!moduleBase_) {
    //    std::cerr << "Could not find module base for: " + std::string(moduleName.begin(), moduleName.end());
    //}
}


Memory::~Memory() {
    if (hProcess_ && hProcess_ != INVALID_HANDLE_VALUE) {
        CloseHandle(hProcess_);
        hProcess_ = nullptr;
    }
}



// -------------------------------------------
// Template methods
// -------------------------------------------

template <typename T>
T Memory::Read(uintptr_t address) const {
    EnsureProcessOpen();

    T buffer{};
    SIZE_T bytesRead = 0;
    if (!ReadProcessMemory(hProcess_,
        reinterpret_cast<LPCVOID>(address),
        &buffer,
        sizeof(T),
        &bytesRead)
        || bytesRead != sizeof(T))
    {
        //std::ostringstream oss;
        //oss << "Read<T> failed at address 0x" << std::hex << address
        //    << " [Win32 error: " << std::dec << GetLastError() << "]";
        //std::cerr << oss.str();
    }
    return buffer;
}


template <typename T>
T Memory::Read(uintptr_t base, const std::vector<unsigned int>& offsets) const {
    uintptr_t finalAddr = FindDMAAddy(base, offsets);
    //if (!finalAddr) {
    //    std::cerr << "Read<T>: invalid final address from pointer chain";
    //}
    return Read<T>(finalAddr);
}


template <typename T>
void Memory::Write(uintptr_t address, const T& value) const {
    EnsureProcessOpen();

    SIZE_T bytesWritten = 0;
    if (!WriteProcessMemory(hProcess_,
        reinterpret_cast<LPVOID>(address),
        &value,
        sizeof(T),
        &bytesWritten)
        || bytesWritten != sizeof(T))
    {
        //std::ostringstream oss;
        //oss << "Write<T> failed at address 0x" << std::hex << address
        //    << " [Win32 error: " << std::dec << GetLastError() << "]";
        //std::cerr << oss.str();
    }
}




// -------------------------------------------
// Private Helpers
// -------------------------------------------

DWORD Memory::GetProcID(const std::wstring& processName) const {
    DWORD pid = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    //if (hSnapshot == INVALID_HANDLE_VALUE) {
    //    std::cerr << "CreateToolhelp32Snapshot (process) failed. Error: " + std::to_string(GetLastError());
    //}

    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(pe32);

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}


uintptr_t Memory::GetModuleBaseAddress(DWORD pid, const std::wstring& moduleName) const {
    uintptr_t baseAddress = 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    //if (hSnapshot == INVALID_HANDLE_VALUE) {
    //    std::cerr << "CreateToolhelp32Snapshot (module) failed. Error: " + std::to_string(GetLastError());
    //}

    MODULEENTRY32W modEntry{};
    modEntry.dwSize = sizeof(modEntry);

    if (Module32FirstW(hSnapshot, &modEntry)) {
        do {
            if (_wcsicmp(modEntry.szModule, moduleName.c_str()) == 0) {
                baseAddress = reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
                break;
            }
        } while (Module32NextW(hSnapshot, &modEntry));
    }

    CloseHandle(hSnapshot);
    return baseAddress;
}


uintptr_t Memory::FindDMAAddy(uintptr_t base, const std::vector<unsigned int>& offsets) const {
    uintptr_t addr = base;
    for (auto offset : offsets) {
        uintptr_t temp = 0;
        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(hProcess_,
                               reinterpret_cast<LPCVOID>(addr),
                               &temp,
                               sizeof(temp),
                               &bytesRead)
            || bytesRead != sizeof(temp))
        {
            //std::cerr <<
            //    ("FindDMAAddy - pointer read failed at 0x" + std::to_string(addr) +
            //    " [Win32 error: " + std::to_string(GetLastError()) + "]"
            //);
        }
        addr = temp + offset;
    }
    return addr;
}


void Memory::EnsureProcessOpen() const {
    if (!hProcess_ || hProcess_ == INVALID_HANDLE_VALUE) {
        std::cerr << "Memory: invalid or closed process handle.";
    }
}




//its experimental structure. this bullshit needs to be rewrited
//it is shitty chatgpt code