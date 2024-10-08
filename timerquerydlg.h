#ifndef TIMERQUERYDLG_H
#define TIMERQUERYDLG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

class TimerQueryDlg : public QDialog {
    Q_OBJECT
public:
    TimerQueryDlg(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setCountdown(int value);
    int getCountdown();
private:
    int countdown = 0;
};

#endif // TIMERQUERYDLG_H
