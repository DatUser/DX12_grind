#include "D3D11/D3D11Viewport.h"

//D3D11Viewport::D3D11Viewport(uint32_t uWidth, uint32_t uHeight, uint32_t uTopLeftX, uint32_t uTopLeftY)
D3D11Viewport::D3D11Viewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
: RHIViewport(hWnd, uWidth, uHeight)
{
	ZeroMemory(&m_oViewport, sizeof(D3D11_VIEWPORT));
	m_oViewport.TopLeftX = 0;
	m_oViewport.TopLeftY = 0;
	m_oViewport.Width = uWidth;
	m_oViewport.Height = uHeight;
}

D3D11Viewport::~D3D11Viewport()
{
}