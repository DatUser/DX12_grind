#pragma once

#include "Core/Core.h"

// Be careful with packing rules: https://learn.microsoft.com/fr-fr/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules?redirectedfrom=MSDN
struct Light
{
	Vec3	m_oPos;
	float	m_fLuminance; // 16 bytes alignment
	Vec4	m_oColor;
	//float	m_fHereForAlignment;
};
