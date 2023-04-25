#pragma once

#include "window.h"
#include <d3d11.h>
#include <dxgi.h>

#include <wrl/client.h>

#define GET_LAST_GRAPHICS_ERROR(hr)                                                         \
    {                                                                                       \
        std::stringstream ss;                                                               \
        auto sErrMsg = (hr == DXGI_ERROR_DEVICE_REMOVED) ?                                  \
            m_pDevice->GetDeviceRemovedReason() : "";                                       \
                                                                                            \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
             << std::endl;                                                                  \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }

class DXG
{
public:
    DXG(HWND hWnd);
    DXG(const DXG& oDxg) = delete;
    ~DXG() = default;

    void PresentFrame();
    void ClearRenderView(float r, float g, float b, float a = 1.f);

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapchain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
};