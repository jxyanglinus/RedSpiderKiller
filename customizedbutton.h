#ifndef CUSTOMIZEDBUTTON_H
#define CUSTOMIZEDBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QObject>

class CustomizedButton : public QPushButton {
    Q_OBJECT
public:
    CustomizedButton(const QString &text, QWidget *parent = nullptr);
};

#endif // CUSTOMIZEDBUTTON_H
