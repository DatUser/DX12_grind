#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"
#include "RHI/rhi.h"

class Camera;
class D3D11Buffer;

enum EShaderStage
{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};


/**
 *  Initializes and handles the device, the swapchain and the context
*/
class D3D11Interface : public RHI
{
public:
    static void CreateInterface(HWND hWnd, Camera* pCamera)
    {
        RHI::m_spGFXInterface = std::make_unique<D3D11Interface>(hWnd, pCamera);
    }

    D3D11Interface(HWND hWnd, Camera* pCamera);
    ~D3D11Interface();

    /**
     * @brief Create a Shader Instance object
     *
     * @param pShaderBuffer
     * @param[out] pShaderInstance
     * @return HRESULT S_OK on sucess
     */
    HRESULT createShaderInstance(ID3D10Blob* pShaderBuffer, void** pShaderInstance, EShaderStage eShaderStage);

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
		const void* pData,
		UINT uByteWidth,
		D3D11Buffer* pRHIBuffer,
		D3D11_USAGE eUsage = D3D11_USAGE_DEFAULT
    );

    virtual void PresentFrame() override;

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(
		const void* pData,
		unsigned int uByteWidth,
		ERHIBufferFlags eFlags,
		ECPUAccessFlags eCPUAccess=ECPUAccessFlags::NONE
	) override;

    virtual void CreateSwapchain() override;
    virtual void ClearRenderView() override;

    void ClearRenderView(float r, float g, float b, float a = 1.f);

    /**
     * @brief Create a Input Layout containing information about buffer data layout
     *
     * @param pVSBuffer
     * @param eFormat
     * @param pName
     * @param pLayout
     * @return HRESULT
     */
    HRESULT createInputLayout(ID3D10Blob* pVSBuffer, DXGI_FORMAT eFormat, LPCSTR pName, void** pLayout);

    /**
     * @brief Adds buffer as input of given Vertex buffer
     *
     * @param pBuffers
     * @param nBuffers
     * @param pVSBuffer
     * @param uFlags
     */
    void AddBuffers(std::vector<ID3D11Buffer*> vVertBuffers, ID3D11Buffer* pIdxBuffer, Microsoft::WRL::ComPtr<ID3D11InputLayout> spVertsLayout, UINT uStride, UINT uOffset);

    void Draw();

    /**
     * @brief Inits the default shader program
     */
    void InitTestScene();
private:
    // Handles memory stuff
    ComPtr<ID3D11Device> m_spDevice;
    ComPtr<IDXGISwapChain> m_spSwapchain;
    // Handles gfx pipeline stuff
    ComPtr<ID3D11DeviceContext> m_spContext;
    ComPtr<ID3D11RenderTargetView> m_spTarget;

    std::vector<ComPtr<ID3D11Buffer>> m_vBuffers;

    // Viewport camera
    Camera* m_pMainCamera;
};
