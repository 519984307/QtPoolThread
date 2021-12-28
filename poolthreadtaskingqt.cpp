#include "poolthreadtaskingqt.h"

PoolThreadTasking::PoolThreadTasking(int &numberThreads, QObject *parent):
 QObject(parent)
{
    init_(numberThreads);

}


PoolThreadTasking::~PoolThreadTasking() = default;

void PoolThreadTasking::init_(int &numberThreads) {
    // Каждый поток должен выполняться в своей функции с бесконечным циклом, постоянно ожидая захвата и запуска новых задач
    while((numberThreads--)>0) {
        ThreadWorker *worker = new ThreadWorker(this);

        connect(worker, &ThreadWorker::started, worker,&ThreadWorker::loopFunc  );
//        threads_.push_back(worker);
        worker->start();
    }
}


void PoolThreadTasking::PoolThreadTasking::ThreadWorker::loopFunc(){
    /// если очередь пуста, то ждем
    /// берем задачу из потока
    /// удаляем задачу из очереди
    /// выполняем

    while (true)
    {
        std::function<void()> function;

        {
//             QMutexLocker locker(&m_pool->mutex_);
             m_pool->mutex_.lock();

            if (m_pool->m_queue.empty()) {
                /// ждать пока очередь пуста или пока не закончится работа..
//                m_pool->cond_.wait(locker,
//                                     [this]{ return m_pool->finishedJob_ || !m_pool->m_queue.empty(); });
                m_pool->cond_.wait(&m_pool->mutex_);

//                m_pool->cond_.wait(lock); // Блокирует поток
            }
            if (m_pool->finishedJob_ && m_pool->m_queue.empty())
            {
                return;
            }

            function = std::move(m_pool->m_queue.front());
            m_pool->m_queue.dequeue();
            m_pool->mutex_.unlock();
        }
        function();
    }
}

PoolThreadTasking::ThreadWorker::ThreadWorker(PoolThreadTasking *pool, QObject *parent) :
 QThread(parent){
    m_pool = pool;
}

void PoolThreadTasking::clear_()
{
//    QMutexLocker locker(&mutex_);
    mutex_.lock();
    finishedJob_ = true;
    mutex_.unlock();
    cond_.notify_all(); // Разблокирует все потоки, которые ожидают объект condition_variable.

    // Join all threads.
    for (ThreadWorker &th : threads_)
    {
//        th.finished();
//        th.finish();
//        if (th.joinable()) th.join();
    }

    threads_.clear();
}
