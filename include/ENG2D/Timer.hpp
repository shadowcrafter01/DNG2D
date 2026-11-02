#ifndef ENG_TIMER_HPP
#define ENG_TIMER_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include <thread>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace ENG
{

    class Timer
    {
    private:
        void _Run()
        {
            // Capture the steady_clock epoch once, then measure offsets
            const auto start = std::chrono::steady_clock::now();

            while (_running.load(std::memory_order_acquire))
            {
                auto elapsed = std::chrono::steady_clock::now() - start;
                uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
                _currentNs.store(ns, std::memory_order_release);
            }
        }

        double _timeLast = 0;
        std::atomic<bool> _running;
        std::atomic<uint64_t> _currentNs;
        std::thread _thread;
        double _delta;
        double _fps;

    public:
        Timer() : _running(true),
                  _currentNs(0),
                  _fps(0)
        {
            _thread = std::thread(&Timer::_Run, this);
        }

        ~Timer()
        {
            Stop();
        }

        // Non-copyable, non-movable
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;

        // Returns the latest captured nanosecond count since epoch of steady_clock
        double GetNanoSecond()
        {
            return _currentNs.load(std::memory_order_acquire);
        }
        double GetMilliSecond()
        {
            return _currentNs.load(std::memory_order_acquire) / 1000000.0f;
        }
        double GetSecond()
        {
            return _currentNs.load(std::memory_order_acquire) / 1000000000.0f;
        }
        double GetDelta()
        {
            return _delta;
        }
        double GetFPS()
        {
            return _fps;
        }

        void Update()
        {
            double now = GetNanoSecond();
            _delta = (now - _timeLast) / 1000000000.0f;
            _timeLast = now;
            if (_delta == 0)
            {
                _fps = 0;
            }
            else
            {
                _fps = 1.0 / _delta;
            }
        }

        // Stop the updating thread
        void Stop()
        {
            bool expected = true;
            if (_running.compare_exchange_strong(expected, false))
            {
                if (_thread.joinable())
                {
                    _thread.join();
                }
            }
        }
    };

};

#endif