// Mesh.cpp : Represents a Mesh with its VertexBuffer, VertexIndices, RootSignature and Shader
//

#include "stdafx.h"
#include "Mesh.h"

#include "GraphicsContext.h"
#include "RenderingContext.h"

Mesh::Mesh(_In_ GraphicsContext & DeviceContext)
	:DeviceContext(DeviceContext)
{
}

void Mesh::Create()
{
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexUploadResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexUploadResource;
	GPUFence Fence;

	Fence.Initialize(DeviceContext.GetDevice());
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator)));
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&CommandList)));
	
	UploadVertices(CommandList, VertexUploadResource);
	UploadIndices(CommandList, IndexUploadResource);

	Utility::ThrowOnFail(CommandList->Close());

	ID3D12CommandList * CommandListPointer = CommandList.Get();
	DeviceContext.GetCommandQueue()->ExecuteCommandLists(1, &CommandListPointer);

	Fence.SetAndWait(DeviceContext.GetCommandQueue());
}

void Mesh::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, RenderingContext & RenderingContext, const TransformList & Objects)
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

void Mesh::UploadVertices(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource)
{
	std::array<Vertex, 8> Cube =
	{ {
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	} };

	constexpr size_t SizeOfCube = Cube.size() * sizeof(Vertex);

	UploadData(CommandList, VertexBuffer, UploadResource, Cube.data(), SizeOfCube);

	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.StrideInBytes = sizeof(Vertex);
	VertexBufferView.SizeInBytes = SizeOfCube;
}

void Mesh::UploadIndices(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource)
{
	using IndexSize = uint16_t;
	std::array<IndexSize, 36> Indices =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	constexpr size_t SizeOfIndices = Indices.size() * sizeof(IndexSize);
	IndexCount = static_cast<UINT>(Indices.size());

	UploadData(CommandList, IndexBuffer, UploadResource, Indices.data(), SizeOfIndices);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.SizeInBytes = SizeOfIndices;
	IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Mesh::UploadData(_In_ Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_reads_bytes_(DataSize) const void * Data, _In_ size_t  DataSize)
{
	CD3DX12_HEAP_PROPERTIES DefaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES UploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(DataSize);

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
		&DefaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&Resource)));

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateCommittedResource(
		&UploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadResource)));

	D3D12_SUBRESOURCE_DATA SubresourceData = {};
	SubresourceData.pData = Data;
	SubresourceData.RowPitch = DataSize;
	SubresourceData.SlicePitch = SubresourceData.RowPitch;

	UpdateSubresources(CommandList.Get(), Resource.Get(), UploadResource.Get(), 0, 0, 1, &SubresourceData);

	CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	CommandList->ResourceBarrier(1, &ResourceBarrier);
}