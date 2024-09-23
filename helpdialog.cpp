#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *label = new QLabel;

    layout->addWidget(label);

    this->setLayout(layout);
    this->setWindowTitle("帮助");

    label->setFont(QFont("黑体", 12));
    label->setText(helpText);
}
