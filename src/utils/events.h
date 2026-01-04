#ifndef EVENTS_H
#define EVENTS_H

#include <functional>
#include <vector>

template <typename... Args>
class Signal
{
private:
    struct connexion
    {
        size_t id;
        std::function<void(Args...)> callback;
    };

    std::vector<connexion> connexions;
    size_t nextId = 0;

public:
    // connexion handle that can be used to disconnect
    class Handle
    {
    private:
        size_t id;
        Signal *signal;
        friend class Signal;

    public:
        Handle() : id(0), signal(nullptr) {}
        Handle(size_t id, Signal *sig) : id(id), signal(sig) {}

        // Disconnect this specific connexion
        void disconnect()
        {
            if (signal)
            {
                signal->disconnectById(id);
                signal = nullptr;
            }
        }

        bool isValid() const { return signal != nullptr; }
    };

    Handle connect(std::function<void(Args...)> func)
    {
        size_t id = nextId++;
        connexions.push_back({id, func});
        return Handle(id, this);
    }

    void disconnectById(size_t id)
    {
        for (auto it = connexions.begin(); it != connexions.end(); ++it)
        {
            if (it->id == id)
            {
                connexions.erase(it);
                return;
            }
        }
    }

    void disconnectAll()
    {
        connexions.clear();
    }

    void emit(Args... args)
    {
        for (auto &conn : connexions)
        {
            if (conn.callback)
            {
                conn.callback(args...);
            }
        }
        Serial.println("Signal emitted");
    }
};

#endif