#pragma once
#include "poolthreadtaskingqt.h"
#include <iostream>
#include "timer.h"

class TestMultipleThreads

{
public:
    TestMultipleThreads() = default;

    void testF(const int &number_threads);

    static long double fact(int N);

    static void testFactorial(int N);

};
