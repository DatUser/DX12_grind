
#pragma once

#include <stdint.h>
#include <memory>

#include "camera.h"

class RHIViewport
{
public:
	RHIViewport()
	: m_spMainCamera(std::make_shared<Camera>())
	{}
	virtual ~RHIViewport() = default;

	inline virtual Camera* GetCamera() const
	{
		return m_spMainCamera.get();
	}

	virtual void SetWidth(uint32_t uWidth) = 0;
	virtual void SetHeight(uint32_t uHeight) = 0;
protected:
	std::shared_ptr<Camera> 	m_spMainCamera;
};
