#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"
#include "RHI/rhi_buffer.h"

class D3D11Buffer: public RHIBuffer
{
	friend class D3D11Interface;
public:
	D3D11Buffer(/* args */);
	~D3D11Buffer();

	virtual void Init(void* pData, unsigned int uByteWidth, ERHIBufferFlags eFlags) override final;

	virtual void Update() override final;

private:
	ComPtr<ID3D11Buffer> pInitResource;
};
