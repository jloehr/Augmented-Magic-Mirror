#pragma once

#include "Transform.h"
#include "GPUFence.h"

class GraphicsContext;
class Camera;
class RenderingContext;

class Mesh
{
public:
	Mesh(_In_ GraphicsContext & DeviceContext);

	void CreateCube();
	void CreatePlane(_In_ unsigned Width, _In_ unsigned Height);
	void Render(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const RenderingContext & RenderingContext, _In_ const TransformList & Objects) const;

	void UpdateVertices(const Vector3List & Vertices);

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
	typedef std::vector<Vertex> VertexList;
	typedef uint32_t Index;
	typedef std::vector<Index> IndexList;

	GraphicsContext & DeviceContext;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexUploadResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexUploadResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBuffer;
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	GPUFence UploadFence;

	UINT IndexCount;

	void Create(_In_ const VertexList & Vertices, _In_ const IndexList & Indices);
	void CreateVertexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const VertexList & Vertices);
	void CreateIndexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const IndexList & Indices);
	void CreateBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_ size_t  ResourceSize);

	void UploadData(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_reads_bytes_(DataSize) const void * Data, _In_ size_t DataSize);
};
