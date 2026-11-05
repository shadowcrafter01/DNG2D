#ifndef DNG_STOPWATCH_HPP
#define DNG_STOPWATCH_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>
#include <atomic>

namespace DNG
{

    class StopwatchAsync
    {
    private:
        std::function<void()> function;
        std::thread thread;
        bool active = true;

    public:
        StopwatchAsync(std::function<void()> f, int interval_ms = 0)
        {
            function = ([f, interval_ms, this]()
                        {
                double timeLast=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
                while (active) 
                {
                    //auto start = std::chrono::steady_clock::now();
                    double currentTime=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
                    if (currentTime-timeLast>=interval_ms)
                    {
                        f(); // Execute the function
                        tps=1000/(currentTime-timeLast);
                        timeLast=currentTime;

                    }
                        //std::this_thread::sleep_until(start + std::chrono::milliseconds(interval_ms));
                } });
            thread = std::thread(function);
        }
        ~StopwatchAsync()
        {
            active = false;
            thread.join();
        }
        /*
        StopwatchAsync(std::function<void()> f)
        {
            function=([f,this]() {
                while (true)
                {
                    f();
                }
            });
            thread=std::thread(function);
        }
        */
        double tps;
    };

    class Stopwatch
    {
    private:
        std::function<void()> function;

        double timeLast = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        double timeCurrent;

    public:
        Stopwatch(std::function<void()> f, int interval_ms) : function{f},
                                                              interval{interval_ms}
        {
            update();
        }
        void update()
        {
            timeCurrent = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if (timeCurrent - timeLast >= interval)
            {
                function();
                timeLast = timeCurrent;
            }
        }

        int interval;
    };

};

#endif