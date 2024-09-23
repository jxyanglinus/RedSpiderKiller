#ifndef DELAYEDOFFDLG_H
#define DELAYEDOFFDLG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QFile>
#include <QtWinExtras/QtWinExtras>
#include <psapi.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <windows.h>
#include <windef.h>
#include <QPushButton>
#include <QMessageBox>
#include "processfunc.h"

class DelayedOffDlg : public QDialog {
    Q_OBJECT
public:
    DelayedOffDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setCountdown(const int &cntdown);
private:
    int countdown = 0;
    const QString storeFileName = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                  + "\\.red_spider_path_file.inf"; // 本程序将路径信息存储于此
    const QString appName = "REDAgent.exe"; // 红蜘蛛本名
    const QString editedName = "StupidAgent.exe"; // 更改后名称
    TCHAR tcAppName[13] = _T("REDAgent.exe"); // 用于寻找红蜘蛛进程
    QString appPath; // 红蜘蛛路径
    QString editedPath; // 杀死后红蜘蛛软件名称将被修改，并存放于此路径
    // 以下为窗口部件
    QVBoxLayout *layout = nullptr;
    QPushButton *button = nullptr;
private:
    void delayedOffExec();
    void setNewText();
};

#endif // DELAYEDOFFDLG_H
