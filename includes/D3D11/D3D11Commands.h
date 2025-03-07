#pragma once

#include "RHI/fwd_rhi.h"

class D3D11Commands
{
public:
	void SetVertexBuffers(RHIBuffer* pBuffer);

private:
	D3D11Commands();
	~D3D11Commands();

	static std::unique_ptr<D3D11Commands> m_spInstance;

	//std::unique_ptr<>

    ComPtr<ID3D11DeviceContext> m_spContext;
};