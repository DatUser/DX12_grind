constexpr uint32_t operator&(uint32_t uFlags, ERHITextureFlags eFlags)
{
	return uFlags & static_cast<uint32_t>(eFlags);
}

constexpr uint32_t operator|=(uint32_t& uFlags, ERHITextureFlags eFlag)
{
	return uFlags |= static_cast<uint32_t>(eFlag);
}

constexpr uint32_t operator|(ERHITextureFlags eFlagA, ERHITextureFlags eFlagB)
{
	return static_cast<uint32_t>(eFlagA) | static_cast<uint32_t>(eFlagB);
}

constexpr int GetNumChannels(ETextureFormat eFormat)
{
	switch (eFormat)
	{
	case ETextureFormat::R8G8B8A8_UNORM:
		return 4;
	case ETextureFormat::R8_UNORM:
		return 1;
	case ETextureFormat::D24_UNORM_S8_UINT:
		return 1;
	case ETextureFormat::R32G32B32_FLOAT:
		return 3;
	case ETextureFormat::R32G32B32A32_FLOAT:
		return 4;
	default:
		return 0;
	}
}

constexpr bool IsFloatFormat(ETextureFormat eFormat)
{
	return static_cast<uint32_t>(eFormat) >= static_cast<uint32_t>(ETextureFormat::R32G32B32_FLOAT);
}

constexpr int GetByteSize(ETextureFormat eFormat, int iWidth, int iHeight)
{
	int iEltSize = IsFloatFormat(eFormat) ? 4 : 1;
	return GetNumChannels(eFormat) * iEltSize * iWidth * iHeight;
}