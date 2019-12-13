#pragma once

#include <iostream>
#include <string>
#include <chrono>

/**
 * Simple timer class. Activated/Deactived using start() and end() methods, respectively.
 */
class Timer
{
private:
    using Clock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<std::chrono::system_clock,std::chrono::system_clock::duration>;

    std::string _label;
    TimePoint _begin;

public:
    Timer(std::string label);

    /**
     * Starts time measurement / resets existing time measurement.
     */
    void start();

    /**
      * This method measures the time since the
      * timer was started.
      */
    void end();

    void setLabel(std::string label);
};
