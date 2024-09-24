#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setWindowTitle(tr("红蜘蛛电子教室杀手"));
    this->setWindowIcon(QIcon(tr(":/images/icon/app-icon")));
    this->setMaximumHeight(450);
    this->setMaximumWidth(450);
    this->setMinimumHeight(450);
    this->setMinimumWidth(450);

    QPixmap pixmap = QPixmap(":/images/pictures/bk"); // 贴张背景图 :)
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    QAction *helpAction = new QAction(QIcon(tr(":/images/icon/help-icon")), "帮助", this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    helpAction->setStatusTip("点击以获取帮助");

    QMenu *helpMenu = menuBar()->addMenu("关于程序");
    helpMenu->addAction(helpAction);

    CustomizedButton *killButton = new CustomizedButton("杀掉红蜘蛛");
    killButton->setStatusTip("立即杀死红蜘蛛电子教室");

    CustomizedButton *recoverButton = new CustomizedButton("恢复红蜘蛛");
    recoverButton->setStatusTip("恢复红蜘蛛，并将在不久后自动启动");

    CustomizedButton *delayButton = new CustomizedButton("定时关闭");
    delayButton->setStatusTip("设定一个倒计时，倒计时结束将自动杀死红蜘蛛");

    connect(helpAction, &QAction::triggered, this, &MainWindow::getHelp);
    connect(killButton, &CustomizedButton::clicked, this, &MainWindow::killProcess);
    connect(recoverButton, &CustomizedButton::clicked, this, &MainWindow::recoverProcess);
    connect(delayButton, &CustomizedButton::clicked, this, &MainWindow::delayedOff);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(killButton, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(recoverButton, 1, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(delayButton, 2, 0, 1, 1, Qt::AlignCenter);
    layout->setHorizontalSpacing(20);
    layout->setVerticalSpacing(20);
    layout->setContentsMargins(50, 50, 50, 50);
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
    pid = ProcessFunc::findProcess(tcAppName);
    if (pid == 1) {
        QMessageBox::information(this, "提示", "红蜘蛛进程不存在。");
        return;
    }
    appPath = ProcessFunc::getProcessPath(pid);
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
    } else if (appPath.size() == 0 && editedPath.size() == 0) {
        QMessageBox::warning(this, "警告", "路径信息文件为空。若红蜘蛛正在运行，请先执行杀死操作。");
    } else {
        QMessageBox::warning(this, "警告", "恢复失败（或红蜘蛛正在正常运行）");
    }
}

void MainWindow::delayedOff() {
    if (ProcessFunc::findProcess(tcAppName) == 1) {
        QMessageBox::information(this, "提示", "红蜘蛛进程不存在。");
        return;
    }
    TimerQueryDlg dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int countdown = dialog.getCountdown();
        DelayedOffDlg offDialog;
        offDialog.setCountdown(countdown);
        if (offDialog.exec() == QDialog::Rejected) {
            QMessageBox::information(this, "提示", "已取消");
        }
    }
}

void MainWindow::setNewPath() {
    appPath.replace("\\", "\\\\");
    editedPath = appPath;
    editedPath.replace(appName, editedName);
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
