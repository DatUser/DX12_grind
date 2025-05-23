#pragma once

#include <string>

#ifdef DOMAIN
#undef DOMAIN
#endif

enum class EShaderStage : uint32_t
{
	VERTEX,
	HULL,
	DOMAIN,
	GEOMETRY,
	PIXEL,

	AMPLIFICATION,
	MESH,

	COMPUTE
};

class RHIShader
{
public:
	RHIShader(
		const std::wstring& wsFilepath,
		const std::string& 	sEntryPoint,
		EShaderStage eStage
	);

	virtual ~RHIShader();

	virtual bool Compile() = 0;

	bool IsCompiled() const { return m_bIsCompiled; }

protected:
	std::wstring	m_wsFilepath;
	std::string 	m_sEntryPoint;
	EShaderStage	m_eStage;
	bool 			m_bIsCompiled;
};