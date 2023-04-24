#include "dxg.h"

#include <atldef.h>

#pragma comment(lib, "d3d11.lib")

DXG::DXG(HWND hWnd)
: m_pDevice(nullptr)
, m_pSwapchain(nullptr)
, m_pContext(nullptr)
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
        &oSwapDesc,
        &m_pSwapchain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );
    ATLASSERT(hr == S_OK);

    // Access to swapchain back buffer resource
    ID3D11Resource* pBackBuffer = nullptr;
    hr = m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
    ATLASSERT(hr == S_OK);

    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTarget);
    ATLASSERT(hr == S_OK);
}

DXG::~DXG()
{
    if (m_pDevice)
        m_pDevice->Release();
    if (m_pSwapchain)
        m_pSwapchain->Release();
    if (m_pContext)
        m_pContext->Release();
    if (m_pTarget)
        m_pTarget->Release();
}

void DXG::PresentFrame()
{
    HRESULT hr = m_pSwapchain->Present(1, 0);
    ATLASSERT(hr == S_OK);
}

void DXG::ClearRenderView(float r, float g, float b, float a)
{
    float pColor[] = {r, g, b, a};
    m_pContext->ClearRenderTargetView(m_pTarget, pColor);
}