#pragma once

#include <array>
#include <string_view>
#include <tuple>

#include "D3D11/D3D11Interface.h"
#include "Engine/window.h"
#include "Engine/renderer.h"
#include "RHI/rhi_shader.h"

#ifndef GFX_INTERFACE
#define GFX_INTERFACE "DX11"
#endif

typedef void (*RHICreationInterface)();

template <size_t N>
using RHIInterfaceMap = std::array<std::pair<std::string_view, RHICreationInterface>, N>;

// Handled interfaces
constexpr RHIInterfaceMap<1> mapInterfaces{
	{
		{"DX11", D3D11Interface::CreateInterface}
    }
};

/**
 * @brief Finds a value by name in array and returns default if not found
 *
 * @tparam T
 * @tparam defaultValue
 * @tparam size
 * @param sKey
 * @param arr
 * @return constexpr T
 */
template <typename T, T defaultValue, std::size_t size>
constexpr T find_else_default(
	const std::string_view& sKey,
	const std::array<std::pair<std::string_view, T>, size>& arr
)
{
	for (const auto& oPair : arr)
    {
        if (oPair.first == sKey)
            return oPair.second;
    }

    return defaultValue;
}

constexpr RHICreationInterface CreateInterface = find_else_default<RHICreationInterface, D3D11Interface::CreateInterface, mapInterfaces.size()>(GFX_INTERFACE, mapInterfaces);

using ShaderData = std::tuple<std::wstring_view, std::string_view, EShaderStage>;
constexpr std::array<ShaderData, static_cast<unsigned int>(ERendererShaders::_size)> mapD3D11ShadersPaths{
	ShaderData{L"shaders/Default.hlsl", "VSDefaultMain", EShaderStage::VERTEX},
	ShaderData{L"shaders/Default.hlsl", "GS_Main", EShaderStage::GEOMETRY},
	ShaderData{L"shaders/Default.hlsl", "PSDefaultMain", EShaderStage::PIXEL}
};