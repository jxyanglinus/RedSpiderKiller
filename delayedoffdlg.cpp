#include "delayedoffdlg.h"

DelayedOffDlg::DelayedOffDlg(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f) {
    readInfo();
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
}

void DelayedOffDlg::readInfo() {
    if (!QFile::exists(storeFileName)) {
        this->reject();
    } else {
        QFile file(storeFileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        appPath = "", editedPath = "";
        int cnt = 0;
        while (!file.atEnd()) {
            QString curLine = file.readLine();
            cnt++;
            if (cnt == 1) appPath = curLine.toUtf8();
            else if (cnt == 2) editedPath = curLine.toUtf8();
        }
        file.close();
        if (appPath == "" && editedPath == "") {
            this->reject();
        }
    }
}
