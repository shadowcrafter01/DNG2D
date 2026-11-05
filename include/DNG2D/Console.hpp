#ifndef DNG_CONSOLE_HPP
#define DNG_CONSOLE_HPP

#include <iostream>
#include <cmath>
#include <string>
#include <SDL3/SDL.h>
// #include <Timer.hpp>
#include <DNG2D/Timer.hpp>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <thread>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace DNG
{

    class Console
    {
    private:
        const inline static auto start = std::chrono::steady_clock::now();

        static double GetCurrentMS()
        {
            auto elapsed = std::chrono::steady_clock::now() - start;
            uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
            currentMS = ns / 1000000.0;
            return currentMS;
        }

        static std::string reportCurrentMS()
        {
            double ms = GetCurrentMS();
            std::string time = std::to_string(std::round(ms * 100) / 100);
            std::string buffer = "";
            int decimal = 0;
            for (char i : time)
            {
                if (i == '.' || decimal > 0)
                    decimal++;
                if (decimal > 3)
                    break;
                buffer = buffer + i;
            }

            for (int i = buffer.length(); i < 8; ++i)
            {
                buffer = " " + buffer;
            }

            return "[" + buffer + "]";
        }

        // inline static Timer *timer;
        inline static double timeStore;
        inline static double currentMS;

    public:
        static void Log(std::string message, std::string label = "")
        {
            std::cout << label << message << "\n";
        }

        static void LogInfo(std::string message, std::string label_override = " -INFO : ")
        {
            std::cout << reportCurrentMS() << label_override << message << "\n";
        }

        static void LogDebug(double message, std::string label_override = " -DEBUG: ")
        {
            std::cout << reportCurrentMS() << label_override << message << "\n";
        }

        static void LogWarn(std::string message)
        {
            std::cout << reportCurrentMS() << " -WARN : " << message << "\n";
        }

        static void LogError(std::string message = "", std::string error = SDL_GetError())
        {
            std::cout << reportCurrentMS() << " -ERROR: " << message << " -> " << error << "\n";
        }

        static void LogLoadStart(std::string message)
        {
            std::cout << reportCurrentMS() << " -LOAD : " << message << "... ";
            timeStore = currentMS;
        }
        static void LogLoadEnd(bool success, std::string follow_up = "")
        {
            if (success)
            {
                if (follow_up == "")
                {
                    std::cout << "Success in " << std::to_string(GetCurrentMS() - timeStore) << "ms\n";
                }
                else
                {
                    std::cout << "Success in " << std::to_string(GetCurrentMS() - timeStore) << "ms -> " << follow_up << "\n";
                }
            }
            else
            {
                if (follow_up == "")
                {
                    std::cout << "Failed in " << std::to_string(GetCurrentMS() - timeStore) << "ms -> " << SDL_GetError() << "\n";
                }
                else
                {
                    std::cout << "Failed in " << std::to_string(GetCurrentMS() - timeStore) << "ms -> " << follow_up << " -> " << SDL_GetError() << "\n";
                }
            }
        }
    };
    // static Console console;

};

#endif