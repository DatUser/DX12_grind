#pragma once

#include "RHI/rhi_buffer.h"
#include "RHI/rhi_shader.h"

#include <memory>
#include <variant>

class RHISwapchain;
enum class ETextureFormat : uint8_t;
enum class ERendererShaders : uint8_t;

class RHITexture;
class RHIViewport;

struct HWND__;
typedef struct HWND__ *HWND;

using ShaderType = std::variant<
				std::integral_constant<EShaderStage, EShaderStage::VERTEX>,
				std::integral_constant<EShaderStage, EShaderStage::GEOMETRY>,
				std::integral_constant<EShaderStage, EShaderStage::COMPUTE>
				//std::integral_constant<EShaderStage, EShaderStage::PIXEL>,
				//std::integral_constant<EShaderStage, EShaderStage::COMPUTE>
			>;

using ConstBool = std::variant<
		std::bool_constant<true>,
		std::bool_constant<false>
	>;

enum class ECullMode
{
	CULL_BACK,
	CULL_FRONT,
	NONE,

	_size
};

/**
 *  This is the rendering interface
 *  Its main purpose is to request frames
 *  and execute command lists
*/
class RHI
{
public:
    virtual ~RHI() = default;

    virtual void ClearRenderView(const RHITexture* pTexture, float fR, float fG, float fB, float fA=1.f) = 0;
    virtual void ClearDepthStencilView(const RHITexture* pTexture) = 0;
    virtual void ClearUnorderedAccessView(const RHITexture* pTexture, float fR, float fG, float fB, float fA=1.f) = 0;
    //virtual void Draw() = 0;

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(
		void* pData,
		unsigned int uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess = ERHICPUAccessFlags::NONE,
		ERHIBufferUsage eUsage = ERHIBufferUsage::DEFAULT
		) = 0;

	virtual std::shared_ptr<RHITexture> CreateTexture(
		const std::string& sFilePath,
		ETextureFormat eFormat
		) = 0;

	virtual std::shared_ptr<RHITexture> CreateTexture(
		void* pData,
		int iWidth,
		int iHeight,
		ETextureFormat eFormat,
		uint32_t uFlags = 0
		) = 0;

	/**
	 * @brief Copies pData to pBuffer CPU memory then uploads it to the GPU
	 *
	 * @param pData
	 * @return true
	 * @return false
	 */
    virtual void SetBufferData(const RHIBuffer* pBuffer, const void* pData) = 0;

    virtual std::shared_ptr<RHISwapchain> CreateSwapchain(HWND hWnd, uint32_t uWidth, uint32_t uHeight) = 0;

	virtual void SetRasterizerState(ECullMode eMode, bool bIsWireframe = false) = 0;

	virtual std::shared_ptr<RHIViewport> CreateViewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight) = 0;
	virtual void SetViewport(const RHIViewport* pViewport) = 0;

	virtual std::shared_ptr<RHIShader> CreateShader(ERendererShaders eShader) = 0;
	virtual void ClearShaders() = 0;
	virtual void SetVertexShader(const RHIShader* pShader) = 0;
	virtual void SetGeometryShader(const RHIShader* pShader) = 0;
	virtual void SetPixelShader(const RHIShader* pShader) = 0;
	virtual void SetComputeShader(const RHIShader* pShader) = 0;

	virtual void SetVertexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetIndexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetBuffer(uint32_t uSlot, const RHIBuffer* pBuffer, ShaderType eShaderStage) = 0;
	virtual void SetTexture(uint32_t uSlot, const RHITexture* pTexture, ShaderType eShaderStage, bool bIsUAV=false) = 0;

	virtual void SetContextRenderTarget(const RHITexture* pTargets, const RHITexture* pDepth) = 0;
	virtual void SetContextRenderTargets(RHITexture* const* pTargets, uint32_t uNumTargets, const RHITexture* pDepth) = 0;
	virtual void SetDepthStencilState(const RHISwapchain* pSwapchain) = 0;
	virtual void SetBlendState() = 0;

	virtual void CopyTexture(const RHITexture* pSrc, const RHITexture* pDst) const = 0;

	virtual void DrawIndexed(uint32_t uIndexCount, uint32_t uIndexOffset, uint32_t uVertexOffset) = 0;

	virtual void Dispatch(uint32_t uGroupCountX, uint32_t uGroupCountY, uint32_t uGroupCountZ) = 0;
	virtual void Dispatch(const RHITexture* pTex) = 0;


    //inline static auto&& GetInterface()
	inline static RHI* GetInterface()
	{
		//return m_spGFXInterface;
		return m_spGFXInterface.get();
	}
protected:
	static std::unique_ptr<RHI>	m_spGFXInterface;
	static constexpr uint32_t m_uGroupSizeX = 8;
	static constexpr uint32_t m_uGroupSizeY = 8;
	static constexpr uint32_t m_uGroupSizeZ = 1;
};