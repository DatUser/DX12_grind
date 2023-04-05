#pragma once

#include "event.h"

class Window;

enum EInputType
{
    KEY_PRESSED,
    KEY_RELEASED,
    KEY_DOUBLE_CLICK
};

/**
 * @brief
 *
 *  Window: Holds the event sender
 *  EKeyEvent: Contains event information
 */
class InputEvent : public Event<void, Window*, unsigned int, EInputType>
{
public:
    InputEvent() = default;
    ~InputEvent() = default;

private:
};