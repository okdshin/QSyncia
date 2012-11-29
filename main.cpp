#include "mainwindow.h"
#include <QApplication>

#include "syncia/SynciaEnvironment.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto exec_code = a.exec();

    return exec_code;
}
