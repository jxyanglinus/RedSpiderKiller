#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle(tr("红蜘蛛电子教室终结者"));
    this->setWindowIcon(QIcon(tr(":/images/icon/app-icon")));
    this->setMaximumHeight(576);
    this->setMaximumWidth(576);
    this->setMinimumHeight(576);
    this->setMinimumWidth(576);

    helpAction = new QAction(QIcon(tr(":/images/icon/help-icon")), "帮助", this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    helpAction->setStatusTip("点击以获取帮助");

    QMenu *helpMenu = menuBar()->addMenu("关于程序");
    helpMenu->addAction(helpAction);

    killButton = new QPushButton("杀掉红蜘蛛");
    recoverButton = new QPushButton("恢复红蜘蛛");
    killButton->setFont(QFont("Microsoft Yahei UI", 20));
    recoverButton->setFont(QFont("Microsoft Yahei UI", 20));

    connect(helpAction, &QAction::triggered, this, &MainWindow::getHelp);
    connect(killButton, &QPushButton::clicked, this, &MainWindow::killProcess);
    connect(recoverButton, &QPushButton::clicked, this, &MainWindow::recoverProcess);

    layout = new QVBoxLayout;
    layout->addWidget(killButton);
    layout->addWidget(recoverButton);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    this->setCentralWidget(widget);

    statusBar();
}

void MainWindow::getHelp() {
    QDialog *helpDialog = new QDialog(this);
    QLabel *label = new QLabel(helpDialog);

    helpDialog->setAttribute(Qt::WA_DeleteOnClose);
    helpDialog->setMinimumHeight(220);
    helpDialog->setMinimumWidth(500);

    label->setFont(QFont("Microsoft Yahei UI", 12));
    label->setText(helpText);

    helpDialog->show();
}

void MainWindow::killProcess() {
    pid = findProcess(tcAppName);
    if (pid == 1) {
        QMessageBox::information(this, "提示", "红蜘蛛进程不存在。");
        return;
    }
    appPath = getProcessPath(pid);
#ifdef __DEBUG__
    qDebug() << "Current app path: " << appPath;
#endif
    setNewPath();
    appPath.replace("\\", "\\\\");
    editedPath.replace("\\", "\\\\");

    QFile file(storeFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(appPath.toUtf8());
    file.write(QString("\n").toUtf8());
    file.write(editedPath.toUtf8());
    file.close();

    int killStatus = QProcess::execute("taskkill -f -im " + appName);
    bool renameStatus = QFile::rename(appPath, editedPath);
    if (killStatus) {
        QMessageBox::warning(this, "警告", "无法终止进程。");
    } else if (!renameStatus) {
        QMessageBox::warning(this, "警告", "更改文件名失败。");
    } else {
        QMessageBox::information(this, "提示", "成功杀死红蜘蛛！已生成路径文件，请勿删除。");
    }
}

void MainWindow::recoverProcess() {
    if (!QFile::exists(storeFileName)) {
        QMessageBox::warning(this, "警告", "路径文件不存在或已删除，无法自动恢复进程，请手动恢复。");
        return;
    }
    QFile file(storeFileName);
    int cnt = 0;
    while (!file.atEnd()) {
        QString curLine = file.readLine();
        cnt++;
        if (cnt == 1) appPath = curLine;
        else if (cnt == 2) editedPath = curLine;
    }
    bool renameStatus = QFile::rename(editedPath, appPath);
    if (renameStatus) {
        QMessageBox::information(this, "提示", "已经恢复，红蜘蛛即将自动启动。");
    } else {
        QMessageBox::warning(this, "警告", "恢复失败（或红蜘蛛正在正常运行）");
    }
}

void MainWindow::setNewPath() {
    editedPath = appPath;
    editedPath.replace(appName, editedName);
}

DWORD MainWindow::findProcess(TCHAR name[]) {
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

QString MainWindow::getProcessPath(const DWORD &pid) {
//    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,0,pid);      // 获取进程句柄
//    TCHAR process_name[MAX_PATH] = {};
//    QString fullPath;
//    if(!GetModuleFileNameEx(hProcess,NULL,process_name,MAX_PATH)) {
//#ifdef __DEBUG__
//        qDebug()<<"Error 2";
//#endif
//        fullPath = "";
//    } else {
//        fullPath = QString::fromUtf16((ushort*)process_name);      // 转换TCHAT到QString
//#ifdef __DEBUG__
//        qDebug()<<fullPath;
//#endif
//    }
//    return fullPath;

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

MainWindow::~MainWindow() {
}
