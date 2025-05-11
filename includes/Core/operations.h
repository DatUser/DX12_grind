#pragma once

#include <cmath>
#include <cstdint>

constexpr uint32_t GetFirstBitSet(uint32_t uVal)
{
    ATLASSERT(uVal != 0);

    uint32_t uResult = 0;

    while (!(uVal & 1))
        uResult++, uVal >>= 1;

    return uResult;
}