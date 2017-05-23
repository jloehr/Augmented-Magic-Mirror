#pragma once

#include "GPUFence12.h"
#include "RenderingContext12.h"

#include "GraphicsContext.h"

namespace D3DX12 
{
	class GraphicsContext : public ::GraphicsContext
	{
	public:
		GraphicsContext();
		virtual ~GraphicsContext() = default;

		virtual void Initialize();
		virtual void Update();
		virtual void Release();

		Microsoft::WRL::ComPtr<IDXGIFactory4> & GetFactory();
		Microsoft::WRL::ComPtr<ID3D12Device> & GetDevice();
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> & GetCommandQueue();
		RenderingContext & GetDefaultShader();

		virtual PRenderContext CreateRenderContext(_In_ Window & TargetWindow, _In_ Camera & NoseCamera, _In_ Camera & LeftEyeCamera, _In_ Camera & RighEyeCamera);
		virtual PMesh CreateMesh();

		void ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList) const;
		static void ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, const Microsoft::WRL::ComPtr<ID3D12CommandQueue> & CommandQueue);

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
		Microsoft::WRL::ComPtr<ID3D12Device> Device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;

		GPUFence Fence;
		RenderingContext DefaultShader;

		void EnableDebugLayer();
		void CreateFactory();
		void CreateDevice();
		void CreateCommandQueue();
	};
}
