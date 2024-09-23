#ifndef PROCESSFUNC_H
#define PROCESSFUNC_H

#include <QProcess>
#include <QDebug>
#include <QtWinExtras/QtWinExtras>
#include <QString>
#include <psapi.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <windows.h>
#include <windef.h>

class ProcessFunc {
public:
    static DWORD findProcess(TCHAR name[]); // 判断进程是否存在，存在返回PID，返回1代表不存在
    static QString getProcessPath(const DWORD &pid); // 根据 pid 获取进程完全地址
};

#endif // PROCESSFUNC_H
