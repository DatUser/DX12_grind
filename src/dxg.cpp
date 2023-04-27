#include "dxg.h"

#include <atldef.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")

namespace wrl = Microsoft::WRL;

DXG::DXG(HWND hWnd)
: m_spDevice(nullptr)
, m_spSwapchain(nullptr)
, m_spContext(nullptr)
{
    DXGI_SWAP_CHAIN_DESC oSwapDesc;
    ZeroMemory(&oSwapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    oSwapDesc.BufferDesc.Width = 0;
    oSwapDesc.BufferDesc.Height = 0;
    oSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    oSwapDesc.BufferDesc.RefreshRate.Numerator = 0;
    oSwapDesc.BufferDesc.RefreshRate.Denominator = 0;
    oSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    oSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    oSwapDesc.SampleDesc.Count = 1;
    oSwapDesc.SampleDesc.Quality = 0;
    oSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    oSwapDesc.BufferCount = 1;
    oSwapDesc.OutputWindow = (HWND) hWnd;
    oSwapDesc.Windowed = TRUE;
    oSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    oSwapDesc.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        //Here pointer is released then we access its address
        &oSwapDesc,
        &m_spSwapchain,
        &m_spDevice,
        nullptr,
        &m_spContext
    );
    ATLASSERT(hr == S_OK);

    // Access to swapchain back buffer resource
    wrl::ComPtr<ID3D11Resource> spBackBuffer;
    hr = m_spSwapchain->GetBuffer(0, __uuidof(ID3D11Resource), &spBackBuffer);
    ATLASSERT(hr == S_OK);

    hr = m_spDevice->CreateRenderTargetView(spBackBuffer.Get(), nullptr, &m_spTarget);
    ATLASSERT(hr == S_OK);

    compileShader(nullptr, nullptr, nullptr);
}

void DXG::PresentFrame()
{
    HRESULT hr = m_spSwapchain->Present(1, 0);
    ATLASSERT(hr == S_OK);
}

void DXG::ClearRenderView(float r, float g, float b, float a)
{
    float pColor[] = {r, g, b, a};
    m_spContext->ClearRenderTargetView(m_spTarget.Get(), pColor);
}

wrl::ComPtr<ID3D10Blob> DXG::compileShader(
        LPCWSTR pFilename,
        LPCSTR pEntryPoint,
        LPCSTR pShaderModel,
        const D3D10_SHADER_MACRO* pDefines)
{
    wrl::ComPtr<ID3D10Blob> spShaderBuffer;
    wrl::ComPtr<ID3DBlob> spErrorBuffer;

    HRESULT hr = D3DCompileFromFile(
        pFilename,
        pDefines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        pEntryPoint,
        pShaderModel,
        0,
        0,
        &spShaderBuffer,
        &spErrorBuffer
    );

    if (FAILED(hr))
    {
        LOG_SHADER_COMPILE_ERROR(spErrorBuffer);
    }

    return spShaderBuffer;
}

void DXG::DrawHelloTriangle()
{
    //m_pContext->IASetVertexBuffers(0, 1, )
}