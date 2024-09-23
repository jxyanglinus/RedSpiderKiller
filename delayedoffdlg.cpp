#include "delayedoffdlg.h"

DelayedOffDlg::DelayedOffDlg(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f) {
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setWindowIcon(QIcon(":/images/icon/app-icon"));
    this->setWindowTitle("倒计时");
    this->setMinimumWidth(300);

    button = new QPushButton("开始");
    button->setFont(QFont("黑体", 15));
    connect(button, &QPushButton::clicked, this, &DelayedOffDlg::delayedOffExec);

    layout = new QVBoxLayout;
    layout->addWidget(button);
    this->setLayout(layout);
}

void DelayedOffDlg::delayedOffExec() {
    button->setDisabled(true);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DelayedOffDlg::setNewText);
    timer->start(1000);
}

void DelayedOffDlg::setNewText() {
    button->setText(QString("剩余：%1 秒").arg(countdown));
    countdown--;
    // 设置文本顺带终结程序
    if (!countdown) {
        int pid = ProcessFunc::findProcess(tcAppName);
        if (pid == 1) {
            QMessageBox::information(this, "提示", "红蜘蛛进程不存在。");
            return;
        }
        appPath = ProcessFunc::getProcessPath(pid);
        appPath.replace("\\", "\\\\");
        editedPath = appPath;
        editedPath.replace(appName, editedName);

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
        this->accept();
    }
}

void DelayedOffDlg::setCountdown(const int &cntdown) {
    countdown = cntdown;
}
