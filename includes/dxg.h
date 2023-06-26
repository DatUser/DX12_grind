#pragma once

#include "window.h"
#include <d3d11.h>
#include <dxgi.h>

#include <wrl/client.h>

#define GET_LAST_GRAPHICS_ERROR(hr)                                                         \
    {                                                                                       \
        std::stringstream ss;                                                               \
        auto sErrMsg = (hr == DXGI_ERROR_DEVICE_REMOVED) ?                                  \
            m_spDevice->GetDeviceRemovedReason() : S_OK;                                    \
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

enum EShaderStage
{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

class DXG
{
public:
    DXG(HWND hWnd);
    DXG(const DXG& oDxg) = delete;
    ~DXG() = default;

    void PresentFrame();
    void ClearRenderView(float r, float g, float b, float a = 1.f);

    /**
     * @brief
     *
     * @param pFilename
     * @param pEntryPoint
     * @param pShaderModel
     * @param pDefines
     * @return Smart Pointer to shader buffer on success, otherwise nullptr
     */
    static Microsoft::WRL::ComPtr<ID3D10Blob> compileShader(
        LPCWSTR pFilename,
        LPCSTR pEntryPoint,
        LPCSTR pShaderModel,
        const D3D10_SHADER_MACRO* pDefines = nullptr);

    /**
     * @brief Create a Shader Instance object
     *
     * @param pShaderBuffer
     * @param[out] pShaderInstance
     * @return HRESULT S_OK on sucess
     */
    HRESULT createShaderInstance(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage);

    /**
     * @brief Create a Buffer object in GPU memory
     *
     * @param pData
     * @param uByteWidth
     * @param opBuffer
     * @param uFlags
     * @return HRESULT
     */
    HRESULT createBuffer(void* pData, UINT uByteWidth, void** opBuffer, UINT uFlags = D3D11_BIND_VERTEX_BUFFER);

    /**
     * @brief Create a Input Layout containing information about buffer data layout
     *
     * @param pVSBuffer
     * @param eFormat
     * @param pName
     * @param pLayout
     * @return HRESULT
     */
    HRESULT createInputLayout(ID3D10Blob* pVSBuffer, DXGI_FORMAT eFormat, LPCSTR pName, void** pLayout);

    /**
     * @brief Adds buffer as input of given Vertex buffer
     *
     * @param pBuffers
     * @param nBuffers
     * @param pVSBuffer
     * @param uFlags
     */
    void AddBuffers(std::vector<ID3D11Buffer*> vBuffers, Microsoft::WRL::ComPtr<ID3D11InputLayout> spVertsLayout, UINT uStride, UINT uOffset);

    /**
     * @brief Inits the default shader program
     *
     */
    void InitTestScene();

    void DrawHelloTriangle();

private:
    // Handles memory stuff
    Microsoft::WRL::ComPtr<ID3D11Device> m_spDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_spSwapchain;
    // Handles gfx pipeline stuff
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_spContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_spTarget;
    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_vBuffers;
};