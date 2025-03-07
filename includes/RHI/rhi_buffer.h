#pragma once

#include <stdint.h>

enum class ERHIBufferFlags
{
	VERTEX,
	INDEX,
	CONSTANT
};

enum class ECPUAccessFlags
{
	NONE,
	READ,
	WRITE,
	READ_WRITE
};

class RHIBuffer
{
public:
	RHIBuffer(
		void* pData,
		uint32_t uByteWidth,
		ERHIBufferFlags eFlags,
		ECPUAccessFlags eCPUAccess);

	virtual void Update() = 0;
	virtual bool IsValid() = 0;


protected:
	void*				m_pData;
	uint32_t 			m_uByteWidth;
	ERHIBufferFlags		m_eFlags;
	ECPUAccessFlags		m_eCPUAccess;
private:
	static constexpr int m_map = 0;
	//static constexpr std::map<ERHIBufferFlags, unsigned int> m_map;
};
