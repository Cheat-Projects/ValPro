#pragma once



#include <windows.h>
#include <tlhelp32.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>



class Memory {

    DWORD     procID_{ 0 };
    HANDLE    hProcess_{ nullptr };
    uintptr_t moduleBase_{ 0 };

    public:

        /*
         * Constructs a Memory object by process name and module name.
         */
        Memory(const std::wstring& processName, const std::wstring& moduleName);


        /*
        * Destructor closes the process handle automatically.
        */
        ~Memory();


        /*
        * Reads a value of type T from an absolute address in the target process.
        */
        template <typename T>
        T Read(uintptr_t address) const;


        /*
        * Reads a value of type T by following a multi-level pointer chain.
        */
        template <typename T>
        T Read(uintptr_t base, const std::vector<unsigned int>& offsets) const;


        /**
         * Writes a value of type T to an absolute address in the target process.
         */
        template <typename T>
        void Write(uintptr_t address, const T& value) const;


        /*
        * Returns the cached base address of the specified module (supplied in the constructor).
        */
        uintptr_t GetModuleBase() const { return moduleBase_; }


        Memory(const Memory&) = delete;
        Memory& operator=(const Memory&) = delete;
        Memory(Memory&&) = delete;
        Memory& operator=(Memory&&) = delete;

    private:

        // Private helpers
        DWORD GetProcID(const std::wstring& processName) const;
        uintptr_t GetModuleBaseAddress(DWORD procID, const std::wstring& moduleName) const;
        uintptr_t FindDMAAddy(uintptr_t base, const std::vector<unsigned int>& offsets) const;

        void EnsureProcessOpen() const;

};



//fuck it all
//TODO:: DELETE AND REWRITE