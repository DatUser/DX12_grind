#pragma once

enum class ERHIBufferFlags
{
	VERTEX,
	INDEX,
	CONSTANT
};

class RHIBuffer
{
public:

	virtual void Init(void* pData, unsigned int uByteWidth, ERHIBufferFlags eFlags) = 0;

	virtual void Update() = 0;

protected:
	ERHIBufferFlags		m_eFlags;
private:
};
