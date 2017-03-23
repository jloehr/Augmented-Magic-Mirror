#pragma once

#include "RenderTarget12.h"

#include "RenderContext.h"
#include "Window.h"

class Camera;

namespace D3DX12
{
	class GraphicsContext;
	class RenderingContext;
	class Mesh;

	class RenderContext : public ::RenderContext
	{
	public:
		RenderContext(_In_ GraphicsContext & DeviceContext, _In_ Window & TargetWindow, _In_ ::Camera & Camera);

		virtual void Initialize();

		virtual void Render(_In_ MeshList DrawCalls);

		void OnWindowSizeChange(_In_ const Window::WindowSize & NewSize);

	protected:
		typedef std::array<float, 4> DXColor;
		static constexpr DXColor BackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr UINT BufferFrameCount = 2;
		template<typename T> using BufferFrameArray = std::array<T, BufferFrameCount>;

		GraphicsContext & DeviceContext;

		Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencelView;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

		D3D12_VIEWPORT Viewport;
		D3D12_RECT ScissorRect;

		BufferFrameArray<RenderTarget> RenderTargets;
		GPUFence Fence;

		UINT RTVDescSize;
		UINT BufferFrameIndex;

		void CreateSwapChain();
		void CreateRTVHeap();
		void CreateDSVHeap();
		void CreateDepthStencilView();
		void InitializeRenderTargets();

		void CreateCommandList();

		void ResizeBuffers(_In_ const Window::WindowSize & NewSize);
		void ReleaseSizeDependentBuffers();
		void ResizeSwapChain(_In_ const Window::WindowSize & NewSize);
		void RecreateSizeDependentBuffers();
		void UpdateViewportAndScissorRect(_In_ const Window::WindowSize & Size);

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle();

		typedef std::pair<RenderingContext &, MeshList> RenderParameter;
		typedef std::vector<RenderParameter> RenderParameterList;
		void Render(_In_ RenderParameterList DrawCalls);
	};
}