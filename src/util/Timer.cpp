#include "Timer.h"

Timer::Timer(std::string label)
    : _label(label)
{
    start();
}

void Timer::start()
{
    _begin = Clock::now();
}

/**
     * This method measures the time since the
     * timer was started.
     */
void Timer::end()
{
    TimePoint end = Clock::now();

    std::cout << _label << " \t"
                << std::chrono::duration_cast<std::chrono::milliseconds>(end - _begin).count()
                << " ms" << std::endl;
}

void Timer::setLabel(std::string label)
{
    _label = label;
}