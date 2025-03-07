#include "D3D11/D3D11Common.h"

#include "Core/asserts.h"

ComPtr<ID3D10Blob> compileShader(
	LPCWSTR pFilename, LPCSTR pEntryPoint, LPCSTR pShaderModel, const D3D10_SHADER_MACRO *pDefines)
{
	ComPtr<ID3D10Blob> spShaderBuffer;
    ComPtr<ID3DBlob> spErrorBuffer;

    HRESULT hr = D3DCompileFromFile(
        pFilename,
        pDefines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        pEntryPoint,
        pShaderModel,
        0,
        0,
        &spShaderBuffer,
        &spErrorBuffer
    );

    if (FAILED(hr))
    {
        LOG_SHADER_COMPILE_ERROR(spErrorBuffer);
        return nullptr;
    }

    return spShaderBuffer;
}