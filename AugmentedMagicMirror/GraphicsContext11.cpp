// GraphicsDevice11.cpp : The graphics device context
//

#include "stdafx.h"
#ifdef USE_D3DX11
#include "GraphicsContext11.h"

#include "RenderContext11.h"
#include "Mesh11.h"

PGraphicsContext CreateGraphicsContext()
{
	return std::make_unique<D3DX11::GraphicsContext>();
}

namespace D3DX11
{
	GraphicsContext::GraphicsContext()
		: DefaultShader(*this)
		, StereoHandle(nullptr)
	{
	}

	void GraphicsContext::Initialize()
	{
		InitializeNvAPI();
		CreateFactory();
		CreateDevice();
		CreateStereoHandle();

		DefaultShader.Create();
	}

	void GraphicsContext::Release()
	{
		UnloadNvAPI();
	}

	Microsoft::WRL::ComPtr<IDXGIFactory2>& GraphicsContext::GetFactory()
	{
		return Factory;
	}

	Microsoft::WRL::ComPtr<ID3D11Device>& GraphicsContext::GetDevice()
	{
		return Device;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> & GraphicsContext::GetDeviceContext()
	{
		return DeviceContext;
	}

	RenderingContext & GraphicsContext::GetDefaultShader()
	{
		return DefaultShader;
	}

	StereoHandle & GraphicsContext::GetStereoHandle()
	{
		return StereoHandle;
	}

	PRenderContext GraphicsContext::CreateRenderContext(_In_ Window & TargetWindow, _In_ Camera & NoseCamera, _In_ Camera & LeftEyeCamera, _In_ Camera & RighEyeCamera)
	{
		return std::make_unique<RenderContext>(*this, TargetWindow, NoseCamera, LeftEyeCamera, RighEyeCamera);
	}

	PMesh GraphicsContext::CreateMesh()
	{
		return std::make_unique<Mesh>(*this);
	}

	void GraphicsContext::InitializeNvAPI()
	{
#ifdef USE_NVAPI
		NvAPI_Status Status = NVAPI_OK;
		NvU8 IsEnabled = 0;

		Status = NvAPI_Initialize();
		Status = NvAPI_Stereo_SetDriverMode(NVAPI_STEREO_DRIVER_MODE_DIRECT);

#endif
	}

	void GraphicsContext::CreateStereoHandle()
	{
#ifdef USE_NVAPI
		NvAPI_Status Status = NVAPI_OK;
		NvU8 IsEnabled = 0;

		Status = NvAPI_Stereo_IsEnabled(&IsEnabled);
		if (IsEnabled)
		{
			Status = NvAPI_Stereo_CreateHandleFromIUnknown(Device.Get(), &StereoHandle);
		}
#endif
	}

	void GraphicsContext::UnloadNvAPI()
	{
#ifdef USE_NVAPI
		NvAPI_Status Status = NVAPI_OK;

		if (StereoHandle != nullptr)
		{
			Status = NvAPI_Stereo_DestroyHandle(StereoHandle);
		}

		Status = NvAPI_Unload();
#endif
	}

	void GraphicsContext::CreateFactory()
	{
		Utility::ThrowOnFail(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)));
	}

	void GraphicsContext::CreateDevice()
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1> HardwareAdapter;
		UINT CreationFlags = 0;
#ifdef _DEBUG
		CreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		for (UINT AdapterIndex = 0; Factory->EnumAdapters1(AdapterIndex, &HardwareAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
		{
			DXGI_ADAPTER_DESC1 Desc;
			HardwareAdapter->GetDesc1(&Desc);

			if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Skip the software adapter
				continue;
			}
			D3D_FEATURE_LEVEL FeatureLevelRequest = D3D_FEATURE_LEVEL_11_1;
			D3D_FEATURE_LEVEL FeatureLevel;

			HRESULT Result = D3D11CreateDevice(HardwareAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, CreationFlags, &FeatureLevelRequest, 1, D3D11_SDK_VERSION, &Device, &FeatureLevel, &DeviceContext);
			if (SUCCEEDED(Result))
			{
				break;
			}
		}

		if (Device == nullptr)
		{
			Utility::Throw(L"No Device was created!");
		}
	}
}
#endif
