#pragma once

#include "Core/Core.h"
#include "Core/fwdtypes.h"
#include "RHI/rhi_buffer.h"

#include <array>


class D3D11Buffer: public RHIBuffer
{
	friend class D3D11Interface;
public:

	D3D11Buffer() = delete;

	D3D11Buffer(
		void* 			pData,
		uint32_t 		uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess,
		ERHIBufferUsage eUsage);

	virtual ~D3D11Buffer();

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
		constexpr static std::array<unsigned int, static_cast<unsigned int>(ERHIBufferFlags::_size)> mapFlags{{
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_BIND_INDEX_BUFFER,
			D3D11_BIND_VERTEX_BUFFER
		}
		};

		return mapFlags[static_cast<unsigned int>(eFlags)];
	}

private:
	ComPtr<ID3D11Buffer> pInitResource;
};