#pragma once

#include <array>

#include "Core/Core.h"

enum class ERHICPUAccessFlags : uint8_t
{
    NONE,
    READ,
    WRITE,
    READ_WRITE,

    _size
};

enum class ERHIBufferUsage : uint8_t
{
    DEFAULT,
    DYNAMIC,

    _size
};

class RHIResource
{
protected:
    RHIResource() = delete;
    RHIResource(
        void* pData,
        ERHICPUAccessFlags eCPUAccess,
        ERHIBufferUsage eUsage
    )
    : m_pData(pData)
    , m_eCPUAccess(eCPUAccess)
    , m_eUsage(eUsage)
    {}

    virtual ~RHIResource() = default;

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

    void*				m_pData;
    ERHICPUAccessFlags	m_eCPUAccess;
    ERHIBufferUsage		m_eUsage;
};