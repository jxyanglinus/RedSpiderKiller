#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle(tr("红蜘蛛电子教室杀手"));
    this->setWindowIcon(QIcon(tr(":/images/icon/app-icon")));
    this->setMaximumHeight(520);
    this->setMaximumWidth(520);
    this->setMinimumHeight(520);
    this->setMinimumWidth(520);

    QPixmap pixmap = QPixmap(":/images/pictures/bk"); // 贴张背景图 :)
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    helpAction = new QAction(QIcon(tr(":/images/icon/help-icon")), "帮助", this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    helpAction->setStatusTip("点击以获取帮助");

    QMenu *helpMenu = menuBar()->addMenu("关于程序");
    helpMenu->addAction(helpAction);

    killButton = new CustomizedButton("杀掉红蜘蛛");
    recoverButton = new CustomizedButton("恢复红蜘蛛");
    delayButton = new CustomizedButton("定时关闭");

    connect(helpAction, &QAction::triggered, this, &MainWindow::getHelp);
    connect(killButton, &CustomizedButton::clicked, this, &MainWindow::killProcess);
    connect(recoverButton, &CustomizedButton::clicked, this, &MainWindow::recoverProcess);
    connect(delayButton, &CustomizedButton::clicked, this, &MainWindow::delayedOff);

    layout = new QVBoxLayout;
    layout->addWidget(killButton);
    layout->addWidget(recoverButton);
    layout->addWidget(delayButton);
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    this->setCentralWidget(widget);

    statusBar();
}

void MainWindow::getHelp() {
    HelpDialog *dialog = new HelpDialog(this);

    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
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
        QMessageBox::warning(this, "警告", "路径信息文件不存在或已删除，无法自动恢复进程，请手动恢复。");
        return;
    }
    readInfo();
    appPath.replace("\n", "");
    if (QFile::rename(editedPath, appPath)) {
        QMessageBox::information(this, "提示", "已经恢复，红蜘蛛即将自动启动。");
        QProcess::execute(appPath);
    } else if (appPath.size() == 0 && editedPath.size() == 0) {
        QMessageBox::warning(this, "警告", "路径信息文件为空。若红蜘蛛正在运行，请先执行杀死操作。");
    } else {
        QMessageBox::warning(this, "警告", "恢复失败（或红蜘蛛正在正常运行）");
    }
}

void MainWindow::delayedOff() {
    TimerQueryDlg *dialog = new TimerQueryDlg(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    if (dialog->exec() == QDialog::Accepted) {
    } else {
    }
}

void MainWindow::setNewPath() {
    appPath.replace("\\", "\\\\");
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

void MainWindow::readInfo() {
    if (!QFile::exists(storeFileName)) {
        return;
    }
    QFile file(storeFileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    int cnt = 0;
    while (!file.atEnd()) {
        QString curLine = file.readLine();
        cnt++;
        if (cnt == 1) appPath = curLine.toUtf8();
        else if (cnt == 2) editedPath = curLine.toUtf8();
    }
    file.close();
}

MainWindow::~MainWindow() {
}
