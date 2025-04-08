
#pragma once

#include <stdint.h>
#include <memory>

class Camera;

class RHIViewport
{
public:
	RHIViewport() {}
	virtual ~RHIViewport() {}

	inline virtual void SetWidth(uint32_t uWidth) = 0;
	inline virtual void SetHeight(uint32_t uHeight) = 0;
protected:
	std::shared_ptr<Camera> 	m_spMainCamera;
};