#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QGridLayout>
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
#include <QStandardPaths>
#include "customizedbutton.h"
#include "timerquerydlg.h"
#include "helpdialog.h"
#include "processfunc.h"
#include "delayedoffdlg.h"

//#define __DEBUG__

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    const QString storeFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                  + "\\.red_spider_path_file.inf"; // 本程序将路径信息存储于此
    const QString appName = "REDAgent.exe"; // 红蜘蛛本名
    const QString editedName = "StupidAgent.exe"; // 更改后名称
    TCHAR tcAppName[13] = _T("REDAgent.exe"); // 用于寻找红蜘蛛进程
    QString appPath; // 红蜘蛛路径
    QString editedPath; // 杀死后红蜘蛛软件名称将被修改，并存放于此路径
    DWORD pid = 0; // 红蜘蛛pid
private:
    void killProcess(); // 杀死红蜘蛛
    void recoverProcess(); // 恢复红蜘蛛
    void getHelp(); // 帮助界面
    void delayedOff(); // 定时关闭（延时关闭）
    void setNewPath(); // 得到更改名称后的红蜘蛛的完全路径
    void readInfo(); // 从文件读取信息
};
#endif // MAINWINDOW_H
