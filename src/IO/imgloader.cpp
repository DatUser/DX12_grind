#include "IO/imgloader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "external/stb_image.h"

#include "RHI/rhi_texture.h"

bool load_texture(const std::string& sTextureFilename, RHITexture* pTexture)
{
	int iWidth = 0, iHeight = 0, iChannels = 0;
	stbi_uc* pPixelData = stbi_load(sTextureFilename.c_str(), &iWidth, &iHeight, &iChannels, 0);

	// Failed to load
	if (!pPixelData)
		return false;

	// Sizes
	ETextureFormat eDesiredFormat = pTexture->GetFormat();
	int iDesiredChannels = GetNumChannels(eDesiredFormat);
	bool bIsFloatFormat = IsFloatFormat(eDesiredFormat);
	const int iNumPx = iWidth * iHeight;
	const int iChannelsToCopy = min(iChannels, iDesiredChannels);

	// Update texture attributes
	uint32_t uByteSize = GetByteSize(eDesiredFormat, iWidth, iHeight);
	pTexture->SetWidth(iWidth);
	pTexture->SetHeight(iHeight);
	auto& arrTextureData = pTexture->GetData();
	if (arrTextureData.capacity() < uByteSize)
		arrTextureData.resize(uByteSize);

	void* pTextureData = arrTextureData.data();
	for (int i = 0; i < iNumPx; i += iChannels)
	{
		for (int j = 0; j < iChannelsToCopy; j++)
		{
			if (bIsFloatFormat)
				((float*)pTextureData)[i + j] = ((float*)pPixelData)[i + j] / 255.0f;
			else
				((uint8_t*)pTextureData)[i + j] = pPixelData[i + j];
		}
	}

	std::cout << "Texture " << sTextureFilename << " loaded." << std::endl;
	stbi_image_free(pPixelData);
	return true;
}