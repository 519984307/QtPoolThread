#include "poolthread.h"
#include "test.h"
#include "vis.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestConsumer *test = new TestConsumer();
    Vis *vis = new Vis();
    QObject::connect(vis, &Vis::enabledStart, test, &TestConsumer::startTest);
    QObject::connect(test, &TestConsumer::setResult, vis, &Vis::setResult);
    vis->show();
    return a.exec();
}

