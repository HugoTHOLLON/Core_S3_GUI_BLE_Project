#ifndef EVENTS_H
#define EVENTS_H

#include <functional>
#include <vector>

template <typename... Args>
class Signal
{
private:
    std::vector<std::function<void(Args...)>> callbacks;
    bool funcAreEqual(std::function<Args...> func1, std::function<Args...> func2)
    {
        // Compare types and actual function pointers
        return func1.target_type() == func2.target_type() &&
               func1.template target<void (*)(Args...)>() ==
                   func2.template target<void (*)(Args...)>()
    }

public:
    void connect(std::function<Args...> func)
    {
        if (!isConnected(func))
            callbacks.push_back(func);
    }

    bool isConnected(std::function<Args...> func)
    {
        for (const auto &callback : callbacks)
        {
            if (funcAreEqual(callback, func))
                return true;
        }
        return false;
    }

    bool hasConnexions() const
    {
        return !callbacks.empty();
    }

    void emit(Args... args)
    {
        for (auto &callback : callbacks)
        {
            if (callback)
                callback(args...);
        }
    }

    bool disconnect(std::function<Args...> func)
    {
        for (size_t i = 0; i < callbacks.size(); i++)
        {
            if (funcAreEqual(callbacks[i], func))
            {
                callbacks.erase(callbacks.begin() + i);
                return true;
            }
        }
        return false;
    }

    void disconnectAll()
    {
        callbacks.clear();
    }
};

#endif