#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon(":/img/clientIcon.ico"));
    w.setWindowTitle("Orders Client");
    w.show();
    return a.exec();
}
