// Mesh.cpp : Represents a Mesh with its VertexBuffer, VertexIndices, RootSignature and Shader
//

#include "stdafx.h"
#ifdef USE_D3DX12
#include "Mesh12.h"

#include "GraphicsContext12.h"
#include "RenderingContext12.h"

namespace D3DX12
{
	Mesh::Mesh(_In_ GraphicsContext & DeviceContext)
		:DeviceContext(DeviceContext)
	{
	}

	void Mesh::Render(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const RenderingContext & RenderingContext, _In_ const TransformList & Objects) const
	{
		CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
		CommandList->IASetIndexBuffer(&IndexBufferView);

		for (const Transform & Object : Objects)
		{
			RenderingContext.SetObjectMatrix(CommandList, Object.GetMatrix());
			CommandList->DrawIndexedInstanced(IndexCount, 1, 0, 0, 0);
		}
	}

	void Mesh::UpdateVertices(const VertexList & Vertices)
	{
		if (UploadFence.IsBusy())
		{
			OutputDebugString(L"Upload is skipped!");
			return;
		}
		Utility::ThrowOnFail(CommandAllocator->Reset());
		Utility::ThrowOnFail(CommandList->Reset(CommandAllocator.Get(), nullptr));

		UploadData(CommandList, VertexBuffer, VertexUploadResource, Vertices.data(), VertexBufferView.SizeInBytes);

		Utility::ThrowOnFail(CommandList->Close());
		DeviceContext.ExecuteCommandList(CommandList);
		UploadFence.Set(DeviceContext.GetCommandQueue());
	}

	void Mesh::Create(const VertexList & Vertices, const IndexList & Indices)
	{
		UploadFence.Initialize(DeviceContext.GetDevice());

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator)));
		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&CommandList)));

		CreateVertexBuffer(CommandList, Vertices);
		CreateIndexBuffer(CommandList, Indices);

		Utility::ThrowOnFail(CommandList->Close());

		DeviceContext.ExecuteCommandList(CommandList);
		UploadFence.SetAndWait(DeviceContext.GetCommandQueue());
	}

	void Mesh::CreateVertexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const VertexList & Vertices)
	{
		size_t BufferSize = Vertices.size() * sizeof(VertexList::value_type);

		CreateBuffer(VertexBuffer, VertexUploadResource, BufferSize);

		UploadData(CommandList, VertexBuffer, VertexUploadResource, Vertices.data(), BufferSize);

		VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		VertexBufferView.StrideInBytes = sizeof(Vertex);
		VertexBufferView.SizeInBytes = static_cast<UINT>(BufferSize);
	}

	void Mesh::CreateIndexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const IndexList & Indices)
	{
		size_t BufferSize = Indices.size() * sizeof(IndexList::value_type);
		IndexCount = static_cast<UINT>(Indices.size());

		CreateBuffer(IndexBuffer, IndexUploadResource, BufferSize);

		UploadData(CommandList, IndexBuffer, IndexUploadResource, Indices.data(), BufferSize);

		IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		IndexBufferView.SizeInBytes = static_cast<UINT>(BufferSize);
		IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	}

	void Mesh::CreateBuffer(_In_ Microsoft::WRL::ComPtr<ID3D12Resource>& Resource, _In_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_ size_t ResourceSize)
	{
		CD3DX12_HEAP_PROPERTIES DefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_HEAP_PROPERTIES UploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ResourceSize);

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
			&DefaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			nullptr,
			IID_PPV_ARGS(&Resource)));

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
			&UploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&UploadResource)));
	}

	void Mesh::UploadData(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_reads_bytes_(DataSize) const void * Data, _In_ size_t DataSize)
	{
		{
			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, D3D12_RESOURCE_STATE_COPY_DEST);
			CommandList->ResourceBarrier(1, &ResourceBarrier);
		}

		D3D12_SUBRESOURCE_DATA SubresourceData = {};
		SubresourceData.pData = Data;
		SubresourceData.RowPitch = DataSize;
		SubresourceData.SlicePitch = SubresourceData.RowPitch;

		UpdateSubresources(CommandList.Get(), Resource.Get(), UploadResource.Get(), 0, 0, 1, &SubresourceData);

		{
			CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			CommandList->ResourceBarrier(1, &ResourceBarrier);
		}
	}
}
#endif
