
#pragma once

#include <stdint.h>

class RHIViewport
{
public:
	RHIViewport() {}
	virtual ~RHIViewport() {}

	inline virtual void SetWidth(uint32_t uWidth) = 0;
	inline virtual void SetHeight(uint32_t uHeight) = 0;
private:
};