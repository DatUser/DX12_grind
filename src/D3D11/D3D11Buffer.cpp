#include "D3D11/D3D11Buffer.h"

#include "app.h"
#include "camera.h"
#include "D3D11/D3D11Interface.h"

D3D11Buffer::D3D11Buffer()
{
}

D3D11Buffer::~D3D11Buffer()
{
}

void D3D11Buffer::Init(void *pData, unsigned int uByteWidth, ERHIBufferFlags eFlags)
{
	//D3D11Interface* pD3D11Interface = dynamic_cast<D3D11Interface&>(App::GetInstance()->GetRHI());
	RHI* pInterface = App::GetInstance()->GetRHI();
	pInterface->CreateBuffer(pData, uByteWidth, this);

}

void D3D11Buffer::Update()
{
}
