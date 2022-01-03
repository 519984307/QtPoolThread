/*! \file timer.h
 * \brief Timer class
 *
 * Вычисление времени исполнения
 *
 * \author Alyona Fomina
 * \date September 2021
 */
#pragma once
#include <string>
#include <iostream>
#include <chrono>

class Timer
{
public:
    Timer();
    std::chrono::steady_clock::time_point end;
    std::chrono::steady_clock::time_point begin;
    void timeStart();
    std::string timeEnd(const std::string &name_timer);
};