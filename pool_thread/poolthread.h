#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QMutexLocker>
#include <iostream>
#include <QVariant>

#include "timer.h"



class PoolThread : public QObject {
    Q_OBJECT
public:
    explicit PoolThread(QObject *parent = 0);

    ~PoolThread();

    std::string result_();
    void stopThreads_();
    int numThreads_{-1};
    int numThreadsFinished_{0};
    bool initialized_{false};

    Timer *timer_;

public slots:

    /*! \fn void addTask_();
     * \brief функция добавления задач(функций) в очередь
     */
    void addTask_(const std::function<QVariant(QVariant)> &func, const QVariant &args);

    ///< инициализация потоков
    void init_(const int &numberThreads);

    ///< деинициализация потоков
    void deinit_();

private:

   class ThreadWorker;

///< пул потоков
   std::vector<QThread*> threads_;

   ///< для выхода из бесконечного цикла
   bool finishedJob_{false};

   ///< очередь для выполнения функций
   QQueue<std::function<QVariant()>> mQueue_;

   QMutex mutex_;

   ///< condition_variable используется для ожидания события при наличии mutex типа unique_lock<mutex>.
   QWaitCondition cond_;
};



class PoolThread::ThreadWorker : public QObject {
 Q_OBJECT
public:
    explicit ThreadWorker(PoolThread * pool,QObject *parent = 0);
    ~ThreadWorker();
    void loopFunc_();
    int id_{-1};
private:
    PoolThread * m_pool;

public slots:

    void finishWorker();
public :
signals:
    void stop();
    void finished();

};
