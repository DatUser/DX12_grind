#include <map>

LPCSTR D3D11Shader::GetShaderModelFromShaderStage(EShaderStage eStage)
{
	static const std::map<EShaderStage, LPCSTR> mapD3D11ShaderStage {
		#define XPAIR(eStage, pTargetName) {eStage, #pTargetName},
			D3D11_SHADER_STAGES
		#undef XPAIR
	};

	return mapD3D11ShaderStage.at(eStage);
}