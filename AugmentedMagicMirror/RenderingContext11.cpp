// RenderingContext11.cpp : Context for rendering a mesh, i.e. Shader, PipelineState
//

#include "stdafx.h"
#ifdef USE_D3DX11
#include "RenderingContext11.h"

#include "GraphicsContext11.h"

#include "Camera.h"
#include "GraphicsContext.h"
#include "Resource.h"

namespace D3DX11
{
	RenderingContext::RenderingContext(_In_ GraphicsContext & DeviceContext)
		:DeviceContext(DeviceContext)
	{
	}

	void RenderingContext::Create()
	{
		Microsoft::WRL::ComPtr<ID3DBlob> VertexShaderBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> PixelShaderBlob;

		GraphicsContext::LoadAndCompileShader(VertexShaderBlob, PixelShaderBlob, IDR_SHADER11, "5_0");

		CreateShaders(VertexShaderBlob, PixelShaderBlob);
		CreateInputLayout(VertexShaderBlob);

		CreateConstantBuffer(CameraConstantBuffer, sizeof(CameraConstantBufferType));
		CreateConstantBuffer(ObjectConstantBuffer, sizeof(ObjectConstantBufferType));
	}

	void RenderingContext::Prepare(_In_ const Camera & Camera)
	{
		CameraConstantBufferType CameraData = { Camera.GetViewMatrix(), Camera.GetProjectionMatrix() };
		UpdateConstantBuffer(CameraConstantBuffer, &CameraData, sizeof(CameraData));

		DeviceContext.GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
		DeviceContext.GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

		std::array<ID3D11Buffer*, 2> ConstantBuffers = { CameraConstantBuffer.Get(), ObjectConstantBuffer.Get() };
		DeviceContext.GetDeviceContext()->VSSetConstantBuffers(0, 2, ConstantBuffers.data());

		DeviceContext.GetDeviceContext()->IASetInputLayout(InputLayout.Get());
	}

	void RenderingContext::SetObjectMatrix(_In_ const DirectX::XMFLOAT4X4 & ObjectMatrix)
	{
		UpdateConstantBuffer(ObjectConstantBuffer, &ObjectMatrix, sizeof(ObjectMatrix));
	}

	void RenderingContext::UpdateConstantBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & ConstantBuffer, _In_ const void * Data, _In_ size_t DataSize)
	{
		D3D11_MAPPED_SUBRESOURCE MappedSubresource = {};

		Utility::ThrowOnFail(DeviceContext.GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubresource));
		std::memcpy(MappedSubresource.pData, Data, DataSize);
		DeviceContext.GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);
	}

	void RenderingContext::CreateShaders(_In_ const Microsoft::WRL::ComPtr<ID3DBlob> & VertexShaderBlob, _In_ const Microsoft::WRL::ComPtr<ID3DBlob> & PixelShaderBlob)
	{
		LPVOID Buffer = VertexShaderBlob->GetBufferPointer();
		SIZE_T Size = VertexShaderBlob->GetBufferSize();

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateVertexShader(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), nullptr, &VertexShader));
		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreatePixelShader(PixelShaderBlob->GetBufferPointer(), PixelShaderBlob->GetBufferSize(), nullptr, &PixelShader));
	}

	void RenderingContext::CreateInputLayout(_In_ const Microsoft::WRL::ComPtr<ID3DBlob> & VertexShaderBlob)
	{
		std::array<D3D11_INPUT_ELEMENT_DESC, 2> InputElementDesc
		{ 
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			} 
		};

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateInputLayout(InputElementDesc.data(), static_cast<UINT>(InputElementDesc.size()), VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), &InputLayout));
	}

	void RenderingContext::CreateConstantBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & ConstantBuffer, _In_ UINT BufferSize)
	{
		D3D11_BUFFER_DESC BufferDesc = {};
		BufferDesc.ByteWidth = BufferSize;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = 0;

		Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateBuffer(&BufferDesc, nullptr, &ConstantBuffer));
	}
}
#endif
