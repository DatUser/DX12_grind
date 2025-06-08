#include "D3D11/D3D11Swapchain.h"

#include "Core/asserts.h"

#include "D3D11/D3D11Texture.h"

D3D11Swapchain::D3D11Swapchain(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
{
	ATLASSERT(D3D11Interface::GetInterface()->createSwapchainInternal(this, hWnd, uWidth, uHeight) == S_OK);

	// Binding back buffer texture
	m_spBackBufferRTV = std::make_shared<D3D11Texture>();
	//RHITexture* pRHITex = m_spBackBufferRTV.get();
	D3D11Texture* pBackBufferRTV = dynamic_cast<D3D11Texture*>(m_spBackBufferRTV.get());
	ATLASSERT(m_spSwapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferRTV->m_spInitResource)) == S_OK);

	// Setting RT flag (needed to be used as context RT)
	pBackBufferRTV->m_uFlags |= ERHITextureFlags::RENDER_TARGET;
	pBackBufferRTV->m_iWidth = uWidth;
	pBackBufferRTV->m_iHeight = uHeight;
	// Creating view for back buffer
	ATLASSERT(D3D11Interface::GetInterface()->createRTVInternal(pBackBufferRTV) == S_OK);

	// Create depth stencil state
	ATLASSERT(D3D11Interface::GetInterface()->createDSSInternal(&m_spDepthStencilState) == S_OK);

	// Create depth stencil texture
	m_spDepthStencilView =
		RHI::GetInterface()->CreateTexture(
				nullptr,
				uWidth,
				uHeight,
				ETextureFormat::D24_UNORM_S8_UINT,
				static_cast<uint32_t>(ERHITextureFlags::DEPTH_STENCIL));

	D3D11Texture* pDepthStencilView = dynamic_cast<D3D11Texture*>(m_spDepthStencilView.get());
	
	// Creating view for depth stencil
	ATLASSERT(D3D11Interface::GetInterface()->createDSVInternal(pDepthStencilView) == S_OK);

}

void D3D11Swapchain::Present()
{
	HRESULT hr = m_spSwapchain->Present(0, 0);
	if (hr != S_OK)
	{
		LOG_ERROR(hr);
		LOG_LAST_ERROR();
		//LOG_ERROR(m_spDevice->GetDeviceRemovedReason());
	}

	ATLASSERT(hr == S_OK);
}