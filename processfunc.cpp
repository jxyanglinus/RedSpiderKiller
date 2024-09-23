#include "processfunc.h"

DWORD ProcessFunc::findProcess(TCHAR name[]) {
    DWORD ID = 1; //PID为0是系统的进程，不存在PID为1的进程
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnap, &pe)) {
        do {
            if (lstrcmpi(name, pe.szExeFile) == 0) { //进程存在的条件
                ID = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
    return ID;
}

QString ProcessFunc::getProcessPath(const DWORD &pid) {
    // access process path
    WCHAR name[1024];
    for (int i = 0; i < 1024; i++) name[i] = 0;
    // open process handler
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (NULL != hProcess) {
        DWORD cbNeeded;
        HMODULE hMod;
        // process path
        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            DWORD dw = GetModuleFileNameExW(hProcess, hMod, name, 1024);
        }
        CloseHandle(hProcess);
    }
    return QString::fromWCharArray(name);
}
