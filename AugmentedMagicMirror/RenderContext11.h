#pragma once

#include "RenderContext.h"
#include "Window.h"

namespace D3DX11
{
	class GraphicsContext;

	class RenderContext : public ::RenderContext
	{
	public:
		RenderContext(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow, _In_::Camera & Camera);
		virtual ~RenderContext() = default;

		virtual void Initialize();

		virtual void Render(_In_ MeshList DrawCalls);

		void OnWindowSizeChange(_In_ const Window::WindowSize & NewSize);

	private:
		typedef std::array<float, 4> DXColor;
		static constexpr DXColor BackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr UINT BufferFrameCount = 2;

		GraphicsContext & DeviceContext;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV;
		
		D3D11_VIEWPORT Viewport;
		D3D11_RECT ScissorRect;

		void CreateSwapChain(_In_ const Window::WindowSize & Size);
		void CreateRenderTargets();
		void CreateDepthStencil(_In_ const Window::WindowSize & Size);

		void UpdateViewportAndScissorRect(_In_ const Window::WindowSize & Size);
	};
}

