#include <QCoreApplication>
#include <QDebug>
#include "mainclass.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainClass mainClass;
    return a.exec();
}
