#ifndef DNG_RANDOM_HPP
#define DNG_RANDOM_HPP

#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace DNG
{
    class Random
    {
    private:
    public:
        static void Init()
        {
            std::srand(std::time(nullptr));
        }
        static float GetNext()
        {
            return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        }
        static float GetNext(float min, float max)
        {
            return (GetNext() * (max - min)) + min;
        }

        operator float()
        {
            return GetNext();
        }
    };

};

#endif