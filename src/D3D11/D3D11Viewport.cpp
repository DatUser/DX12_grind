#include "D3D11/D3D11Viewport.h"

D3D11Viewport::D3D11Viewport()
: RHIViewport()
{
	ZeroMemory(&m_oViewport, sizeof(D3D11_VIEWPORT));
}

D3D11Viewport::D3D11Viewport(uint32_t uWidth, uint32_t uHeight, uint32_t uTopLeftX, uint32_t uTopLeftY)
: RHIViewport()
{
	ZeroMemory(&m_oViewport, sizeof(D3D11_VIEWPORT));
	m_oViewport.TopLeftX = uTopLeftX;
	m_oViewport.TopLeftY = uTopLeftY;
	m_oViewport.Width = uWidth;
	m_oViewport.Height = uHeight;
}

D3D11Viewport::~D3D11Viewport()
{
}