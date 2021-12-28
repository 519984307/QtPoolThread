#pragma once
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include <QSemaphore>
#include <QQueue>
#include <QMutexLocker>

class PoolThreadTasking : public QObject {
    Q_OBJECT
public:

    explicit PoolThreadTasking(int &numberThreads,QObject *parent = 0);

    ~PoolThreadTasking();
   ///< завершение работы потоков
   void clear_();

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

private:
    class ThreadWorker : public QThread {
       private:
           PoolThreadTasking * m_pool;
       public:
           explicit ThreadWorker(PoolThreadTasking * pool,QObject *parent = 0);
           void loopFunc();
       };

///< пул потоков
   std::vector<ThreadWorker> threads_;
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
