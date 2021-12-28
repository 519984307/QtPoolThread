#include "poolthreadtaskingqt.h"
#include "test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestMultipleThreads test;
    test.testF(5);

    return a.exec();
}
