#pragma once

#include "event.h"

class Window;

enum EKeyEvent
{

};

/**
 * @brief
 *
 *  Window: Holds the event sender
 *  EKeyEvent: Contains event information
 */
class InputEvent : public Event<int, Window*, EKeyEvent>
{
public:
    InputEvent() = default;
    ~InputEvent() = default;

private:
};