#pragma once

#include <string>

class RHITexture;

bool load_texture(
	const std::string& sTextureFilename,
	RHITexture* pTexture
);