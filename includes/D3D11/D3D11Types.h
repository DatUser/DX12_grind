#pragma once

#include "Core/Core.h"
#include "RHI/rhi_types.h"

static DXGI_FORMAT arrDXGIFormats[]
{
    // R
    DXGI_FORMAT_R8_UNORM,
    DXGI_FORMAT_R8_UINT,
    DXGI_FORMAT_R16_UNORM,
    DXGI_FORMAT_R16_UINT,
    DXGI_FORMAT_R16_FLOAT,
    DXGI_FORMAT_R32_UINT,
    DXGI_FORMAT_R32_FLOAT,
    // RG
    DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_R16G16_FLOAT,
    DXGI_FORMAT_R16G16_UNORM,
    DXGI_FORMAT_R32G32_FLOAT,
    // RGB
    DXGI_FORMAT_R32G32B32_FLOAT,
    // RGBA
    DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_R32G32B32A32_FLOAT
};

static D3D11_PRIMITIVE_TOPOLOGY arrD3D11Topologies[]
{
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ
};