#ifndef DNG_RUNONCE_HPP
#define DNG_RUNONCE_HPP

#include <vector>
#include <functional>

namespace DNG
{

    class RunOnce
    {
    private:
        bool flag_onTrue = true;
        bool flag_onFalse = false;
        std::vector<std::function<void()>> functions;

    public:
        RunOnce(std::function<void()> f)
        {
            AssignFunction(f);
        }

        void AssignFunction(std::function<void()> f)
        {
            functions.push_back(f);
        }

        bool OnTrue(bool condition)
        {
            if (condition)
            {
                if (flag_onTrue)
                {
                    flag_onTrue = false;
                    for (auto f : functions)
                    {
                        f();
                    }
                    return true;
                }
                return false;
            }
            else
            {
                flag_onTrue = true;
            }
            return false;
        }
        bool OnFalse(bool condition)
        {
            if (!condition)
            {
                if (flag_onFalse)
                {
                    flag_onFalse = false;
                    for (auto f : functions)
                    {
                        f();
                    }
                    return true;
                }
                return false;
            }
            else
            {
                flag_onFalse = true;
            }
            return false;
        }
    };

};

#endif