#pragma once


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

    void*				m_pData;
    ERHICPUAccessFlags	m_eCPUAccess;
    ERHIBufferUsage		m_eUsage;
};