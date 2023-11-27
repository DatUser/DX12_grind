#include "dxg.h"

#include <atldef.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

DXG::DXG(HWND hWnd, Camera* pCamera)
: m_spDevice(nullptr)
, m_spSwapchain(nullptr)
, m_spContext(nullptr)
, m_pMainCamera(pCamera)
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
    m_spContext->OMSetRenderTargets( 1, m_spTarget.GetAddressOf(), NULL );

    InitTestScene();
}

void DXG::PresentFrame()
{
    HRESULT hr = m_spSwapchain->Present(0, 0);
    if (hr != S_OK)
    {
        LOG_LAST_ERROR();
        LOG_ERROR(m_spDevice->GetDeviceRemovedReason());
    }
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

HRESULT DXG::createBuffer(void* pData, UINT uByteWidth, void** opBuffer, UINT uFlags, D3D11_USAGE eUsage, UINT uCPUAccess)
{
    // Init buffer descriptor
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

    bufferDesc.BindFlags = uFlags;
    bufferDesc.ByteWidth = uByteWidth;//sizeof(float)  * 3 * 3;
    bufferDesc.CPUAccessFlags = uCPUAccess;
    bufferDesc.MiscFlags = 0;
    bufferDesc.Usage = eUsage;//D3D11_USAGE_DEFAULT;

    //float Verts[9] = {  0.f, 0.5f, 0.5f,
    //                    0.5f, -0.5f, 0.5f,
    //                    -0.5f, -0.5f, 0.5f };
    // Init buffer data
    D3D11_SUBRESOURCE_DATA bufferData;
    ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    // TODO: Fix to handle multiple buffers
    bufferData.pSysMem = pData;

    // Create buffer object
    wrl::ComPtr<ID3D11Buffer> spBuffer;
    return m_spDevice->CreateBuffer(&bufferDesc, (pData) ? &bufferData : nullptr, (ID3D11Buffer**) opBuffer);
}

HRESULT DXG::createInputLayout(ID3D10Blob* pVSBuffer, DXGI_FORMAT eFormat, LPCSTR pName, void** pLayout)
{
    // Create data layout
    D3D11_INPUT_ELEMENT_DESC inputDesc;
    ZeroMemory(&inputDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
    inputDesc.AlignedByteOffset = 0;
    inputDesc.Format = eFormat;//DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc.InputSlot = 0;
    inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc.InstanceDataStepRate = 0;
    inputDesc.SemanticIndex = 0;
    inputDesc.SemanticName = pName;// "POSITION";

    wrl::ComPtr<ID3D11InputLayout> spVertsLayout;
    return m_spDevice->CreateInputLayout(&inputDesc, 1, pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), (ID3D11InputLayout**) pLayout);
    //ATLASSERT(hr == S_OK);
}

void DXG::AddBuffers(std::vector<ID3D11Buffer*> vBuffers, wrl::ComPtr<ID3D11InputLayout> spVertsLayout, UINT uStride, UINT uOffset)
{
    // Set buffers to Input assembly
    m_spContext->IASetVertexBuffers(0, vBuffers.size(), vBuffers.data(), &uStride, &uOffset);

    //m_spContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, uOffset);

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
    //----Create shader buffer----//
    wrl::ComPtr<ID3D10Blob> spVSBuffer = compileShader(L"shaders/Default.hlsl", "VSDefaultMain", "vs_5_0");
    wrl::ComPtr<ID3D10Blob> spPSBuffer = compileShader(L"shaders/Default.hlsl", "PSDefaultMain", "ps_5_0");

    //----Create shader instances----//
    wrl::ComPtr<ID3D11VertexShader> spVSInst;
    HRESULT hr = createShaderInstance(spVSBuffer.Get(), &spVSInst, EShaderStage::VERTEX_SHADER);
    ATLASSERT(hr == S_OK);

    wrl::ComPtr<ID3D11PixelShader> spPSInst;
    hr = createShaderInstance(spPSBuffer.Get(), &spPSInst, EShaderStage::FRAGMENT_SHADER);
    ATLASSERT(hr == S_OK);


    m_spContext->VSSetShader(spVSInst.Get(), 0, 0);
    m_spContext->PSSetShader(spPSInst.Get(), 0, 0);

    //----Bind vertices----//
    float Verts[9] = {  0.f, 5.f, 5.f,
                        5.f, -5.f, 5.f,
                        -5.f, -5.f, 5.f };

    std::vector<ID3D11Buffer*> vBuffers;
    wrl::ComPtr<ID3D11Buffer> spBuffer;
    ATLASSERT(createBuffer(
        Verts,                      //Buffer data
        sizeof(float) * 9,          //Buffer byte size
        &spBuffer,                  //OutputBuffer
        D3D11_BIND_VERTEX_BUFFER    //Buffer flags
        ) == S_OK);
    vBuffers.push_back(spBuffer.Get());

    wrl::ComPtr<ID3D11InputLayout> spInputLayout;
    ATLASSERT(
        createInputLayout(
            spVSBuffer.Get(),               //Shader text data
            DXGI_FORMAT_R32G32B32_FLOAT,    //Data format
            "POSITION",                     //Structure semantic name
            &spInputLayout
        ) == S_OK);

    AddBuffers(
        vBuffers,           //Buffers to be bound
        spInputLayout,      //Layout for these buffers
        sizeof(float) * 3,  //Size of buffer elements
        0                   //Byte shift before 1st elt
        );

    ATLASSERT(m_pMainCamera != nullptr);

    //----Bind uniform data----//

    // TODO: Move ViewMat compute to camera
    Vec3 oPos = m_pMainCamera->GetPosition();
    Vec3 oLookAt =  {0.f, 0.f, 0.f};
    Vec3 oUp = m_pMainCamera->GetUpVector();

    // Not useful yet => we just try to place an object at (0, 0, 0)
    Mat4x4 oModelMat = dx::XMMatrixIdentity();

    Mat4x4 oViewMat = dx::XMMatrixLookAtLH(
                        dx::XMLoadFloat3(&oPos),
                        dx::XMLoadFloat3(&oLookAt),
                        dx::XMLoadFloat3(&oUp));

    Mat4x4 oViewProj = dx::XMMatrixTranspose(dx::XMMatrixPerspectiveFovLH(
            m_pMainCamera->GetFOV(),
            m_pMainCamera->GetAspectRatio(),
            m_pMainCamera->GetNearClipping(),
            m_pMainCamera->GetFarClipping()
        ));
        //DirectX::XMStoreFloat4x4(&constants.model, DirectX::XMMatrixIdentity());
        //DirectX::XMStoreFloat4x4(&constants.view, DirectX::XMMatrixTranspose(create_view_matrix(main_camera.position, main_camera.look_at)));
        //DirectX::XMStoreFloat4x4(&constants.projection, DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(main_camera.fov, main_camera.aspect_ratio, main_camera.near_clip, main_camera.far_clip)))

    Mat4x4 oMVP = oModelMat * oViewMat * oViewProj;

    wrl::ComPtr<ID3D11Buffer> spMVPBuffer;
    ATLASSERT(!FAILED(createBuffer(
        &oMVP,
        sizeof(Mat4x4),
        &spMVPBuffer,
        D3D11_BIND_CONSTANT_BUFFER,
        D3D11_USAGE_DYNAMIC,
        D3D11_CPU_ACCESS_WRITE
    )));

    m_spContext->VSSetConstantBuffers(0u, 1u, spMVPBuffer.GetAddressOf());
    //spMVPBuffer->
}

void DXG::DrawHelloTriangle()
{
    //m_pContext->IASetVertexBuffers(0, 1, )
    m_spContext->Draw(3, 0);
}