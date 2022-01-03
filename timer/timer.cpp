#include "timer.h"

#include <utility>

Timer::Timer()
{

}

void Timer::timeStart()
{
    begin = std::chrono::steady_clock::now();
}

std::string Timer::timeEnd(const std::string &name_timer)
{
    end = std::chrono::steady_clock::now();
    auto t_difference = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    auto t_difference_mc = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    std::string result =   name_timer +  " took : " +  std::to_string(t_difference_mc) + " [mcs].";
    return result;
}
