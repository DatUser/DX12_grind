#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

class VShader
{
public:
    VShader(Microsoft::WRL::ComPtr<ID3D11Device> spDevice);

    /**
     * @brief Initializes shader buffer and return init error code
     *
     * @param pShaderBytecode
     * @param BytecodeLength
     * @param ppVertexShader
     * @return HRESULT
     */
    HRESULT InitShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader **ppVertexShader);

    /**
     * @brief Initialiazes shader input buffer
     */
    HRESULT InitShaderBuffer(/*Tuple typedefed with CreateBuffer Func arguments*/);
private:
};