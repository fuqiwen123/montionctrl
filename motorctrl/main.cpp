#include "mainwindow.h"
#include "Dmc1000.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w(d1000_board_init());
    w.show();

    int nRet = a.exec();
    d1000_board_close();
    return nRet;
}
