#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"
#include "RHI/rhi_buffer.h"

#include <map>

class D3D11Buffer: public RHIBuffer
{
	friend class D3D11Interface;
public:
	~D3D11Buffer();

	virtual void Update() override final;

	static inline unsigned int CastToInterfaceFlag(ERHIBufferFlags eFlags)
	{
		const static std::map<ERHIBufferFlags, unsigned int> mapFlags{
			{ERHIBufferFlags::CONSTANT, D3D11_BIND_CONSTANT_BUFFER},
			{ERHIBufferFlags::INDEX, D3D11_BIND_INDEX_BUFFER},
			{ERHIBufferFlags::VERTEX, D3D11_BIND_VERTEX_BUFFER}
		};

		return mapFlags.at(eFlags);
	}

	static inline unsigned int CastToInterfaceCPUAccess(ECPUAccessFlags eCPUAccess)
	{
		const static std::map<ECPUAccessFlags, unsigned int> mapFlags{
			{ECPUAccessFlags::NONE, 0},
			{ECPUAccessFlags::READ, D3D11_CPU_ACCESS_READ},
			{ECPUAccessFlags::WRITE, D3D11_CPU_ACCESS_WRITE},
			{ECPUAccessFlags::READ_WRITE, D3D11_CPU_ACCESS_READ|D3D11_CPU_ACCESS_WRITE}
		};

		return mapFlags.at(eCPUAccess);
	}

private:
	D3D11Buffer(ERHIBufferFlags eFlags, ECPUAccessFlags eCPUAccess);

	ComPtr<ID3D11Buffer> pInitResource;
};
