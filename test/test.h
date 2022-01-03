#pragma once
#include "poolthread.h"
#include <iostream>
#include "timer.h"
#include <QThread>
#include <QVariant>

class TestConsumer : public QObject {
    Q_OBJECT
public:

    explicit TestConsumer(QObject *parent = 0);

    ~TestConsumer();



private:
    QThread *threadWorker;

    PoolThread *worker;

    static long double fact(int N);

    static QVariant testFactorial(const QVariant &N);
    int numberThreads{1};
    int numTasks{20};

public slots:
    void startTest(const int &value);
    void numberTasks(const int &value);

signals:

    void addTask_(const std::function<QVariant(QVariant)> &func, const QVariant &args);

    void startWork(const int &N);

    void setResult(const std::string &str);

};
