#pragma once

#include "RHI/rhi_buffer.h"
#include "RHI/rhi_shader.h"

#include <memory>
#include <variant>

enum class ERendererShaders : uint8_t;
class RHIBuffer;
class RHIShader;
class RHIViewport;

struct HWND__;
typedef struct HWND__ *HWND;

using ShaderType = std::variant<
				std::integral_constant<EShaderStage, EShaderStage::VERTEX>,
				std::integral_constant<EShaderStage, EShaderStage::PIXEL>>;

/**
 *  This is the rendering interface
 *  Its main purpose is to request frames
 *  and execute command lists
*/
class RHI
{
public:
    virtual ~RHI() = default;


    /**
     *  Present frame to user
    */
    virtual void PresentFrame() = 0;

    virtual void ClearRenderView() = 0;
    //virtual void Draw() = 0;

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(
		void* pData,
		unsigned int uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess = ERHICPUAccessFlags::NONE,
		ERHIBufferUsage eUsage = ERHIBufferUsage::DEFAULT
	) = 0;

	/**
	 * @brief Copies pData to pBuffer CPU memory then uploads it to the GPU
	 *
	 * @param pData
	 * @return true
	 * @return false
	 */
    virtual void SetBufferData(const RHIBuffer* pBuffer, const void* pData) = 0;

    virtual void CreateSwapchain(HWND hWnd) = 0;

	virtual std::shared_ptr<RHIViewport> CreateViewport(uint32_t uWidth, uint32_t uHeight) = 0;
	virtual void SetViewport(const RHIViewport* pViewport) = 0;

	virtual std::shared_ptr<RHIShader> CreateShader(ERendererShaders eShader) = 0;
	virtual void SetVertexShader(const RHIShader* pShader) = 0;
	virtual void SetGeometryShader(const RHIShader * get) = 0;
	virtual void SetPixelShader(const RHIShader* pShader) = 0;

	virtual void SetVertexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetIndexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetBuffer(const RHIBuffer *pBuffer, ShaderType eShaderStage) = 0;

	virtual void DrawIndexed(uint32_t uIndexCount, uint32_t uIndexOffset, uint32_t uVertexOffset) = 0;


    //inline static auto&& GetInterface()
	inline static RHI* GetInterface()
	{
		//return m_spGFXInterface;
		return m_spGFXInterface.get();
	}
protected:
	static std::unique_ptr<RHI>	m_spGFXInterface;
};