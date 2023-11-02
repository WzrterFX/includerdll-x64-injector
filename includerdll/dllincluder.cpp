#include "include.h"

DWORD FindProcessID(const wchar_t* processName)
{
    DWORD processID = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if (Process32FirstW(snapshot, &processEntry))
        {
            while (true)
            {
                if (wcscmp(processEntry.szExeFile, processName) == 0)
                {
                    processID = processEntry.th32ProcessID;
                    break;
                }

                if (!Process32NextW(snapshot, &processEntry))
                    break;
            }
        }
        CloseHandle(snapshot);
    }
    return processID;
}

int main()
{
    const wchar_t* targetProcessName = L"MyProcces.exe";
    DWORD processID = FindProcessID(targetProcessName);

    if (processID == 0)
        return 0;

    wchar_t dllPath[MAX_PATH];
    if (GetFullPathNameW(L"MyModule.dll", MAX_PATH, dllPath, nullptr) == 0)
        return 0;

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (processHandle != nullptr)
    {
        const LPVOID loadLibraryAddress = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");

        if (loadLibraryAddress)
        {
            auto* memoryLocation = VirtualAllocEx(processHandle, nullptr, MAX_PATH * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            if (memoryLocation)
            {
                WriteProcessMemory(processHandle, memoryLocation, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), nullptr);

                HANDLE hThread = CreateRemoteThread(processHandle, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(loadLibraryAddress), memoryLocation, 0, nullptr);

                if (hThread)
                {
                    WaitForSingleObject(hThread, INFINITE);
                    CloseHandle(hThread);
                }
            }
        }
        CloseHandle(processHandle);
    }

    return 0;
}