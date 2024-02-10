#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"

/**
 * @brief Tries to compile given shader file
 *
 * @param pFilename     Shader file path
 * @param pEntryPoint   Entry point name (shader method name)
 * @param pShaderModel  Feature level
 * @param pDefines      Shader macro definitions
 * @return ComPtr to shader buffer on success, otherwise nullptr
 */
ComPtr<ID3D10Blob> compileShader(
    LPCWSTR pFilename,
    LPCSTR pEntryPoint,
    LPCSTR pShaderModel,
    const D3D10_SHADER_MACRO* pDefines = nullptr);