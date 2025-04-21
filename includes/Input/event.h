#pragma once

#include <vector>

template<typename RetType, typename... ParamsType>
class Event
{
public:
    Event();
    ~Event();

    virtual void AddAction(RetType (*)(ParamsType...));
    virtual void Broadcast(ParamsType... params);
    virtual void RemoveAction(RetType (*)(ParamsType...));

protected:
    std::vector<RetType (*)(ParamsType...)> m_vCallbacks;
};

#include "event.hpp"
