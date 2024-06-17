#pragma once

#include <array>
#include <string_view>

#include "D3D11/D3D11Interface.h"
#include "window.h"

#ifndef GFX_INTERFACE
#define GFX_INTERFACE "DX11"
#endif

class Camera;
class RHI;

typedef RHI* (*RHICreationInterface)(HWND d, Camera* pCam);
typedef std::array<std::pair<std::string_view, RHICreationInterface>, 1> ConstExprMap;

constexpr ConstExprMap mapInterfaces{
    {
        {"DX11", D3D11Interface::CreateInterface}
    }
};

constexpr RHICreationInterface find_else_default(const std::string_view& sKey)
{
    for (const auto& oPair : mapInterfaces)
    {
        if (oPair.first == sKey)
            return oPair.second;
    }

    return D3D11Interface::CreateInterface;
}

constexpr RHICreationInterface CreateInterface = find_else_default(GFX_INTERFACE);