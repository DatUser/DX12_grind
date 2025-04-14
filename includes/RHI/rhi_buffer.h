#pragma once

#include <stdint.h>

enum class ERHIBufferFlags : uint8_t
{
	CONSTANT,
	INDEX,
	VERTEX,

	_size
};

enum class ERHICPUAccessFlags : uint8_t
{
	NONE,
	READ,
	WRITE,
	READ_WRITE,

	_size
};

enum class ERHIBufferUsage : uint8_t
{
	DEFAULT,
	DYNAMIC,

	_size
};

class RHIBuffer
{
	friend class D3D11Interface;
public:
	RHIBuffer(
		void* pData,
		uint32_t uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess,
		ERHIBufferUsage eUsage);

	virtual void Update() = 0;
	virtual bool IsValid() = 0;


protected:
	void*				m_pData; 		// Buffer cpu data
	uint32_t 			m_uByteWidth;	// Size of the buffer (bytes)
	uint32_t 			m_uOffset;		// Size of a single element
	uint32_t 			m_uStride;		// Size between each element
	ERHIBufferFlags		m_eFlags;
	ERHICPUAccessFlags	m_eCPUAccess;
	ERHIBufferUsage		m_eUsage;
private:
	static constexpr int m_map = 0;
	//static constexpr std::map<ERHIBufferFlags, unsigned int> m_map;
};
