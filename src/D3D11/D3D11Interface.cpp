#include "D3D11/D3D11Interface.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")

#include "camera.h"
#include "Core/asserts.h"

#include "D3D11/D3D11Buffer.h"
#include "D3D11/D3D11Common.h"
#include "D3D11/D3D11Shader.h"

#include "RHI/rhi_shader.h"

#include "Engine/Mesh.h"
#include "IO/objloader.h"
#include "Shapes/teapot.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

D3D11Interface::D3D11Interface(HWND hWnd, Camera *pCamera)
	: m_spDevice(nullptr),
	  m_spSwapchain(nullptr),
	  m_spContext(nullptr),
	  m_pMainCamera(pCamera)
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
    ComPtr<ID3D11Resource> spBackBuffer{};
    //hr = m_spSwapchain->GetBuffer(0, /**__uuidof(ID3D11Resource)**/__uuidof(ID3D11Texture2D), &spBackBuffer);
    hr = m_spSwapchain->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer));
    ATLASSERT(hr == S_OK);

	// Create wireframe rasterizer state
	ComPtr<ID3D11RasterizerState> spWireframe{};
	D3D11_RASTERIZER_DESC oWfDesc{};
	ZeroMemory(&oWfDesc, sizeof(D3D11_RASTERIZER_DESC));
	oWfDesc.FillMode = D3D11_FILL_WIREFRAME;
	oWfDesc.CullMode = D3D11_CULL_NONE;
	hr = m_spDevice->CreateRasterizerState(&oWfDesc, &spWireframe);

	// Set rasterizer state
	m_spContext->RSSetState(spWireframe.Get());

    hr = m_spDevice->CreateRenderTargetView(spBackBuffer.Get(), nullptr, &m_spTarget);
    ATLASSERT(hr == S_OK);

    //Set our Render Target
    m_spContext->OMSetRenderTargets( 1, m_spTarget.GetAddressOf(), NULL );

    //InitTestScene();

	//ID3D11CommandList* pCommandList = nullptr;
	//pCommandList->QueryInterface(__uuidof(ID3D11CommandList), (void**)&m_spCommandList);
}

D3D11Interface::~D3D11Interface()
{
	delete m_pMainCamera;
}

HRESULT D3D11Interface::createShaderInstance(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage)
{
    if (!pShaderBuffer)
        return E_INVALIDARG;

    switch (eShaderStage)
    {
    case EShaderStage::VERTEX:
            return m_spDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11VertexShader**) pShaderInstance);
    case EShaderStage::PIXEL:
            return m_spDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11PixelShader**) pShaderInstance);
    default:
        return E_INVALIDARG;
    }
}

HRESULT D3D11Interface::createBufferInternal(
	D3D11Buffer* pRHIBuffer
	)
{
    // Init buffer descriptor
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

    bufferDesc.BindFlags = D3D11Buffer::CastToInterfaceFlag(pRHIBuffer->m_eFlags);
    bufferDesc.ByteWidth = pRHIBuffer->m_uByteWidth;//sizeof(float)  * 3 * 3;
    bufferDesc.CPUAccessFlags = D3D11Buffer::CastToInterfaceCPUAccess(pRHIBuffer->m_eCPUAccess);
    bufferDesc.MiscFlags = 0;
    bufferDesc.Usage = D3D11Buffer::CastToInterfaceUsage(pRHIBuffer->m_eUsage);

    //float Verts[9] = {  0.f, 0.5f, 0.5f,
    //                    0.5f, -0.5f, 0.5f,
    //                    -0.5f, -0.5f, 0.5f };
    // Init buffer data
    D3D11_SUBRESOURCE_DATA bufferData;
    ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    // TODO: Fix to handle multiple buffers
    bufferData.pSysMem = pRHIBuffer->m_pData;

    // Create buffer object
    return m_spDevice->CreateBuffer(&bufferDesc, (pRHIBuffer->m_pData) ? &bufferData : nullptr, (ID3D11Buffer**) &pRHIBuffer->pInitResource);
}

HRESULT D3D11Interface::createInputLayout(ID3D10Blob* pVSBuffer, DXGI_FORMAT eFormat, LPCSTR pName, void** pLayout)
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

    ComPtr<ID3D11InputLayout> spVertsLayout;
    return m_spDevice->CreateInputLayout(&inputDesc, 1, pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), (ID3D11InputLayout**) pLayout);
    //ATLASSERT(hr == S_OK);
}

void D3D11Interface::AddBuffers(std::vector<ID3D11Buffer*> vVertBuffers, ID3D11Buffer* pIdxBuffer, ComPtr<ID3D11InputLayout> spVertsLayout, UINT uStride, UINT uOffset)
{
    // Set buffers to Input assembly
    m_spContext->IASetVertexBuffers(0, vVertBuffers.size(), vVertBuffers.data(), &uStride, &uOffset);
    if (pIdxBuffer)
        m_spContext->IASetIndexBuffer(pIdxBuffer, DXGI_FORMAT_R32_UINT, uOffset);

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

void D3D11Interface::PresentFrame()
{
    HRESULT hr = m_spSwapchain->Present(0, 0);
    if (hr != S_OK)
    {
        LOG_LAST_ERROR();
        LOG_ERROR(m_spDevice->GetDeviceRemovedReason());
    }

    ATLASSERT(hr == S_OK);
}

std::shared_ptr<RHIBuffer> D3D11Interface::CreateBuffer(
	void* pData,
	unsigned int uByteWidth,
	ERHIBufferFlags eFlags,
	ERHICPUAccessFlags eCPUAccess,
	ERHIBufferUsage eUsage
)
{
	std::shared_ptr<D3D11Buffer> spBuffer = std::make_shared<D3D11Buffer>(pData, uByteWidth, eFlags, eCPUAccess, eUsage);
	return spBuffer;
}

bool D3D11Interface::UploadBuffer(const std::shared_ptr<RHIBuffer>& spBuffer)
{
    ATLASSERT(createBufferInternal((D3D11Buffer*) spBuffer.get()) == S_OK);
	return spBuffer->IsValid();
}

void D3D11Interface::CreateSwapchain()
{
    //return E_NOTIMPL;
}

void D3D11Interface::ClearRenderView()
{
    ClearRenderView(1.f, 0.f, 0.f);
}

void D3D11Interface::ClearRenderView(float r, float g, float b, float a)
{
    float pColor[] = {r, g, b, a};
    m_spContext->ClearRenderTargetView(m_spTarget.Get(), pColor);
}

std::shared_ptr<RHIShader> D3D11Interface::CreateShader(ERendererShaders eShader)
{
	const ShaderData& oShaderData = mapD3D11ShadersPaths[static_cast<unsigned int>(eShader)];
	return std::make_shared<D3D11Shader>(std::get<0>(oShaderData).data(), std::get<1>(oShaderData).data(), std::get<2>(oShaderData));
}

void D3D11Interface::SetVertexBuffer(const RHIBuffer *pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	uint32_t uStride = 0;
	uint32_t uOffset = sizeof(float) * 3;
	m_spContext->IASetVertexBuffers(0, 1, pD3D11Buffer->pInitResource.GetAddressOf(), &uStride, &uOffset);
	//m_spContext->IASetVertexBuffers(0, 1, &pD3D11Buffer->pInitResource, &pD3D11Buffer->uStride, &pD3D11Buffer->uOffset);
}

void D3D11Interface::SetIndexBuffer(const RHIBuffer* pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	uint32_t uOffset = sizeof(uint32_t) * 3;
	m_spContext->IASetIndexBuffer(pD3D11Buffer->pInitResource.Get(), DXGI_FORMAT_R32_UINT, uOffset);
}

void D3D11Interface::SetBuffer(const RHIBuffer *pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	m_spContext->VSSetConstantBuffers(0, 1, pD3D11Buffer->pInitResource.GetAddressOf());
}

void D3D11Interface::SetVertexShader(const RHIShader* pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->VSSetShader(dynamic_cast<ID3D11VertexShader*>(pD3D11Shader->m_spShader.Get()), nullptr, 0);

	m_spContext->IASetInputLayout(dynamic_cast<ID3D11InputLayout*>(pD3D11Shader->m_spInputLayout.Get()));
	// TODO: Make this generic
	m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Interface::SetPixelShader(const RHIShader* pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->PSSetShader(dynamic_cast<ID3D11PixelShader*>(pD3D11Shader->m_spShader.Get()), nullptr, 0);
}

void D3D11Interface::DrawIndexed(
		uint32_t uIndexCount,
		uint32_t uIndexOffset,
		uint32_t uVertexOffset
	)
{
    //m_pContext->IASetVertexBuffers(0, 1, )
    //m_spContext->Draw(3, 0);
	//m_spContext->Draw(NumTeapotVertices, 0);
	//m_spContext->Draw(56880, 0);

	//m_spContext->DrawIndexed(18960, 0, 0);
	m_spContext->DrawIndexed(uIndexCount, uIndexOffset, uVertexOffset);
}

//void D3D11Interface::InitTestScene()
//{
//    //----Create shader buffer----//
//    ComPtr<ID3D10Blob> spVSBuffer = compileShader(L"shaders/Default.hlsl", "VSDefaultMain", "vs_5_0");
//    ComPtr<ID3D10Blob> spPSBuffer = compileShader(L"shaders/Default.hlsl", "PSDefaultMain", "ps_5_0");
//
//    //----Create shader instances----//
//    ComPtr<ID3D11VertexShader> spVSInst;
//    HRESULT hr = createShaderInstance(spVSBuffer.Get(), &spVSInst, EShaderStage::VERTEX);
//    ATLASSERT(hr == S_OK);
//
//    ComPtr<ID3D11PixelShader> spPSInst;
//    hr = createShaderInstance(spPSBuffer.Get(), &spPSInst, EShaderStage::PIXEL);
//    ATLASSERT(hr == S_OK);
//
//
//    m_spContext->VSSetShader(spVSInst.Get(), 0, 0);
//    m_spContext->PSSetShader(spPSInst.Get(), 0, 0);
//
//    //----Load obj file----//
//	std::vector<Mesh*> vMeshes;
//	load_obj("models/teapot.obj", vMeshes);
//	Mesh* pMesh = vMeshes[0];
//
//    //----Bind vertices----//
//    float Verts[9] = {  0.f, 5.f, 5.f,
//                        5.f, -5.f, 5.f,
//                        -5.f, -5.f, 5.f };
//    std::vector<ID3D11Buffer*> vVertBuffers;
//
//	// IDX BUFFER
//
//    size_t ullIdxSize = pMesh->GetNumIndices() * sizeof(int);
//    //size_t ullIdxSize = sizeof(TeapotIndices);
//	D3D11Buffer* pIndiceBuffer = new D3D11Buffer(
//		pMesh->GetIndiceData(),		//Buffer data
//        ullIdxSize,                 //Buffer byte size
//		ERHIBufferFlags::INDEX,
//		ERHICPUAccessFlags::NONE);
//    //ComPtr<ID3D11Buffer> spBufferIndices;
//    ATLASSERT(createBufferInternal(
//		//pMesh->GetIndiceData(),
//        //Verts,                      //Buffer data
//        //ullIdxSize,                 //Buffer byte size
//        //&spBufferIndices,           //OutputBuffer
//        pIndiceBuffer
//        ) == S_OK);
//
//    //m_spContext->IASetIndexBuffer(spBufferIndices.Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	// END
//
//	// VERT BUFFER
//
// 	size_t ullVertsSize = pMesh->GetNumVerts() * 3 * sizeof(float);
//    //size_t ullVertsSize = sizeof(TeapotVertices);
//    //size_t ullVertsSize = sizeof(Verts);
//	D3D11Buffer* pVertBuffer = new D3D11Buffer(
//		pMesh->GetVerticeData(),
//		ullVertsSize,
//		ERHIBufferFlags::VERTEX,
//		ERHICPUAccessFlags::NONE);
//    //ComPtr<ID3D11Buffer> spBufferVerts;
//    ATLASSERT(createBufferInternal(
//		//pMesh->GetVerticeData(),
//        //(void*) (TeapotVertices),       //Buffer data
//        //(void*) (Verts),                //Buffer data
//        //ullVertsSize,                   //Buffer byte size
//        //&spBufferVerts,                 //OutputBuffer
//		pVertBuffer
//        ) == S_OK);
//    //vVertBuffers.push_back(spBufferVerts.Get());
//
//	//Set the vertex buffer
//    //UINT stride = sizeof(float) * 3;
//    //UINT offset = 0;
//    //m_spContext->IASetVertexBuffers( 0, 1, spBufferVerts.GetAddressOf(), &stride, &offset );
//
//	//END
//
//	// LAYOUT
//    //ComPtr<ID3D11InputLayout> spInputLayout{};
//    //ATLASSERT(
//    //    createInputLayout(
//    //        spVSBuffer.Get(),               //Shader text data
//    //        DXGI_FORMAT_R32G32B32_FLOAT,    //Data format
//    //        "POSITION",                     //Structure semantic name
//    //        &spInputLayout
//    //    ) == S_OK);
//
//    //m_spContext->IASetInputLayout(spInputLayout.Get());
//    //m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    ////Create the Viewport
//    //D3D11_VIEWPORT viewport;
//    //ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
//
//    //viewport.TopLeftX = 0;
//    //viewport.TopLeftY = 0;
//    //viewport.Width = 600;
//    //viewport.Height = 600;
//
//    ////Set the Viewport
//    //m_spContext->RSSetViewports(1, &viewport);
//
//	//AddBuffers(
//	//	vVertBuffers,           //Buffers to be bound
//	//	spBufferIndices.Get(),
//	//	//nullptr,  //Index buffer
//	//	spInputLayout,          //Layout for these buffers
//	//	sizeof(float) * 3,      //Size of buffer elements
//	//	0                       //Byte shift before 1st elt
//	//	);
//
//    ATLASSERT(m_pMainCamera != nullptr);
//
//    //----Bind uniform data----//
//
//    // TODO: Move ViewMat compute to camera
//    Vec3 oPos = m_pMainCamera->GetPosition();
//    Vec3 oLookAt =  {0.f, 0.f, 0.f};
//    Vec3 oUp = m_pMainCamera->GetUpVector();
//
//    // Not useful yet => we just try to place an object at (0, 0, 0)
//    Mat4x4 oModelMat = dx::XMMatrixIdentity();
//
//    Mat4x4 oViewMat = dx::XMMatrixLookAtLH(
//                        dx::XMLoadFloat3(&oPos),
//                        dx::XMLoadFloat3(&oLookAt),
//                        dx::XMLoadFloat3(&oUp));
//
//    Mat4x4 oViewProj = dx::XMMatrixPerspectiveFovLH(
//            m_pMainCamera->GetFOV(),
//            m_pMainCamera->GetAspectRatio(),
//            m_pMainCamera->GetNearClipping(),
//            m_pMainCamera->GetFarClipping()
//        );
//        //DirectX::XMStoreFloat4x4(&constants.model, DirectX::XMMatrixIdentity());
//        //DirectX::XMStoreFloat4x4(&constants.view, DirectX::XMMatrixTranspose(create_view_matrix(main_camera.position, main_camera.look_at)));
//        //DirectX::XMStoreFloat4x4(&constants.projection, DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(main_camera.fov, main_camera.aspect_ratio, main_camera.near_clip, main_camera.far_clip)))
//
//    Mat4x4* pMVP = new Mat4x4(dx::XMMatrixTranspose(oModelMat * oViewMat * oViewProj));
//
//	D3D11Buffer* pMVPBuffer = new D3D11Buffer(
//		pMVP,
//		sizeof(Mat4x4),
//		ERHIBufferFlags::CONSTANT,
//		ERHICPUAccessFlags::WRITE);
//
//    //ComPtr<ID3D11Buffer> spMVPBuffer;
//    ATLASSERT(!FAILED(createBufferInternal(
//        //&oMVP,
//        //sizeof(Mat4x4),
//        //&spMVPBuffer,
//		pMVPBuffer,
//        D3D11_USAGE_DYNAMIC
//    )));
//
//    m_spContext->VSSetConstantBuffers(0u, 1u, (ID3D11Buffer**) pMVPBuffer->pInitResource.GetAddressOf());
//    //m_spContext->VSSetConstantBuffers(0u, 1u, spMVPBuffer.GetAddressOf());
//    //spMVPBuffer->
//}
