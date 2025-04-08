#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"
#include "RHI/rhi.h"

#include <memory>

class D3D11Buffer;

enum class EShaderStage : uint32_t;


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
    virtual ~D3D11Interface();

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

    virtual void PresentFrame() override;

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(
		void* pData,
		unsigned int uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess=ERHICPUAccessFlags::NONE,
		ERHIBufferUsage eUsage=ERHIBufferUsage::DEFAULT
	) override;

	virtual bool UploadBuffer(const std::shared_ptr<RHIBuffer>& spBuffer) override;

    virtual void ClearRenderView() override;
    void ClearRenderView(float r, float g, float b, float a = 1.f);

    virtual void CreateSwapchain(HWND hWnd) override;
	virtual std::shared_ptr<RHIViewport> CreateViewport(uint32_t uWidth, uint32_t uHeight) override;

	virtual std::shared_ptr<RHIShader> CreateShader(ERendererShaders eShader) override;
	virtual void SetVertexBuffer(const RHIBuffer* pBuffer) override;
	virtual void SetIndexBuffer(const RHIBuffer* pBuffer) override;
	virtual void SetBuffer(const RHIBuffer *pBuffer, ShaderType eShaderStage) override;

	template <EShaderStage eShaderStage>
	void SetBufferInternal(const RHIBuffer* pBuffer){}

	virtual void SetVertexShader(const RHIShader* pShader) override;
	virtual void SetPixelShader(const RHIShader* pShader) override;

	virtual void DrawIndexed(
		uint32_t uIndexCount,
		uint32_t uIndexOffset,
		uint32_t uVertexOffset
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
    HRESULT createInputLayout(ID3D10Blob* pVSBuffer, DXGI_FORMAT eFormat, LPCSTR pName, void** pLayout);

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
private:
    // Handles memory stuff
    ComPtr<ID3D11Device> m_spDevice;
    // Handles gfx pipeline stuff
    ComPtr<ID3D11DeviceContext> m_spContext;


	ComPtr<IDXGIFactory>	m_spFactory;
    ComPtr<IDXGISwapChain> 	m_spSwapchain;
    ComPtr<ID3D11RenderTargetView> m_spTarget;

    std::vector<ComPtr<ID3D11Buffer>> m_vBuffers;
};
