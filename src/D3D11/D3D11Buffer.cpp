#include "D3D11/D3D11Buffer.h"

#include "app.h"
#include "camera.h"
#include "D3D11/D3D11Interface.h"

D3D11Buffer::D3D11Buffer(ERHIBufferFlags eFlags, ECPUAccessFlags eCPUAccess)
: RHIBuffer(eFlags, eCPUAccess)
{
	//D3D11Interface* pD3D11Interface = dynamic_cast<D3D11Interface&>(App::GetInstance()->GetRHI());
	//RHI* pInterface = App::GetInstance()->GetRHI();
	//pInterface->CreateBuffer(pData, uByteWidth, this);
}

D3D11Buffer::~D3D11Buffer()
{
}

void D3D11Buffer::Update()
{
}
