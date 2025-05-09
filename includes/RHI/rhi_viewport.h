
#pragma once

#include <stdint.h>
#include <memory>

#include "Engine/camera.h"

class RHISwapchain;

class RHIViewport
{
public:
	RHIViewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight);
	virtual ~RHIViewport() = default;

	Camera* GetCamera() const
	{
		return m_spMainCamera.get();
	}

	RHISwapchain* GetSwapchain() const
	{
		return m_spSwapchain.get();
	}


	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void SetWidth(uint32_t uWidth) = 0;
	virtual void SetHeight(uint32_t uHeight) = 0;

protected:
	std::shared_ptr<Camera> 		m_spMainCamera;
	std::shared_ptr<RHISwapchain>	m_spSwapchain;
};
