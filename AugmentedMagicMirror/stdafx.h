// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#define USE_D3DX11

#ifndef USE_D3DX11
#define USE_D3DX12
#endif // !USE_D3DX11

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Windowsx.h>
#include <wrl.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#ifdef USE_D3DX11
#endif // USE_D3DX11
#ifdef USE_D3DX12
#include "stdafx12.h"
#endif // USE_D3DX12


#include <Kinect.h>
#include <Kinect.Face.h>

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "Utility.h"
