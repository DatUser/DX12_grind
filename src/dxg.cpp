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
    oSwapDesc.BufferDesc.Width = 600;
    oSwapDesc.BufferDesc.Height = 600;
    oSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    oSwapDesc.BufferDesc.RefreshRate.Numerator = 60;
    oSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
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
    hr = m_spSwapchain->GetBuffer(0, /**__uuidof(ID3D11Resource)**/__uuidof(ID3D11Texture2D), &spBackBuffer);
    ATLASSERT(hr == S_OK);

    hr = m_spDevice->CreateRenderTargetView(spBackBuffer.Get(), nullptr, &m_spTarget);
    ATLASSERT(hr == S_OK);

    //Set our Render Target
    m_spContext->OMSetRenderTargets( 1, &m_spTarget, NULL );

    InitTestScene();
}

void DXG::PresentFrame()
{
    HRESULT hr = m_spSwapchain->Present(0, 0);
    if (hr != S_OK)
        LOG_ERROR(m_spDevice->GetDeviceRemovedReason());
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
        return nullptr;
    }

    return spShaderBuffer;
}

HRESULT DXG::createShaderInstance(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage)
{
    if (!pShaderBuffer)
        return E_INVALIDARG;

    switch (eShaderStage)
    {
    case EShaderStage::VERTEX_SHADER:
            return m_spDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11VertexShader**) pShaderInstance);
    case EShaderStage::FRAGMENT_SHADER:
            return m_spDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11PixelShader**) pShaderInstance);
    default:
        return E_INVALIDARG;
    }
}

void DXG::AddBuffers(void** pBuffers, int nBuffers, ID3D10Blob* pVSBuffer, UINT uFlags)
{
    // Init buffer descriptor
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

    bufferDesc.BindFlags = uFlags;
    bufferDesc.ByteWidth = sizeof(float)  * 3 * 3;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;

    float Verts[9] = {  0.f, 0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f };
    // Init buffer data
    D3D11_SUBRESOURCE_DATA bufferData;
    ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    // TODO: Fix to handle multiple buffers
    bufferData.pSysMem = Verts;//*pBuffers;//[0];

    // Create buffer object
    wrl::ComPtr<ID3D11Buffer> spBuffer;
    HRESULT hr = m_spDevice->CreateBuffer(&bufferDesc, &bufferData, &spBuffer);
    ATLASSERT(hr == S_OK);
    m_vBuffers.push_back(spBuffer);

    // Set buffers to Input assembly
    UINT uStride = sizeof(float) * 3;
    UINT uOffset = 0;
    m_spContext->IASetVertexBuffers(0, nBuffers, &m_vBuffers.data()[/*m_vBuffers.size()*/0], &uStride, &uOffset);

    // Create data layout
    D3D11_INPUT_ELEMENT_DESC inputDesc;
    ZeroMemory(&inputDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
    inputDesc.AlignedByteOffset = 0;
    inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc.InputSlot = 0;
    inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc.InstanceDataStepRate = 0;
    inputDesc.SemanticIndex = 0;
    inputDesc.SemanticName = "POSITION";

    wrl::ComPtr<ID3D11InputLayout> spVertsLayout;
    hr = m_spDevice->CreateInputLayout(&inputDesc, 1, pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), &spVertsLayout);
    ATLASSERT(hr == S_OK);

    m_spContext->IASetInputLayout(spVertsLayout.Get());

    // Set triangle topology
    m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Viewport creation
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Height = 600;
    viewport.Width = 600;

    m_spContext->RSSetViewports(1, &viewport);
}

void DXG::InitTestScene()
{
    // Create shader buffer
    wrl::ComPtr<ID3D10Blob> spVSBuffer = compileShader(L"shaders/Default.hlsl", "VSDefaultMain", "vs_5_0");
    wrl::ComPtr<ID3D10Blob> spPSBuffer = compileShader(L"shaders/Default.hlsl", "PSDefaultMain", "ps_5_0");

    // Create shader instances
    wrl::ComPtr<ID3D11VertexShader> spVSInst;
    HRESULT hr = createShaderInstance(spVSBuffer.Get(), &spVSInst, EShaderStage::VERTEX_SHADER);
    ATLASSERT(hr == S_OK);

    wrl::ComPtr<ID3D11PixelShader> spPSInst;
    hr = createShaderInstance(spPSBuffer.Get(), &spPSInst, EShaderStage::FRAGMENT_SHADER);
    ATLASSERT(hr == S_OK);


    m_spContext->VSSetShader(spVSInst.Get(), 0, 0);
    m_spContext->PSSetShader(spPSInst.Get(), 0, 0);

    float Verts[9] = {  0.f, 0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        -0.5f, -0.5f, 0.5f };
    AddBuffers((void**)&Verts, 1, spVSBuffer.Get());
}

void DXG::DrawHelloTriangle()
{
    //m_pContext->IASetVertexBuffers(0, 1, )
    m_spContext->Draw(3, 0);
}