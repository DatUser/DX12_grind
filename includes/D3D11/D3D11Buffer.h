#pragma once

#include "Core/Core.h"
#include "Core/fwdtypes.h"
#include "RHI/rhi_buffer.h"

#include <array>
#include <memory>

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
		constexpr static std::array<unsigned int, static_cast<unsigned int>(ERHIBufferFlags::_size)> mapFlags{{
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_BIND_INDEX_BUFFER,
			D3D11_BIND_VERTEX_BUFFER
		}
		};

		return mapFlags[static_cast<unsigned int>(eFlags)];
	}

	static inline unsigned int CastToInterfaceCPUAccess(ERHICPUAccessFlags eCPUAccess)
	{
		constexpr static std::array<unsigned int, static_cast<unsigned int>(ERHICPUAccessFlags::_size)> mapFlags{
			0,
			D3D11_CPU_ACCESS_READ,
			D3D11_CPU_ACCESS_WRITE,
			D3D11_CPU_ACCESS_READ|D3D11_CPU_ACCESS_WRITE
		};

		return mapFlags[static_cast<unsigned int>(eCPUAccess)];
	}

	static inline D3D11_USAGE CastToInterfaceUsage(ERHIBufferUsage eUsage)
	{
		constexpr static std::array<D3D11_USAGE, static_cast<unsigned int>(ERHIBufferUsage::_size)> mapFlags{
			D3D11_USAGE_DEFAULT,
			D3D11_USAGE_DYNAMIC
		};

		return mapFlags[static_cast<unsigned int>(eUsage)];
	}

private:
	ComPtr<ID3D11Buffer> pInitResource;
};