#include "D3D11/D3D11Shader.h"

#include "D3D11/D3D11Common.h"

#include <map>
#include "D3D11Shader.h"

#include "Core/asserts.h"

D3D11Shader::D3D11Shader(
		const std::wstring& wsFilepath,
		const std::string&	sEntryPoint,
		EShaderStage	 	eStage
	)
: RHIShader(
	wsFilepath,
	sEntryPoint,
	eStage
	)
{

}

D3D11Shader::~D3D11Shader()
{
}

bool D3D11Shader::Compile()
{
	m_spShaderBuffer = compileShader(
								m_wsFilepath.c_str(),
								m_sEntryPoint.c_str(),
								GetShaderModelFromShaderStage(m_eStage),
								nullptr /*TODO: add defines*/
								);

	HRESULT hr = D3D11Interface::GetInterface()->createShaderInstanceInternal(
		m_spShaderBuffer.Get(),
		&m_spShader,
		m_eStage
		);
	if (FAILED(hr))
		LOG_ERROR(hr)

	std::vector<InputLayoutFormat> vInputLayout;
	vInputLayout.push_back({ DXGI_FORMAT_R32G32B32_FLOAT, "POSITION", 0 });
	vInputLayout.push_back({ DXGI_FORMAT_R32G32B32_FLOAT, "NORMAL", D3D11_APPEND_ALIGNED_ELEMENT });

	if (m_eStage == EShaderStage::VERTEX)
		ATLASSERT(
			D3D11Interface::GetInterface()->createInputLayout(
				m_spShaderBuffer.Get(),          //Shader text data
				vInputLayout,                     //Structure semantic name
				&m_spInputLayout
			) == S_OK);

	return (m_bIsCompiled);
}