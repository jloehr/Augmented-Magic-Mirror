#pragma once

#include "Mesh.h"
#include "Transform.h"

namespace D3DX11
{
	class GraphicsContext;
	class RenderingContext;

	class Mesh : public ::Mesh
	{
	public:
		Mesh(_In_ GraphicsContext & DeviceContext);
		virtual ~Mesh() = default;

		virtual void UpdateVertices(_In_ const VertexList & Vertices); 
		
		void Render(_In_ RenderingContext & RenderingContext, _In_ const TransformList & Objects) const;

	private:
		GraphicsContext & DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
		UINT Stride;
		UINT IndexCount;

		virtual void Create(_In_ const VertexList & Vertices, _In_ const IndexList & Indices);
		void CreateBuffer(_In_ D3D11_BIND_FLAG BindFlag, _In_ const void * InitialData, _In_ size_t Size, _Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & Buffer);
	};
}

