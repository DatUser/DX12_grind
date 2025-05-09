#include "D3D11/D3D11Swapchain.h"

#include "Core/asserts.h"

#include "D3D11/D3D11Texture.h"

D3D11Swapchain::D3D11Swapchain(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
{
	ATLASSERT(D3D11Interface::GetInterface()->createSwapchainInternal(this, hWnd, uWidth, uHeight));
	RHITexture* pTex= m_spBackBufferRTV.get();
	D3D11Texture* pBackBufferRTV = dynamic_cast<D3D11Texture*>(pTex);
	ATLASSERT(D3D11Interface::GetInterface()->createRTVInternal(pBackBufferRTV) == S_OK);
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
