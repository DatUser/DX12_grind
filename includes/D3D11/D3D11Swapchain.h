#pragma once

#include "Core/Core.h"

#include "RHI/rhi_swapchain.h"

class D3D11Swapchain : public RHISwapchain
{
	friend class D3D11Interface;

public:
	D3D11Swapchain(HWND hWnd, uint32_t uWidth, uint32_t uHeight);
	~D3D11Swapchain() override = default;

	void Present() override;

private:
	ComPtr<IDXGISwapChain>	m_spSwapchain;
};
