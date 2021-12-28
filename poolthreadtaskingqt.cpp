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
        QThread* thread = new QThread;
        worker->moveToThread(thread);
        threads_.push_back(worker);

        connect(thread, &QThread::started, worker, &ThreadWorker::loopFunc);
        connect(worker, &ThreadWorker::finished, thread,  &QThread::quit );

//        connect(this, SIGNAL(stopAll()), worker, SLOT(stop()));

//        connect(this, &PoolThreadTasking::finished, worker, &ThreadWorker::finish_worker);

        thread->start();
    }
}


void PoolThreadTasking::PoolThreadTasking::ThreadWorker::finish_worker(){
//    if (m_pool->m_queue.empty())
        emit finished();

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
             QMutexLocker locker(&m_pool->mutex_);
//             m_pool->mutex_.lock();

            if (m_pool->m_queue.empty()) {
                /// ждать пока очередь пуста или пока не закончится работа..
//                m_pool->cond_.wait(locker,
//                                     [this]{ return m_pool->finishedJob_ || !m_pool->m_queue.empty(); });
                m_pool->cond_.wait(&m_pool->mutex_);

//                m_pool->cond_.wait(lock); // Блокирует поток
            }
            if (m_pool->finishedJob_ && m_pool->m_queue.empty())
            {
                std::cout << "finished job " <<std::endl;
                emit finish_worker();
                return;
            }

            function = std::move(m_pool->m_queue.front());
            m_pool->m_queue.dequeue();
//            m_pool->mutex_.unlock();
        }
        function();
    }
}

PoolThreadTasking::ThreadWorker::ThreadWorker(PoolThreadTasking *pool, QObject *parent) :
 QObject(parent){
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
    for (ThreadWorker *th : threads_)
    {
        th->finished();
    }

    threads_.clear();
}

void PoolThreadTasking::stopThreads()  /* принудительная остановка всех потоков */
{
    emit  stopAll();
/* каждый RBWorker получит сигнал остановиться, остановит свой построитель отчетов и вызовет слот quit() своего потока */
}
