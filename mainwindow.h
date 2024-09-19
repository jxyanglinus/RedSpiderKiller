#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLayout>
#include <QPushButton>
#include <tchar.h>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <QLabel>
#include <QtWinExtras/QtWinExtras>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

//#define __DEBUG__

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    const QString helpText = "一个简单的小程序，用于解除红蜘蛛电子教室控制！\n"
                             "本程序的原理非常简单：找到红蜘蛛的位置，终止红蜘蛛进程，\n"
                             "然后更改它的名称来防止它极其恶心的自动重启。\n"
                             "同时会存储红蜘蛛的路径，以便于在必要时恢复红蜘蛛。\n"
                             "若程序无法正常运行，请以管理员身份打开。\n\n"
                             "作为刚刚开始接触 Qt 的新手，程序做的很简陋，也肯定有很多问题，\n"
                             "欢迎友好讨论，愿和大家一起进步！\n\n"
                             "--来自即将退役的 Loser 竞赛生";
    const QString storeFileName = "rspath_dont_delete_me.dat"; // 本程序将路径信息存储于此
    const QString appName = "REDAgent.exe"; // 红蜘蛛本名
    const QString editedName = "StupidAgent.exe"; // 更改后名称
    TCHAR tcAppName[13] = _T("REDAgent.exe"); // 用于寻找红蜘蛛进程
//    const QString appName = "battlefield.exe"; // 本机调试程序用
//    TCHAR tcAppName[16] = _T("battlefield.exe"); // 本机调试程序用
    QString appPath; // 红蜘蛛路径
    QString editedPath; // 杀死后红蜘蛛软件名称将被修改，并存放于此路径
    DWORD pid = 0; // 红蜘蛛pid
    // 以下为窗口部件
    QVBoxLayout *layout = nullptr;
    QPushButton *killButton = nullptr;
    QPushButton *recoverButton = nullptr;
    QPushButton *recover = nullptr;
    QAction *helpAction = nullptr;
private:
    DWORD findProcess(TCHAR name[]); // 判断进程是否存在，存在返回PID，返回1代表不存在
    QString getProcessPath(const DWORD &processId); // 根据 pid 获取进程完全地址
    void killProcess(); // 杀死红蜘蛛
    void recoverProcess(); // 恢复红蜘蛛
    void getHelp(); // 帮助界面
    void setNewPath(); // 得到更改名称后的红蜘蛛的完全路径
};
#endif // MAINWINDOW_H
