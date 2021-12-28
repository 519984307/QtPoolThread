#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include <QSemaphore>
#include <QQueue>
#include <QMutexLocker>
#include <iostream>



class PoolThreadTasking : public QObject {
    Q_OBJECT
public:

    explicit PoolThreadTasking(int &numberThreads,QObject *parent = 0);

    ~PoolThreadTasking();
   ///< завершение работы потоков
   void clear_();

   void stopThreads();

   /*! \fn void addTask_();
    * \brief функция добавления задач(функций) в очередь
    */
   template<class F, class... Args>
   void addTask_(F&& f, Args&&... args){
       auto task = std::make_shared< std::packaged_task<void()> >(
               std::bind(std::forward<F>(f), std::forward<Args>(args)...) );
       QMutexLocker locker(&mutex_);

       m_queue.enqueue([task](){ (*task)(); });

       cond_.notify_one(); // Разблокирует один из потоков, которые ожидают объект condition_variable.
   }
signals:
   void finished();
   void stopAll();

private:
   class ThreadWorker;


///< пул потоков
   std::vector<ThreadWorker*> threads_;
   ///< для выхода из бесконечного цикла
   bool finishedJob_{false};
   ///< очередь для выполнения функций
   QQueue<std::function<void()>> m_queue;

   QMutex mutex_;

   ///< condition_variable используется для ожидания события при наличии mutex типа unique_lock<mutex>.
   QWaitCondition cond_;

   ///< инициализация потоков
   void init_(int &numberThreads);
};



class PoolThreadTasking::ThreadWorker : public QObject {
 Q_OBJECT
   public:
       explicit ThreadWorker(PoolThreadTasking * pool,QObject *parent = 0);
       void loopFunc();

    public slots:

       void finish_worker();
    public :
signals:
        void finished();
private:
    PoolThreadTasking * m_pool;
};
