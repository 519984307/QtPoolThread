#include "test.h"


long double TestMultipleThreads::fact(int N)
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

void TestMultipleThreads::testFactorial(int N){
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 ));
    long double k = fact(N);
    std::cout << "result: " << k << std::endl;
}

void TestMultipleThreads::testF(const int &number_threads){
    unsigned int cores_available = std::thread::hardware_concurrency() - 1;
    int numThread_ = (cores_available != 0 && number_threads < cores_available ) ? number_threads :cores_available  ;
    if (numThread_ < 1) numThread_ = 1;
    std::cout << "Selected " <<  numThread_ << " threads"<< std::endl;

    PoolThreadTasking pTD(numThread_);

    int number_task = 5;
    auto *timer_one = new Timer();
    timer_one->timeStart();
    for (int i = 0; i < number_task; i++){

        pTD.addTask_(testFactorial, i + 42);

    }

    pTD.clear_();
    std::cout << timer_one->timeEnd("With multiple threads ") << std::endl;

    timer_one->timeStart();
    for (int i = 0; i < number_task; i++){
        testFactorial(i + 42);
    }
    std::cout << timer_one->timeEnd("With one thread ") << std::endl;
}
