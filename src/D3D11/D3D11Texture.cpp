#include "D3D11/D3D11Texture.h"

D3D11Texture::D3D11Texture(void* pData, int iWidth, int iHeight, ETextureFormat eFormat)
: RHITexture(pData, iWidth, iHeight, eFormat)
{
}