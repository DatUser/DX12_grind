#include "D3D11/D3D11Buffer.h"

#include "app.h"
#include "D3D11/D3D11Interface.h"


D3D11Buffer::D3D11Buffer(
	void* 			pData,
	uint32_t 		uByteWidth,
	ERHIBufferFlags eFlags,
	ERHICPUAccessFlags eCPUAccess,
	ERHIBufferUsage eUsage
)
: RHIBuffer(
	pData,
	uByteWidth,
	eFlags,
	eCPUAccess,
	eUsage)
{
	//D3D11Interface* pD3D11Interface = dynamic_cast<D3D11Interface&>(RHI::GetInterface());
	//RHI* pInterface = App::GetInstance()->GetRHI();
	//pInterface->CreateBuffer(pData, uByteWidth, this);
	//D3D11Interface::GetInterface()->createBufferInternal(this);

	ATLASSERT(D3D11Interface::GetInterface()->createBufferInternal(this) == S_OK);
}

D3D11Buffer::~D3D11Buffer()
{
}

void D3D11Buffer::Update()
{
}
