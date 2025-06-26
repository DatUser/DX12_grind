#pragma once
#include <cstdint>

namespace RendererRessources
{
	// SRV Slots
	constexpr uint32_t uGBufferAlbedoSRV = 0;
	constexpr uint32_t uGBufferNormalsSRV = 1;
	constexpr uint32_t uGBufferPosSRV = 2;

	// UAV Slots
	constexpr uint32_t uLightsUAV = 0;


	// CBuffer Slots
	constexpr uint32_t uViewProjCB = 0;
	constexpr uint32_t uLightCB = 1;
}
