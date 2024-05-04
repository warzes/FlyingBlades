#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 3)
#	pragma warning(disable : 4265)
#	pragma warning(disable : 4365)
#	pragma warning(disable : 4625)
#	pragma warning(disable : 4626)
#	pragma warning(disable : 4668)
#	pragma warning(disable : 4820)
#	pragma warning(disable : 5039)
#	pragma warning(disable : 5204)
#	pragma warning(disable : 5220)
#endif

#include "WindowsInclude.h"
#include <ShellScalingAPI.h>
#include <wrl.h>
#include <comdef.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dx12.h" // TODO: обновить с гитхаба https://github.com/microsoft/DirectX-Headers

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif