#pragma once

#include "event.h"

class Window;

/**
 * @brief
 *
 *  Window: Holds the event sender
 *  EKeyEvent: Contains event information
 */
//TODO add a controller class
class MoveEvent : public Event<void, Window*, int, int>
{
public:
    MoveEvent() = default;
    ~MoveEvent() = default;

private:
};