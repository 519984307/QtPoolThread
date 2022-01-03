#include "test.h"

TestConsumer::TestConsumer(QObject *parent):
 QObject(parent)
{
    threadWorker = new QThread(this);
    worker = new PoolThread();
    worker->moveToThread(threadWorker);
    qRegisterMetaType < std::function<QVariant(QVariant)> > ( "std::function<QVariant(QVariant)>" );
    connect(this, &TestConsumer::addTask_, worker, &PoolThread::addTask_, Qt::DirectConnection);
    connect(this, &TestConsumer::startWork, worker,&PoolThread::init_,Qt::DirectConnection);
    connect(threadWorker, &QThread::finished, worker, &QObject::deleteLater);
    threadWorker->start();
}


TestConsumer::~TestConsumer(){
    threadWorker->quit();
    threadWorker->wait();
}



long double TestConsumer::fact(int N)
{
    long double result;
    if(N < 0) {
        result = 0;
    }
    if (N == 0) {
        result = 1;
    }
    else {
        result =  N * fact(N - 1);
    }
    return result;
}

QVariant TestConsumer::testFactorial(const QVariant &N){
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 ));
    int n = N.toInt();
    long double k = fact(n);
    std::string res_s = "factorial of " + std::to_string( n) + ": " + std::to_string( k);
    QString qs = res_s.c_str();
    QVariant qv(qs);
    std::cout << res_s << std::endl;
    return qv;
}

void TestConsumer::numberTasks(const int &value){
    numTasks = value;
}

void TestConsumer::startTest(const int &value){

    emit startWork(value);
    for (int i = 0; i < numTasks; i++){
        const std::function<QVariant(QVariant)> f_ = &TestConsumer::testFactorial;
        emit addTask_(f_, QVariant(i + 42));
    }

    worker->deinit_();
    while(worker->numThreadsFinished_ != worker->numThreads_){
        QThread::usleep(1000);
    }
    emit setResult(worker->result_());

//    auto *timer_one = new Timer();
//    timer_one->timeStart();
//    for (int i = 0; i < number_task; i++){
//        testFactorial(i + 42);
//    }
//    std::cout << timer_one->timeEnd("With one thread ") << std::endl;
}
