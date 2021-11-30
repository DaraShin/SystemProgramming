#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

void ListProcessThreads(DWORD const dwOwnerPID)
{
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 threadEntry32;

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("CreateToolhelp32Snapshot (of threads) failed"));
        return;
    }

    threadEntry32.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hThreadSnap, &threadEntry32))
    {
        _tprintf(TEXT("Thread32First failed"));
        CloseHandle(hThreadSnap);
        return;
    }

    // Walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do
    {
        if (threadEntry32.th32OwnerProcessID == dwOwnerPID)
        {
            _tprintf(TEXT("\n\n     THREAD ID      = 0x%08X"), threadEntry32.th32ThreadID);
            _tprintf(TEXT("\n     Base priority  = %d"), threadEntry32.tpBasePri);

        }
    } while (Thread32Next(hThreadSnap, &threadEntry32));

    CloseHandle(hThreadSnap);
}

void ListProcessModules(DWORD const dwPID)
{
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 moduleEntry32;

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("CreateToolhelp32Snapshot (of modules) failed"));
        return;
    }

    moduleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(hModuleSnap, &moduleEntry32))
    {
        _tprintf(TEXT("Module32First failed"));
        CloseHandle(hModuleSnap);
        return;
    }

    // Walk the module list of the process,
    // and display information about each module
    do
    {
        _tprintf(TEXT("\n\n     MODULE NAME    = %s"), moduleEntry32.szModule);
        _tprintf(TEXT("\n     Path           = %s"), moduleEntry32.szExePath);
        _tprintf(TEXT("\n     Base size      = %d"), moduleEntry32.modBaseSize);

    } while (Module32Next(hModuleSnap, &moduleEntry32));

    CloseHandle(hModuleSnap);
}

void GetProcessList()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 processEntry32;
    DWORD dwPriorityClass;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf(TEXT("CreateToolhelp32Snapshot (of processes) failed"));
        return;
    }

    processEntry32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &processEntry32))
    {
        _tprintf(TEXT("Process32First failed"));
        CloseHandle(hProcessSnap);
        return;
    }

    // Walk the snapshot of processes, and
    // display information about each process in turn
    do
    {
        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry32.th32ProcessID);
        if (hProcess == NULL)
        {
            _tprintf(TEXT("OpenProcess failed"));
        }
        else
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass)
            {
                _tprintf(TEXT("GetPriorityClass failed"));
            }

            CloseHandle(hProcess);
        }

        _tprintf(TEXT("\n\n====================================================="));
        _tprintf(TEXT("\n  Process ID          = 0x%08X"), processEntry32.th32ProcessID);
        _tprintf(TEXT("\n  Process name        = %s"), processEntry32.szExeFile);
        _tprintf(TEXT("\n  Parent process ID   = 0x%08X"), processEntry32.th32ParentProcessID);
        if (dwPriorityClass)
        {
            _tprintf(TEXT("\n  Priority class      = %d"), dwPriorityClass);
        }

        _tprintf(TEXT("\n-------------------------------------------------------"));

        _tprintf(TEXT("\n  Thread count        = %d"), processEntry32.cntThreads);
        ListProcessThreads(processEntry32.th32ProcessID);
        ListProcessModules(processEntry32.th32ProcessID);
    } while (Process32Next(hProcessSnap, &processEntry32));

    CloseHandle(hProcessSnap);
}

int main(void)
{
    GetProcessList();
    return 0;
}