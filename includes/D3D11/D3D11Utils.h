#pragma once

#include <array>

#include "Core/Core.h"

#include "RHI/rhi_resource.h"

static inline unsigned int CastToInterfaceCPUAccess(ERHICPUAccessFlags eCPUAccess)
{
    constexpr static std::array<unsigned int, static_cast<unsigned int>(ERHICPUAccessFlags::_size)> mapFlags{
        0,
        D3D11_CPU_ACCESS_READ,
        D3D11_CPU_ACCESS_WRITE,
        D3D11_CPU_ACCESS_READ|D3D11_CPU_ACCESS_WRITE
    };

    return mapFlags[static_cast<unsigned int>(eCPUAccess)];
}

static inline D3D11_USAGE CastToInterfaceUsage(ERHIBufferUsage eUsage)
{
    constexpr static std::array<D3D11_USAGE, static_cast<unsigned int>(ERHIBufferUsage::_size)> mapFlags{
        D3D11_USAGE_DEFAULT,
        D3D11_USAGE_DYNAMIC
    };

    return mapFlags[static_cast<unsigned int>(eUsage)];
}