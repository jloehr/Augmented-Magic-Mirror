#pragma once

#include "GraphicsContext.h"

#include "RenderingContext11.h"

namespace D3DX11
{
	class GraphicsContext : public ::GraphicsContext
	{
	public:
		GraphicsContext();
		virtual ~GraphicsContext() = default;

		virtual void Initialize();
		virtual void Release();

		Microsoft::WRL::ComPtr<IDXGIFactory2> & GetFactory();
		Microsoft::WRL::ComPtr<ID3D11Device> & GetDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> & GetDeviceContext();
		RenderingContext & GetDefaultShader();
		bool IsStereoEnabled() const;

		virtual PRenderContext CreateRenderContext(_In_ Window & TargetWindow, _In_ Camera & NoseCamera, _In_ Camera & LeftEyeCamera, _In_ Camera & RighEyeCamera);
		virtual PMesh CreateMesh();

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory2> Factory;
		Microsoft::WRL::ComPtr<ID3D11Device> Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext > DeviceContext;

		RenderingContext DefaultShader;
		bool StereoEnabled;

		void CreateFactory();
		void CreateDevice();
	};
}

