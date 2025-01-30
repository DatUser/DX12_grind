#include "D3D11Commands.h"

void D3D11Commands::SetVertexBuffers(RHIBuffer* pBuffer)
{
	D3D11Interface* pInterface = RHI::GetInterface().get();

	pInterface->SetInputLayout
}