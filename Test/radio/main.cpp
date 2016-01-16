#include <QCoreApplication>
#include "../../Radio/Tx.h"

Tx tx;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    tx.setup();
    tx.onNewCommand("s l 0 i0");
    tx.onNewCommand("s l 1 i1");
    tx.onNewCommand("s l 2 i2");
    tx.onNewCommand("s l 3 i3");
    tx.onNewCommand("s l 4 i4[0;512]+i5[512;0]");

    //tx.onNewCommand("d l");
    tx.onNewCommand("v");

    return a.exec();
}

