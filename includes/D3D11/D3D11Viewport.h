#pragma once

#include "RHI/rhi_viewport.h"
#include "Core/Core.h"

class D3D11Viewport : public RHIViewport
{
	friend class D3D11Interface;
public:
	D3D11Viewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight);

	~D3D11Viewport() override;

	uint32_t GetWidth() const
	{
		return m_oViewport.Width;
	}

	uint32_t GetHeight() const
	{
		return m_oViewport.Height;
	}

	// TODO: Implement functional stuff
	void SetWidth(uint32_t uWidth) override
	{
		m_oViewport.Width = uWidth;
	}

	void SetHeight(uint32_t uHeight) override
	{
		m_oViewport.Height = uHeight;
	}
private:
	D3D11_VIEWPORT m_oViewport;
};
