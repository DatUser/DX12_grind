#pragma once

#include "Core/Core.h"
#include "fwdtypes.h"
#include "RHI/rhi_buffer.h"

#include <map>
#include <memory>

class D3D11Buffer: public RHIBuffer
{
	friend class D3D11Interface;
public:
	D3D11Buffer(
		void* 			pData,
		uint32_t 		uByteWidth,
		ERHIBufferFlags eFlags,
		ECPUAccessFlags eCPUAccess);
	~D3D11Buffer();

	virtual void Update() override final;
	virtual bool IsValid() override final
	{
		// TODO: Check if better checks can be done
		return pInitResource.Get() != nullptr;
	}

	//inline void** GetResource()
	//{
	//	return reinterpret_cast<void**>(pInitResource.GetAddressOf());
	//}

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
	ComPtr<ID3D11Buffer> pInitResource;
};
