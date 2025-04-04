#pragma once

#include "RHI/rhi_viewport.h"
#include "Core/Core.h"

class D3D11Viewport : public RHIViewport
{
public:
	D3D11Viewport();
	D3D11Viewport(uint32_t uWidth, uint32_t uHeight, uint32_t uTopLeftX, uint32_t uTopLeftY);

	virtual ~D3D11Viewport();

	inline virtual void SetWidth(uint32_t uWidth) override
	{
		m_viewport.Width = uWidth;
	}

	inline virtual void SetHeight(uint32_t uHeight) override
	{
		m_viewport.Height = uHeight;
	}
private:
	D3D11_VIEWPORT m_viewport;
};
