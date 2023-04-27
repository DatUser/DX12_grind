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
            sErrMsg << std::endl;                                                           \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }

#define LOG_SHADER_COMPILE_ERROR(spErrorBuffer)                                             \
    {                                                                                       \
        std::stringstream ss;                                                               \
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());                                    \
        _com_error err(hr);                                                                 \
        const char* sErrMsg = (spErrorBuffer) ?                                             \
            (const char*)spErrorBuffer->GetBufferPointer() : "";                            \
                                                                                            \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            "Win Error: " << err.ErrorMessage() << std::endl <<                             \
            "Compile Error: " << sErrMsg << std::endl;                                      \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
    }

class DXG
{
public:
    DXG(HWND hWnd);
    DXG(const DXG& oDxg) = delete;
    ~DXG() = default;

    void PresentFrame();
    void ClearRenderView(float r, float g, float b, float a = 1.f);

    Microsoft::WRL::ComPtr<ID3D10Blob> compileShader(
        LPCWSTR pFilename,
        LPCSTR pEntryPoint,
        LPCSTR pShaderModel,
        const D3D10_SHADER_MACRO* pDefines = nullptr);

    void DrawHelloTriangle();

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_spDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_spSwapchain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_spContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_spTarget;
};