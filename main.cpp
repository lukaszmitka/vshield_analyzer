#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //TODO remove before deploument
    a.addLibraryPath("C:/Qt/5.7/msvc2015_64/plugins");
    MainWindow w;
    w.show();
    return a.exec();
}
