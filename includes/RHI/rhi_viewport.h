
#pragma once

#include <stdint.h>
#include <memory>

#include "Engine/camera.h"

class RHIViewport
{
public:
	RHIViewport();
	virtual ~RHIViewport() = default;

	inline virtual Camera* GetCamera() const
	{
		return m_spMainCamera.get();
	}

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void SetWidth(uint32_t uWidth) = 0;
	virtual void SetHeight(uint32_t uHeight) = 0;
protected:
	std::shared_ptr<Camera> 	m_spMainCamera;
};