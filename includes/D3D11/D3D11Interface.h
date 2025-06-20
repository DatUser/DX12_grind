#pragma once

#include "Core/Core.h"
#include "Core/fwdtypes.h"
#include "RHI/rhi.h"

#include <array>
#include <memory>

class D3D11Swapchain;
class D3D11Buffer;
class D3D11Texture;

enum class EShaderStage : uint32_t;

using InputLayoutFormat = std::tuple<DXGI_FORMAT, LPCSTR, unsigned int>;

/**
 *  Initializes and handles the device, the swapchain and the context
*/
class D3D11Interface : public RHI
{
public:
    static void CreateInterface()
    {
        RHI::m_spGFXInterface = std::make_unique<D3D11Interface>();
    }

	inline static D3D11Interface* GetInterface()
	{
		RHI* p = RHI::m_spGFXInterface.get();
		return dynamic_cast<D3D11Interface*>(p);
	}

    D3D11Interface();
    ~D3D11Interface() override;

    /**
     * @brief Create a Shader Instance object
     *
     * @param pShaderBuffer
     * @param[out] pShaderInstance
     * @return HRESULT S_OK on sucess
     */
    HRESULT createShaderInstanceInternal(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage);

    /**
     * @brief Create a Buffer object in GPU memory
     *
     * @param pData
     * @param uByteWidth
     * @param opBuffer
     * @param uFlags
     * @return HRESULT
     */
    HRESULT createBufferInternal(
		D3D11Buffer* pRHIBuffer
    );

	HRESULT createTextureInternal(
		D3D11Texture* pTexture
		);

	HRESULT createRTVInternal(
		D3D11Texture* pTexture
	);

	HRESULT createDSVInternal(
		D3D11Texture* pTexture
		);

	HRESULT createSRVInternal(
		D3D11Texture* pTexture
	);

	HRESULT createUAVInternal(
		D3D11Texture* pTexture
	);

	// Depth Stencil State
	HRESULT createDSSInternal(
		ID3D11DepthStencilState ** pDSState,
		bool enableStencilTest = false
	);

	HRESULT createSwapchainInternal(
		D3D11Swapchain* pSwapchain,
		HWND hWnd,
		uint32_t uWidth,
		uint32_t uHeight
	);

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(
		void* pData,
		unsigned int uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess=ERHICPUAccessFlags::NONE,
		ERHIBufferUsage eUsage=ERHIBufferUsage::DEFAULT
	) override;

	virtual std::shared_ptr<RHITexture> CreateTexture(
		void* pData,
		int iWidth,
		int iHeight,
		ETextureFormat eFormat,
		uint32_t uFlags = 0
	) override;

	virtual void SetBufferData(const RHIBuffer* pBuffer, const void* pData) override;

    void ClearRenderView(const RHITexture* pTexture, float fR, float fG, float fB, float fA=1.f) override;
	void ClearDepthStencilView(const RHITexture* pTexture) override;
	void ClearUnorderedAccessView(const RHITexture* pTexture, float fR, float fG, float fB, float fA=1.f) override;

    std::shared_ptr<RHISwapchain> CreateSwapchain(
    	HWND hWnd,
    	uint32_t uWidth,
    	uint32_t uHeight
    ) override;

	std::shared_ptr<RHIViewport> CreateViewport(
		HWND hWnd,
		uint32_t uWidth,
		uint32_t uHeight
	) override;
	void SetViewport(const RHIViewport* pViewport) override;

	void SetRasterizerState(
		ECullMode eMode,
		bool bIsWireframe = false
	) override;

	std::shared_ptr<RHIShader> CreateShader(ERendererShaders eShader) override;
	void SetVertexBuffer(const RHIBuffer* pBuffer) override;
	void SetIndexBuffer(const RHIBuffer* pBuffer) override;
	void SetBuffer(const RHIBuffer *pBuffer, ShaderType eShaderStage) override;
	void SetTexture(uint32_t uSlot, const RHITexture* pTexture, ShaderType eShaderStage, bool bIsUAV=false) override;
	//void SetT

	void SetContextRenderTarget(const RHITexture* pTarget, const RHITexture* pDepth) override;
	void SetContextRenderTargets(RHITexture* const* pTargets, uint32_t uNumTargets, const RHITexture* pDepth) override;
	void SetDepthStencilState(const RHISwapchain *pSwapchain) override;
	void SetBlendState() override;

	template <EShaderStage eShaderStage>
	void SetBufferInternal(/* TODO : Add Slot*/const RHIBuffer* pBuffer);// {}

	template <EShaderStage eShaderStage>
	void SetTextureInternal(uint32_t uSlot, const RHITexture* pTexture);// {}

	template <EShaderStage eShaderStage>
	void SetUAVInternal(uint32_t uSlot, const RHIResource* pResource);// {}

	void ClearShaders() override;
	void SetVertexShader(const RHIShader* pShader) override;
	void SetGeometryShader(const RHIShader* pShader) override;
	void SetPixelShader(const RHIShader* pShader) override;
	void SetComputeShader(const RHIShader* pShader) override;

	void CopyTexture(const RHITexture* pSrc, const RHITexture* pDst) const override;

	void DrawIndexed(
		uint32_t uIndexCount,
		uint32_t uIndexOffset,
		uint32_t uVertexOffset
	) override;

	void Dispatch(
		uint32_t uGroupCountX,
		uint32_t uGroupCountY,
		uint32_t uGroupCountZ
	) override;

	void Dispatch(
		const RHITexture *pTex
	) override;

    /**
     * @brief Create a Input Layout containing information about buffer data layout
     *
     * @param pVSBuffer
     * @param eFormat
     * @param pName
     * @param pLayout
     * @return HRESULT
     */
	// To remove
    HRESULT createInputLayout(ID3D10Blob* pVSBuffer, std::vector<InputLayoutFormat> vInputLayout, void** pLayout);

    /**
     * @brief Adds buffer as input of given Vertex buffer
     *
     * @param pBuffers
     * @param nBuffers
     * @param pVSBuffer
     * @param uFlags
     */
	// To remove
    void AddBuffers(std::vector<ID3D11Buffer*> vVertBuffers, ID3D11Buffer* pIdxBuffer, Microsoft::WRL::ComPtr<ID3D11InputLayout> spVertsLayout, UINT uStride, UINT uOffset);

    //void Draw();

	constexpr static D3D11_CULL_MODE CastToInterfaceCullMode(ECullMode eMode) {
		constexpr std::array<D3D11_CULL_MODE, static_cast<uint32_t>(ECullMode::_size)> arrCullModes = {
			D3D11_CULL_BACK,
			D3D11_CULL_FRONT,
			D3D11_CULL_NONE
		};

		return arrCullModes[static_cast<uint32_t>(eMode)];
	}

private:
    // Handles memory stuff
    ComPtr<ID3D11Device> m_spDevice;
    // Handles gfx pipeline stuff
    ComPtr<ID3D11DeviceContext> m_spContext;


	ComPtr<IDXGIFactory>	m_spFactory;

    std::vector<ComPtr<ID3D11Buffer>> m_vBuffers;
};
