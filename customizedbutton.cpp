#include "customizedbutton.h"

CustomizedButton::CustomizedButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
    this->setFont(QFont("黑体", 20));
}
