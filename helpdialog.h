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
                             "若程序无法正常运行，请以管理员身份打开。\n\n"
                             "本程序的原理非常简单：找到红蜘蛛的位置，终止红蜘蛛进程，\n"
                             "然后更改它的名称来防止它极其恶心的自动重启。\n"
                             "同时会存储红蜘蛛的路径，以便于在必要时恢复红蜘蛛。\n"
                             "另外，本次更新带来全新功能：定时关闭！\n"
                             "被控制一段时间以规避老师检查，等时间一到就自动解除控制。";
};

#endif // HELPDIALOG_H
