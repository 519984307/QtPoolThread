#include "poolthread.h"

PoolThread::PoolThread(QObject *parent):
 QObject(parent)
{
    timer_ = new Timer();
}


PoolThread::~PoolThread(){
   stopThreads_();
   threads_.clear();
}


void PoolThread::init_(const int &numberThreads) {

    std::cout << "reset for thread pool" << std::endl;
    numThreadsFinished_ = 0;
    initialized_ = false;
    timer_->timeStart();
    int nnum = numberThreads;
    numThreads_ = numberThreads;
    // Каждый поток должен выполняться в своей функции с бесконечным циклом, постоянно ожидая захвата и запуска новых задач
    while((nnum--)>0) {
        ThreadWorker *worker = new ThreadWorker(this);
        QThread* thread = new QThread;
        worker->moveToThread(thread);
        worker->id_ = nnum;
        threads_.push_back(thread);

        connect(thread, &QThread::started, worker, &ThreadWorker::loopFunc_);
        connect(worker, &ThreadWorker::stop, thread,  &QThread::quit );
        thread->start();
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    }
}


void PoolThread::addTask_(const std::function<QVariant(QVariant)> &func, const QVariant &args){
    QMutexLocker locker(&mutex_);
    std::cout << "Added new task " << std::endl;
    auto tsk = std::bind(func, args);
    mQueue_.enqueue(tsk);
    cond_.notify_one(); // Разблокирует один из потоков, которые ожидают объект condition_variable.
    initialized_ = true;
}




void PoolThread::PoolThread::ThreadWorker::finishWorker(){
    emit stop();
}

void PoolThread::PoolThread::ThreadWorker::loopFunc_(){
    /// если очередь пуста, то ждем
    /// берем задачу из потока
    /// удаляем задачу из очереди
    /// выполняем
    forever{
        std::function<QVariant()> function = nullptr;

        {
//             QMutexLocker locker(&m_pool->mutex_);
             m_pool->mutex_.lock();
             if (m_pool->finishedJob_ && m_pool->mQueue_.empty() && m_pool->initialized_)
             {
                 m_pool->numThreadsFinished_++;
                 std::cout << this->id_ <<" finished " << std::endl;
                 emit finishWorker();
                 return;
             }

            if (m_pool->mQueue_.empty()) {
                m_pool->cond_.wait(&m_pool->mutex_);
            }
            if (!m_pool->mQueue_.empty()){
                std::cout << this->id_ << " took job" << std::endl;
                function = std::move(m_pool->mQueue_.front());
                m_pool->mQueue_.dequeue();
            }
            m_pool->mutex_.unlock();
        }

        function();
    }
}

PoolThread::ThreadWorker::ThreadWorker(PoolThread *pool, QObject *parent) :
 QObject(parent){
    m_pool = pool;
}

PoolThread::ThreadWorker::~ThreadWorker(){
}

void PoolThread::deinit_(){
    finishedJob_ = true;
}

std::string PoolThread::result_(){
    std::cout << "RESULT TIME " << std::endl;
    while(numThreadsFinished_ != numThreads_){
        QThread::usleep(1000);
    }
    std::string res = "With " + std::to_string(numThreads_) + " threads ";
    return timer_->timeEnd(res);

}



void PoolThread::stopThreads_()  /* принудительная остановка всех потоков */
{
    cond_.notify_all(); // Разблокирует все потоки, которые ожидают объект condition_variable.
    std::cout << "stopThreads" <<std::endl;
    for (QThread *th : threads_)
    {
        th->quit();
        th->wait();
    }
}
