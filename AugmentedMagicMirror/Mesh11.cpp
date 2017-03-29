// Mesh.cpp : Represents a Mesh 
//

#include "stdafx.h"
#ifdef USE_D3DX11
#include "Mesh11.h"

#include "GraphicsContext11.h"
#include "RenderingContext11.h"

namespace D3DX11
{
	Mesh::Mesh(_In_ GraphicsContext & DeviceContext)
		:DeviceContext(DeviceContext)
	{
	}

	void Mesh::UpdateVertices(_In_ const VertexList & Vertices)
	{
		DeviceContext.GetDeviceContext()->UpdateSubresource(VertexBuffer.Get(), 0, nullptr, Vertices.data(), 0, 0);
	}

	void Mesh::Render(_In_ RenderingContext & RenderingContext, _In_ const TransformList & Objects) const
	{
		std::array<ID3D11Buffer *const, 1> VertexBuffers = { VertexBuffer.Get() };
		std::array<UINT, 1> Offsets = { 0 };
		DeviceContext.GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext.GetDeviceContext()->IASetVertexBuffers(0, 1, VertexBuffers.data(), &Stride, Offsets.data());
		DeviceContext.GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		for (const Transform & Object : Objects)
		{
			RenderingContext.SetObjectMatrix(Object.GetMatrix());
			DeviceContext.GetDeviceContext()->DrawIndexedInstanced(IndexCount, 1, 0, 0, 0);
		}
	}

	void  Mesh::Create(_In_ const VertexList & Vertices, _In_ const IndexList & Indices)
	{
		CreateBuffer(D3D11_BIND_VERTEX_BUFFER, Vertices.data(), Vertices.size() * sizeof(VertexList::value_type), VertexBuffer);
		CreateBuffer(D3D11_BIND_INDEX_BUFFER, Indices.data(), Indices.size() * sizeof(IndexList::value_type), IndexBuffer);

		Stride = sizeof(VertexList::value_type);
		IndexCount = static_cast<UINT>(Indices.size());
	}

	void Mesh::CreateBuffer(_In_ D3D11_BIND_FLAG BindFlag, _In_ const void * InitialData, _In_ size_t Size, _Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & Buffer)
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = static_cast<UINT>(Size);
		BufferDesc.BindFlags = BindFlag;

		D3D11_SUBRESOURCE_DATA InitialResData = {};
		InitialResData.pSysMem = InitialData;

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateBuffer(&BufferDesc, &InitialResData, &Buffer));
	}
}
#endif
