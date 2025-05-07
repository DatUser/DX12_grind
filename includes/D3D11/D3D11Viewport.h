#pragma once

#include "RHI/rhi_viewport.h"
#include "Core/Core.h"

class D3D11Viewport : public RHIViewport
{
	friend class D3D11Interface;
public:
	D3D11Viewport();
	D3D11Viewport(uint32_t uWidth, uint32_t uHeight, uint32_t uTopLeftX, uint32_t uTopLeftY);

	virtual ~D3D11Viewport();

	uint32_t GetWidth() const
	{
		return m_oViewport.Width;
	}

	uint32_t GetHeight() const
	{
		return m_oViewport.Height;
	}

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
