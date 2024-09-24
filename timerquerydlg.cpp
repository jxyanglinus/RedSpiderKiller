#include "timerquerydlg.h"

TimerQueryDlg::TimerQueryDlg(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    this->setWindowTitle("填入定时秒数");

    QPushButton *confirm = new QPushButton("确认");
    QSpinBox *spinBox = new QSpinBox;
    QHBoxLayout *layout = new QHBoxLayout;

    spinBox->setRange(30, 3600);
    spinBox->setFont(QFont("黑体", 10));
    confirm->setFont(QFont("黑体", 10));

    void (QSpinBox::*spinBoxSignal)(int) = &QSpinBox::valueChanged;
    connect(spinBox, spinBoxSignal, this, &TimerQueryDlg::setCountdown);
    connect(confirm, &QPushButton::clicked, this, &TimerQueryDlg::accept);

    spinBox->setValue(60);

    layout->addWidget(spinBox);
    layout->addWidget(confirm);
    this->setLayout(layout);
}

void TimerQueryDlg::setCountdown(int value) {
    countdown = value;
}

int TimerQueryDlg::getCountdown() {
    return countdown;
}
