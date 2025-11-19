#ifndef FUNCTIONAL_UTILS_H
#define FUNCTIONAL_UTILS_H
#include <functional>
#include <vector>
class Task
{
private:
    std::vector<std::function<void()>> taskFunction;
    bool completed = false;
    bool running = false;

public:
    Task() {}
    void run()
    {
        if (!running)
        {
            return;
        }

        auto func = taskFunction.at(taskFunction.size() - 1);
        taskFunction.pop_back();
        func();
    }
    void add_task(std::function<void()> func)
    {
        taskFunction.push_back(func);
    }
    void enable()
    {
        running = true;
    }
    void disable()
    {
        running = false;
    }
    bool isCompleted() const { return completed; }
};
#endif