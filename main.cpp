
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(
        "QPushButton {"
        " min-width: 80px;"
        " min-height: 32px;"
        " padding: 6px 18px;"
        " margin: 0 8px;"
        "}"
    );
    MainWindow w;
    w.show();
    return a.exec();
}
