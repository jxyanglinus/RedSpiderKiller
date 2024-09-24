#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLayout>

// 帮助信息对话框
class HelpDialog : public QDialog {
    Q_OBJECT
public:
    HelpDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    const QString helpText = "一个简单的小程序，用于解除红蜘蛛电子教室控制！\n\n"
                             "若程序无法正常运行，请以管理员身份打开。";
};

#endif // HELPDIALOG_H
