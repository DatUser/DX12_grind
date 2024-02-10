#pragma once

//#include <DirectXMath.h>
namespace Microsoft::WRL
{
	template<typename T>
	class ComPtr;
}

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;