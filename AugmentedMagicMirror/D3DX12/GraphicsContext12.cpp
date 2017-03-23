// GraphicsDevice.cpp : The graphics device context
//

#include "stdafx.h"
#include "GraphicsContext12.h"
/*
void GraphicsContext::Initialize()
{
	EnableDebugLayer();
	CreateFactory();
	CreateDevice();
	CreateCommandQueue();

	Fence.Initialize(Device);
}

void GraphicsContext::Release()
{
	// Wait until all work on the GPU is done
	Fence.SetAndWait(CommandQueue);
}

Microsoft::WRL::ComPtr<IDXGIFactory4>& GraphicsContext::GetFactory()
{
	return Factory;
}

Microsoft::WRL::ComPtr<ID3D12Device>& GraphicsContext::GetDevice()
{
	return Device;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GraphicsContext::GetCommandQueue()
{
	return CommandQueue;
}

void GraphicsContext::ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CommandList) const
{
	GraphicsContext::ExecuteCommandList(CommandList, CommandQueue);
}

void GraphicsContext::ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& CommandList, const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& CommandQueue)
{
	ID3D12CommandList * CommandListPointer = CommandList.Get();
	CommandQueue->ExecuteCommandLists(1, &CommandListPointer);
}

void GraphicsContext::EnableDebugLayer()
{
#if defined(_DEBUG)
	Microsoft::WRL::ComPtr<ID3D12Debug> DebugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
	{
		DebugController->EnableDebugLayer();
	}
#endif
}

void GraphicsContext::CreateFactory()
{
	Utility::ThrowOnFail(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)));
}

void GraphicsContext::CreateDevice()
{
	Microsoft::WRL::ComPtr<IDXGIAdapter1> HardwareAdapter;

	for (UINT AdapterIndex = 0; Factory->EnumAdapters1(AdapterIndex, &HardwareAdapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
	{
		DXGI_ADAPTER_DESC1 Desc;
		HardwareAdapter->GetDesc1(&Desc);

		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Skip the software adapter
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(HardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device))))
		{
			break;
		}
	}

	if (Device == nullptr)
	{
		Utility::Throw(L"No Device was created!");
	}
}

void GraphicsContext::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Utility::ThrowOnFail(Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&CommandQueue)));
}
*/