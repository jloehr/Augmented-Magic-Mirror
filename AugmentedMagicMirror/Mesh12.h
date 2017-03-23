#pragma once

#include "GPUFence12.h"

#include "Mesh.h"
#include "Transform.h"


namespace D3DX12
{
	class GraphicsContext;
	class RenderingContext;

	class Mesh : public ::Mesh
	{
	public:
		Mesh(_In_ GraphicsContext & DeviceContext);

		void Render(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const RenderingContext & RenderingContext, _In_ const TransformList & Objects) const;

		virtual void UpdateVertices(const VertexList & Vertices);

	private:
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

		virtual void Create(_In_ const VertexList & Vertices, _In_ const IndexList & Indices);
		void CreateVertexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const VertexList & Vertices);
		void CreateIndexBuffer(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const IndexList & Indices);
		void CreateBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_ size_t  ResourceSize);

		void UploadData(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & Resource, _Out_ const Microsoft::WRL::ComPtr<ID3D12Resource> & UploadResource, _In_reads_bytes_(DataSize) const void * Data, _In_ size_t DataSize);
	};
}
