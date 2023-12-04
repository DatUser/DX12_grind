#include "vertexshader.h"

VShader::VShader(Microsoft::WRL::ComPtr<ID3D11Device> spDevice)
{
}

HRESULT VShader::InitShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader **ppVertexShader)
{
    return E_NOTIMPL;
}

HRESULT VShader::InitShaderBuffer()
{
return E_NOTIMPL;
}
