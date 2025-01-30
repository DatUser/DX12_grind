#pragma once


enum class RHIFormat : uint32_t
{
    // R
    R8_UNORM,
    R8_UINT,
    R16_UNORM,
    R16_UINT,
    R16_FLOAT,
    R32_UINT,
    R32_FLOAT,
    // RG
    R8G8_UNORM,
    R16G16_FLOAT,
    R16G16_UNORM,
    R32G32_FLOAT,
    // RGB
    R32G32B32_FLOAT,
    // RGBA
    R8G8B8A8_UNORM,
    R32G32B32A32_FLOAT
};

enum class RHIPrimitiveTopology : uint32_t
{
	TRIANGLELIST,
	TRIANGLELIST_ADJ
};