#pragma once

#include "Core/Core.h"
#include "RHI/fwd_rhi.h"
#include "RHI/rhi_shader.h"


// Source for target names:
// https://learn.microsoft.com/fr-fr/windows/win32/direct3dhlsl/specifying-compiler-targets
#define D3D11_SHADER_STAGES 				\
	XPAIR(EShaderStage::VERTEX, vs_5_0)		\
	XPAIR(EShaderStage::HULL, hs_5_0)		\
	XPAIR(EShaderStage::DOMAIN, ds_5_0)		\
	XPAIR(EShaderStage::GEOMETRY, gs_5_0)	\
	XPAIR(EShaderStage::PIXEL, ps_5_0)		\
	XPAIR(EShaderStage::COMPUTE, cs_5_0)

class D3D11Shader : public RHIShader
{
	friend class D3D11Interface;
public:
	D3D11Shader(
		const std::wstring&	wsFilepath,
		const std::string& 	sEntryPoint,
		EShaderStage 		eStage
	);

	virtual ~D3D11Shader() override;

	virtual bool Compile() override;

	inline static LPCSTR GetShaderModelFromShaderStage(EShaderStage eStage);
private:
	ComPtr<ID3D10Blob>	m_spShaderBuffer;
	ComPtr<ID3D11InputLayout> m_spInputLayout;
	ComPtr<ID3D11DeviceChild> m_spShader;
};

#include "D3D11/D3D11Shader.hpp"