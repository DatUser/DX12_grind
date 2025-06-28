#include "D3D11/D3D11Interface.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi.lib")

#include "Core/asserts.h"
#include "Engine/camera.h"
#include "Core/operations.h"

#include "Engine/app.h"

#include "D3D11/D3D11Buffer.h"
#include "D3D11/D3D11Shader.h"
#include "D3D11/D3D11Swapchain.h"
#include "D3D11/D3D11Texture.h"
#include "D3D11/D3D11Utils.h"
#include "D3D11/D3D11Viewport.h"

#include "RHI/rhi_shader.h"

#include "Engine/mesh.h"
#include "Shapes/teapot.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

D3D11Interface::D3D11Interface()
	: m_spDevice(nullptr),
	  m_spContext(nullptr)
{
	uint32_t uCreationFlags = 0;
#ifdef DEBUG
	uCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	ATLASSERT(D3D11CreateDevice(
			nullptr,					// Adapter
			D3D_DRIVER_TYPE_HARDWARE,	// Driver type
			nullptr,					// Module software
			uCreationFlags,							// Flags
			nullptr,					// Feature levels
			0,							// Feature level count
			D3D11_SDK_VERSION,			// SDK version
			&m_spDevice,				// Device
			nullptr,					// Feature level
			&m_spContext				// Context
		) == S_OK);

	ATLASSERT(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_spFactory)) == S_OK);

    //HRESULT hr = D3D11CreateDeviceAndSwapChain(
    //    nullptr,					// Adapter
    //    D3D_DRIVER_TYPE_HARDWARE, 	// Driver type
    //    nullptr,					// Module software
    //    0,							// Flags
    //    nullptr,					// Feature levels
    //    0,							// Feature level count
    //    D3D11_SDK_VERSION,			// SDK version
    //    //Here pointer is released then we access its address
    //    &oSwapDesc,					// Swap chain description
    //    &m_spSwapchain,				// Swap chain
    //    &m_spDevice,				// Device
    //    nullptr,					// Feature level
    //    &m_spContext				// Context
    //);
}

D3D11Interface::~D3D11Interface()
{
}

HRESULT D3D11Interface::createShaderInstanceInternal(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage)
{
    if (!pShaderBuffer)
        return E_INVALIDARG;

    switch (eShaderStage)
    {
    case EShaderStage::VERTEX:
            return m_spDevice->CreateVertexShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11VertexShader**) pShaderInstance);
    case EShaderStage::GEOMETRY:
    		return m_spDevice->CreateGeometryShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11GeometryShader**) pShaderInstance);
    case EShaderStage::PIXEL:
            return m_spDevice->CreatePixelShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11PixelShader**) pShaderInstance);
	case EShaderStage::COMPUTE:
    		return m_spDevice->CreateComputeShader(pShaderBuffer->GetBufferPointer(), pShaderBuffer->GetBufferSize(), nullptr, (ID3D11ComputeShader**) pShaderInstance);
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
    bufferDesc.CPUAccessFlags = CastToInterfaceCPUAccess(pRHIBuffer->m_eCPUAccess);
    bufferDesc.MiscFlags = 0;
    bufferDesc.Usage = CastToInterfaceUsage(pRHIBuffer->m_eUsage);

    //float Verts[9] = {  0.f, 0.5f, 0.5f,
    //                    0.5f, -0.5f, 0.5f,
    //                    -0.5f, -0.5f, 0.5f };
    // Init buffer data
    D3D11_SUBRESOURCE_DATA bufferData;
    ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    // TODO: Fix to handle multiple buffers
    bufferData.pSysMem = pRHIBuffer->m_pData;

    // Create buffer object
    return m_spDevice->CreateBuffer(&bufferDesc, (pRHIBuffer->m_pData) ? &bufferData : nullptr, (ID3D11Buffer**) &pRHIBuffer->m_spInitResource);
}

HRESULT D3D11Interface::createTextureInternal(D3D11Texture* pTexture)
{
	D3D11_TEXTURE2D_DESC oTextureDesc;
	ZeroMemory(&oTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	oTextureDesc.Width = pTexture->m_iWidth;
	oTextureDesc.Height = pTexture->m_iHeight;
	oTextureDesc.MipLevels = 0;
	oTextureDesc.ArraySize = 1;
	oTextureDesc.Format = D3D11Texture::CastToInterfaceFormat(pTexture->m_eFormat);
	oTextureDesc.SampleDesc.Count = 1;
	oTextureDesc.SampleDesc.Quality = 0;
	oTextureDesc.Usage = CastToInterfaceUsage(pTexture->m_eUsage);
	oTextureDesc.BindFlags = D3D11Texture::CastToInterfaceBindFlags(pTexture->m_uFlags);
	oTextureDesc.CPUAccessFlags = CastToInterfaceCPUAccess(pTexture->m_eCPUAccess);
	oTextureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA oTexData;
	ZeroMemory(&oTexData, sizeof(D3D11_SUBRESOURCE_DATA));

	oTexData.pSysMem = pTexture->m_pData;

	return m_spDevice->CreateTexture2D(&oTextureDesc, pTexture->m_pData ? &oTexData : nullptr, &pTexture->m_spInitResource);
}

HRESULT D3D11Interface::createRTVInternal(D3D11Texture *pTexture)
{
	ID3D11View** pView = &pTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET))];
	ID3D11RenderTargetView** pRTV = reinterpret_cast<ID3D11RenderTargetView**>(pView);

	return m_spDevice->CreateRenderTargetView(
		pTexture->m_spInitResource.Get(),
		nullptr,
		pRTV);
		//(ID3D11RenderTargetView**) (&pTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET))]));
}

HRESULT D3D11Interface::createDSVInternal(D3D11Texture *pTexture)
{
	ID3D11View** pView = &pTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL))];
	ID3D11DepthStencilView** pDSV = reinterpret_cast<ID3D11DepthStencilView**>(pView);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = D3D11Texture::CastToInterfaceFormat(pTexture->m_eFormat);
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view

	return m_spDevice->CreateDepthStencilView( pTexture->m_spInitResource.Get(), // Depth stencil texture
												 &descDSV, // Depth stencil desc
												 pDSV );  // [out] Depth stencil view
}

HRESULT D3D11Interface::createSRVInternal(D3D11Texture *pTexture)
{
	ID3D11View** pView = &pTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::SHADER_RESOURCE))];
	ID3D11ShaderResourceView** pSRV = reinterpret_cast<ID3D11ShaderResourceView**>(pView);

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	descSRV.Format = D3D11Texture::CastToInterfaceFormat(pTexture->m_eFormat);
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	descSRV.Texture2D.MostDetailedMip = 0;

	// Create the Shader resource view
	return m_spDevice->CreateShaderResourceView( pTexture->m_spInitResource.Get(), // Texture
												 &descSRV, // View desc
												 pSRV );  // [out] Shader resource view
}

HRESULT D3D11Interface::createUAVInternal(D3D11Texture *pTexture)
{
	ID3D11View** pView = &pTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::UNORDERED_ACCESS))];
	ID3D11UnorderedAccessView** pUAV = reinterpret_cast<ID3D11UnorderedAccessView**>(pView);

	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	descUAV.Format = D3D11Texture::CastToInterfaceFormat(pTexture->m_eFormat);
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	descUAV.Texture2D.MipSlice = 0;

	// Create the Shader resource view
	return m_spDevice->CreateUnorderedAccessView( pTexture->m_spInitResource.Get(), // Texture
												 &descUAV, // View desc
												 pUAV );  // [out] Unordered access view
}

HRESULT D3D11Interface::createDSSInternal(ID3D11DepthStencilState ** pDSState, bool enableStencilTest)
{
	D3D11_DEPTH_STENCIL_DESC oDsDesc{};
	ZeroMemory(&oDsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Depth test parameters
	oDsDesc.DepthEnable = true;
	oDsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	oDsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create depth stencil state
	return m_spDevice->CreateDepthStencilState(&oDsDesc, pDSState);
}

HRESULT D3D11Interface::createSwapchainInternal(D3D11Swapchain *pSwapchain, HWND hWnd, uint32_t uWidth, uint32_t uHeight)
{
	// TODO: Move this to renderer using RHI interface
    DXGI_SWAP_CHAIN_DESC oSwapDesc;
    ZeroMemory(&oSwapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    oSwapDesc.BufferDesc.Width = uWidth;
    oSwapDesc.BufferDesc.Height = uHeight;
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

	return m_spFactory->CreateSwapChain(m_spDevice.Get(), &oSwapDesc, &pSwapchain->m_spSwapchain);
}

HRESULT D3D11Interface::createInputLayout(ID3D10Blob* pVSBuffer, std::vector<InputLayoutFormat> vInputLayout, void** pLayout)
{
    // Create data layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
	for (int i = 0; i < vInputLayout.size(); ++i)
	{
		D3D11_INPUT_ELEMENT_DESC inputDesc;
		ZeroMemory(&inputDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));
		inputDesc.AlignedByteOffset = std::get<2>(vInputLayout[i]);
		inputDesc.Format = std::get<0>(vInputLayout[i]);
		inputDesc.InputSlot = 0;
		inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc.InstanceDataStepRate = 0;
		inputDesc.SemanticIndex = 0;
		inputDesc.SemanticName = std::get<1>(vInputLayout[i]);
		vInputLayoutDesc.push_back(inputDesc);
	}

    ComPtr<ID3D11InputLayout> spVertsLayout;
    return m_spDevice->CreateInputLayout(vInputLayoutDesc.data(), vInputLayout.size(), pVSBuffer->GetBufferPointer(), pVSBuffer->GetBufferSize(), (ID3D11InputLayout**) pLayout);
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
   // m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // Viewport creation
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Height = 600;
    viewport.Width = 600;

    m_spContext->RSSetViewports(1, &viewport);
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

std::shared_ptr<RHITexture> D3D11Interface::CreateTexture(
	void* pData,
	int iWidth,
	int iHeight,
	ETextureFormat eFormat,
	uint32_t uFlags
)
{
	std::shared_ptr<D3D11Texture> spTexture = std::make_shared<D3D11Texture>(pData, iWidth, iHeight, eFormat, uFlags);
	return spTexture;
}

void D3D11Interface::SetBufferData(const RHIBuffer* pBuffer, const void* pData)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	D3D11_MAPPED_SUBRESOURCE oMappedSubresource{};

	HRESULT hr = m_spContext->Map(
		pD3D11Buffer->m_spInitResource.Get(),				// Resource
		0,												// Subresource
		D3D11_MAP_WRITE_DISCARD,						// Map type
		0,												// Flags (What CPU does during upload)
		&oMappedSubresource											// Mapped resource (Data, rowPitch, depthPitch)
		);
	memcpy(oMappedSubresource.pData, pData, pBuffer->m_uByteWidth);
	m_spContext->Unmap(pD3D11Buffer->m_spInitResource.Get(), 0);
	 if (FAILED(hr))
	 	LOG_ERROR(hr)
	//m_spContext->UpdateSubresource(pD3D11Buffer->pInitResource.Get(), 0, nullptr, pBuffer->m_pData, 0, 0);
    //ATLASSERT(createBufferInternal((D3D11Buffer*) spBuffer.get()) == S_OK);
	//return spBuffer->IsValid();
}

std::shared_ptr<RHISwapchain> D3D11Interface::CreateSwapchain(
	HWND hWnd,
	uint32_t uWidth,
	uint32_t uHeight
)
{
	std::shared_ptr<RHISwapchain> spSwapchain = std::make_shared<D3D11Swapchain>(hWnd, uWidth, uHeight);
	return spSwapchain;
}

void D3D11Interface::SetRasterizerState(ECullMode eMode, bool bIsWireframe)
{
	// Create wireframe rasterizer state
	ComPtr<ID3D11RasterizerState> spWireframe{};
	D3D11_RASTERIZER_DESC oWfDesc{};
	ZeroMemory(&oWfDesc, sizeof(D3D11_RASTERIZER_DESC));
	oWfDesc.FillMode = bIsWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	oWfDesc.CullMode = CastToInterfaceCullMode(eMode);
	ATLASSERT(m_spDevice->CreateRasterizerState(&oWfDesc, &spWireframe) == S_OK);

	// Set rasterizer state
	m_spContext->RSSetState(spWireframe.Get());

}

std::shared_ptr<RHIViewport> D3D11Interface::CreateViewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
{
	return std::make_shared<D3D11Viewport>(hWnd, uWidth, uHeight);
}

void D3D11Interface::SetViewport(const RHIViewport *pViewport) {
	const D3D11Viewport* pD3D11Viewport = dynamic_cast<const D3D11Viewport*>(pViewport);
	m_spContext->RSSetViewports(1, &pD3D11Viewport->m_oViewport);
}

void D3D11Interface::ClearRenderView(const RHITexture* pTexture, float fR, float fG, float fB, float fA)
{
    float pColor[] = {fR, fG, fB, fA};
	const D3D11Texture* pD3D11Texture = dynamic_cast<const D3D11Texture*>(pTexture);
	ID3D11View* pView = pD3D11Texture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET))].Get();
	ID3D11RenderTargetView* pRTV = (ID3D11RenderTargetView*) pView;
    m_spContext->ClearRenderTargetView(pRTV, pColor);
}

void D3D11Interface::ClearDepthStencilView(const RHITexture *pTexture)
{
	const D3D11Texture* pD3D11Texture = dynamic_cast<const D3D11Texture*>(pTexture);
	ID3D11View* pView = pD3D11Texture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL))].Get();
	ID3D11DepthStencilView* pDSV = (ID3D11DepthStencilView*) pView;
	m_spContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void D3D11Interface::ClearUnorderedAccessView(const RHITexture *pTexture, float fR, float fG, float fB, float fA)
{
    float pColor[] = {fR, fG, fB, fA};
	const D3D11Texture* pD3D11Texture = dynamic_cast<const D3D11Texture*>(pTexture);
	ID3D11View* pView = pD3D11Texture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::UNORDERED_ACCESS))].Get();
	ID3D11UnorderedAccessView* pUAV = (ID3D11UnorderedAccessView*) pView;

	m_spContext->ClearUnorderedAccessViewFloat(pUAV, pColor);
}

std::shared_ptr<RHIShader> D3D11Interface::CreateShader(ERendererShaders eShader)
{
	const ShaderData& oShaderData = mapD3D11ShadersPaths[static_cast<unsigned int>(eShader)];
	return std::make_shared<D3D11Shader>(std::get<0>(oShaderData).data(), std::get<1>(oShaderData).data(), std::get<2>(oShaderData));
}

void D3D11Interface::SetVertexBuffer(const RHIBuffer *pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	uint32_t uStride = sizeof(float) * 3;
	uint32_t uOffset = 0;
	m_spContext->IASetVertexBuffers(0, 1, pD3D11Buffer->m_spInitResource.GetAddressOf(), &uStride, &uOffset);
	//m_spContext->IASetVertexBuffers(0, 1, &pD3D11Buffer->pInitResource, &pD3D11Buffer->uStride, &pD3D11Buffer->uOffset);
}

void D3D11Interface::SetIndexBuffer(const RHIBuffer* pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	uint32_t uOffset = 0;
	m_spContext->IASetIndexBuffer(pD3D11Buffer->m_spInitResource.Get(), DXGI_FORMAT_R32_UINT, uOffset);
}

template <>
void D3D11Interface::SetBufferInternal<EShaderStage::VERTEX>(uint32_t uSlot, const RHIBuffer* pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	m_spContext->VSSetConstantBuffers(uSlot, 1, pD3D11Buffer->m_spInitResource.GetAddressOf());
}

template <>
void D3D11Interface::SetBufferInternal<EShaderStage::GEOMETRY>(uint32_t uSlot, const RHIBuffer* pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	m_spContext->GSSetConstantBuffers(uSlot, 1, pD3D11Buffer->m_spInitResource.GetAddressOf());
}

// TODO : Add UAV alternative
template <>
void D3D11Interface::SetBufferInternal<EShaderStage::COMPUTE>(uint32_t uSlot, const RHIBuffer* pBuffer)
{
	const D3D11Buffer* pD3D11Buffer = dynamic_cast<const D3D11Buffer*>(pBuffer);
	m_spContext->CSSetConstantBuffers(uSlot, 1, pD3D11Buffer->m_spInitResource.GetAddressOf());
}

template <>
void D3D11Interface::SetUAVInternal<EShaderStage::VERTEX>(uint32_t uSlot, const RHIResource *pResource)
{

}

template <>
void D3D11Interface::SetUAVInternal<EShaderStage::GEOMETRY>(uint32_t uSlot, const RHIResource *pResource)
{
}

template <>
void D3D11Interface::SetUAVInternal<EShaderStage::COMPUTE>(uint32_t uSlot, const RHIResource *pResource)
{
	if (pResource == nullptr)
	{
		ID3D11UnorderedAccessView* pUAV = nullptr;
		m_spContext->CSSetUnorderedAccessViews(uSlot, 1, &pUAV, nullptr);
		return;
	}

	// TODO: Make this generic
	const D3D11Texture* pD3D11TargetTexture = dynamic_cast<const D3D11Texture*>(pResource);
	ATLASSERT(pD3D11TargetTexture &&
		(pD3D11TargetTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::UNORDERED_ACCESS)) != 0);

	ID3D11View* const* pView = pD3D11TargetTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::UNORDERED_ACCESS))].GetAddressOf();
	ID3D11UnorderedAccessView* const* pUAV = reinterpret_cast<ID3D11UnorderedAccessView* const*>(pView);

	m_spContext->CSSetUnorderedAccessViews(uSlot, 1, pUAV, nullptr);
}

void D3D11Interface::SetBuffer(uint32_t uSlot, const RHIBuffer *pBuffer, ShaderType eShaderStage)
{
	std::visit(
		// Here eStage is replaced at compile time which allows it to be used as a template parameter
		[this, uSlot, pBuffer](auto eStage)
		{SetBufferInternal<eStage>(uSlot, pBuffer);},
		eShaderStage);
}

template <>
void D3D11Interface::SetTextureInternal<EShaderStage::VERTEX>(uint32_t uSlot, const RHITexture* pTexture)
{

}

template <>
void D3D11Interface::SetTextureInternal<EShaderStage::GEOMETRY>(uint32_t uSlot, const RHITexture* pTexture)
{
}

template <>
void D3D11Interface::SetTextureInternal<EShaderStage::COMPUTE>(uint32_t uSlot, const RHITexture* pResource)
{
	if (pResource == nullptr)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		m_spContext->CSSetShaderResources(uSlot, 1, &pSRV);
		return;
	}

	// TODO: Make this generic
	const D3D11Texture* pD3D11TargetTexture = dynamic_cast<const D3D11Texture*>(pResource);
	ATLASSERT(pD3D11TargetTexture &&
		(pD3D11TargetTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::SHADER_RESOURCE)) != 0);

	ID3D11View* const* pView = pD3D11TargetTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::SHADER_RESOURCE))].GetAddressOf();
	ID3D11ShaderResourceView* const* pSRV = reinterpret_cast<ID3D11ShaderResourceView* const*>(pView);

	m_spContext->CSSetShaderResources(uSlot, 1, pSRV);
}

void D3D11Interface::SetTexture(uint32_t uSlot, const RHITexture *pTexture, ShaderType eShaderStage, bool bIsUAV)
{
	// TODO: Rework this -> use std array of partial specialized templated Set... methods
	if (bIsUAV)
		std::visit(
		// Here eStage is replaced at compile time which allows it to be used as a template parameter
		[this, uSlot, pTexture](auto eStage) {
			SetUAVInternal<eStage>(uSlot, pTexture);
		},
		eShaderStage);
	else
		std::visit(
		// Here eStage is replaced at compile time which allows it to be used as a template parameter
		[this, uSlot, pTexture](auto eStage) {
			SetTextureInternal<eStage>(uSlot, pTexture);
		},
		eShaderStage);
}

void D3D11Interface::SetContextRenderTarget(const RHITexture* pTarget, const RHITexture* pDepth)
{
	// Unset render view
	if (pTarget == nullptr)
	{
		m_spContext->OMSetRenderTargets(0, nullptr, nullptr);
		return;
	}

	const D3D11Texture* pD3D11TargetTexture = dynamic_cast<const D3D11Texture*>(pTarget);
	ATLASSERT(pD3D11TargetTexture &&
		(pD3D11TargetTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET)) != 0);

	ID3D11View* const* pView = pD3D11TargetTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET))].GetAddressOf();
	ID3D11RenderTargetView* const* pRTV = reinterpret_cast<ID3D11RenderTargetView* const*>(pView);

	if (pDepth) {
		const D3D11Texture* pD3D11DepthTexture = dynamic_cast<const D3D11Texture*>(pDepth);
		ATLASSERT(pD3D11DepthTexture &&
		(pD3D11DepthTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL)) != 0);

		ID3D11View* pDepthStencilView = pD3D11DepthTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL))].Get();
		ID3D11DepthStencilView* pDSV = reinterpret_cast<ID3D11DepthStencilView*>(pDepthStencilView);

		m_spContext->OMSetRenderTargets(1, pRTV, pDSV);
	}
	else {
		m_spContext->OMSetRenderTargets(1, pRTV, nullptr);
	}

}

void D3D11Interface::SetContextRenderTargets(RHITexture* const* pTargets, uint32_t uNumTargets, const RHITexture* pDepth)
{
	// Retrieve the render target views
	static std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> arrRTVs{nullptr};
	const D3D11Texture* pD3D11TargetTexture = nullptr;

	for (uint32_t i = 0; i < uNumTargets; ++i)
	{
		pD3D11TargetTexture = dynamic_cast<const D3D11Texture*>(pTargets[i]);
		ATLASSERT(pD3D11TargetTexture &&
			(pD3D11TargetTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET)) != 0);

		ID3D11View* pView = pD3D11TargetTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::RENDER_TARGET))].Get();
		//ID3D11RenderTargetView* const* pRTV = reinterpret_cast<ID3D11RenderTargetView* const*>(pView);
		arrRTVs[i] = reinterpret_cast<ID3D11RenderTargetView*>(pView);
	}

	if (pDepth) {
		// Retrieve the depth stencil view
		const D3D11Texture* pD3D11DepthTexture = dynamic_cast<const D3D11Texture*>(pDepth);
		ATLASSERT(pD3D11DepthTexture &&
		(pD3D11DepthTexture->m_uFlags & static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL)) != 0);

		ID3D11View* pDepthStencilView = pD3D11DepthTexture->m_arrResourceViews[GetFirstBitSet(static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL))].Get();
		ID3D11DepthStencilView* pDSV = reinterpret_cast<ID3D11DepthStencilView*>(pDepthStencilView);

		m_spContext->OMSetRenderTargets(uNumTargets, arrRTVs.data(), pDSV);
	}
	else {
		m_spContext->OMSetRenderTargets(uNumTargets, arrRTVs.data(), nullptr);
	}

}

void D3D11Interface::SetDepthStencilState(const RHISwapchain *pSwapchain)
{
	const D3D11Swapchain* pD3D11Swapchain = dynamic_cast<const D3D11Swapchain*>(pSwapchain);

	m_spContext->OMSetDepthStencilState(pD3D11Swapchain->m_spDepthStencilState.Get(), 1);
}

void D3D11Interface::SetBlendState()
{
	ComPtr<ID3D11BlendState> spBlendState{};
	D3D11_BLEND_DESC oBlendStateDesc{};
	ZeroMemory(&oBlendStateDesc, sizeof(oBlendStateDesc));
	oBlendStateDesc.AlphaToCoverageEnable = false;
	oBlendStateDesc.IndependentBlendEnable = false;

	//D3D11_RENDER_TARGET_BLEND_DESC oRTBlendDesc{};
	//oRTBlendDesc.
//	oBlendStateDesc.RenderTarget[0] =

	m_spDevice->CreateBlendState(&oBlendStateDesc, spBlendState.GetAddressOf());
	m_spContext->OMSetBlendState(spBlendState.Get(), nullptr, 0xffffffff);
}

void D3D11Interface::ClearShaders()
{
	m_spContext->VSSetShader(nullptr, nullptr,  0);
	m_spContext->GSSetShader(nullptr, nullptr,  0);
	m_spContext->PSSetShader(nullptr, nullptr,  0);
	m_spContext->CSSetShader(nullptr, nullptr,  0);
}

void D3D11Interface::SetVertexShader(const RHIShader* pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->VSSetShader((ID3D11VertexShader*) pD3D11Shader->m_spShader.Get(), nullptr, 0);

	m_spContext->IASetInputLayout((ID3D11InputLayout*) pD3D11Shader->m_spInputLayout.Get());
	// TODO: Make this generic
	m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Interface::SetGeometryShader(const RHIShader *pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->GSSetShader(
		pD3D11Shader ? (ID3D11GeometryShader*) pD3D11Shader->m_spShader.Get() : nullptr,
		nullptr, 0);
}

void D3D11Interface::SetPixelShader(const RHIShader* pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->PSSetShader((ID3D11PixelShader*) pD3D11Shader->m_spShader.Get(), nullptr, 0);
}

void D3D11Interface::SetComputeShader(const RHIShader* pShader)
{
	const D3D11Shader* pD3D11Shader = dynamic_cast<const D3D11Shader*>(pShader);
	m_spContext->CSSetShader((ID3D11ComputeShader*) pD3D11Shader->m_spShader.Get(), nullptr, 0);
}

void D3D11Interface::CopyTexture(const RHITexture *pSrc, const RHITexture *pDst) const
{
	const D3D11Texture* pD3D11Src = dynamic_cast<const D3D11Texture*>(pSrc);
	const D3D11Texture* pD3D11Dst = dynamic_cast<const D3D11Texture*>(pDst);

	// TODO : Ready
	//D3D11_BOX oBox{};
	//ZeroMemory(&oBox, sizeof(D3D11_BOX));

	//oBox.top = 0;
	//oBox.left = 0;
	//oBox.bottom = pSrc->m_iHeight - 1;
	//oBox.right = pSrc->m_iWidth - 1;
	//oBox.front = 0;
	//oBox.back = 1;

	m_spContext->CopySubresourceRegion(
		pD3D11Dst->m_spInitResource.Get(),
		0, 0, 0, 0,
		pD3D11Src->m_spInitResource.Get(), 0, /*&oBox*/nullptr);
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

void D3D11Interface::Dispatch(uint32_t uGroupCountX, uint32_t uGroupCountY, uint32_t uGroupCountZ)
{
	m_spContext->Dispatch(uGroupCountX, uGroupCountY, uGroupCountZ);
}

void D3D11Interface::Dispatch(const RHITexture *pTex)
{
	uint32_t uX = (pTex->m_iWidth + m_uGroupSizeX - 1) / m_uGroupSizeX;
	uint32_t uY = (pTex->m_iHeight + m_uGroupSizeY - 1) / m_uGroupSizeX;
	uint32_t uZ = 1;

	Dispatch(uX, uY, uZ);
}

//void D3D11Interface::InitTestScene()
//{
//    //----Create shader buffer----//
//    ComPtr<ID3D10Blob> spVSBuffer = compileShader(L"shaders/Default.hlsl", "VSDefaultMain", "vs_5_0");
//    ComPtr<ID3D10Blob> spPSBuffer = compileShader(L"shaders/Default.hlsl", "PSDefaultMain", "ps_5_0");
//
//    //----Create shader instances----//
//    ComPtr<ID3D11VertexShader> spVSInst;
//    HRESULT hr = createShaderInstanceInternal(spVSBuffer.Get(), &spVSInst, EShaderStage::VERTEX);
//    ATLASSERT(hr == S_OK);
//
//    ComPtr<ID3D11PixelShader> spPSInst;
//    hr = createShaderInstanceInternal(spPSBuffer.Get(), &spPSInst, EShaderStage::PIXEL);
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