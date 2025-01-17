#pragma once

class RHIBuffer;

/**
 *  This is the rendering interface
 *  Its main purpose is to request frames
 *  and execute command lists
*/
class RHI
{
public:
    ~RHI() = default;

    /**
     *  Present frame to user
    */
    virtual HRESULT PresentFrame() = 0;

    virtual void ClearRenderView() = 0;
    virtual void Draw() = 0;

    virtual HRESULT CreateBuffer(void* pData, UINT uByteWidth, RHIBuffer* pBuffer)= 0;
    virtual HRESULT CreateSwapchain()= 0;

private:

};