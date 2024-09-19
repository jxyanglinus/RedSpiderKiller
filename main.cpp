#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    QPixmap pixmap = QPixmap(":/images/pictures/bk").scaled(window.size()); // 贴张背景图 :)
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    window.setPalette(palette);
    window.show();
    return app.exec();
}
