#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <atldef.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <Windows.h>
#include <comdef.h>
#include <wrl.h>

#include <vector>
#include <sstream>


using Vec3 = DirectX::XMFLOAT3;
using Rot = DirectX::XMFLOAT3; // Rotation Pitch, Yaw, Roll
using Quat = DirectX::XMFLOAT4;
using Vec4 = DirectX::XMFLOAT4;
using Mat4x4 = DirectX::XMMATRIX;