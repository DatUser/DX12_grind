#include "RHI/rhi_shader.h"

RHIShader::RHIShader(
	const std::wstring& wsFilepath,
	const std::string& sEntryPoint,
	EShaderStage eStage)
: m_wsFilepath(wsFilepath)
, m_sEntryPoint(sEntryPoint)
, m_eStage(eStage)
, m_bIsCompiled(false)
{
}
RHIShader::~RHIShader()
{
}