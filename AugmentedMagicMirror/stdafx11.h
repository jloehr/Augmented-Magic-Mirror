#pragma once

#define USE_NVAPI

#include <dxgi1_2.h>
#include <d3d11.h> 

#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D3d11.lib")

#ifdef USE_NVAPI
#include <nvapi.h>

#ifdef _WIN64
#pragma comment (lib, "../NVAPI/amd64/nvapi64.lib")
#else
#pragma comment (lib, "../NVAPI/x86/nvapi.lib")
#endif
#else
typedef void * StereoHandle;
#endif // USE_NVAPI
