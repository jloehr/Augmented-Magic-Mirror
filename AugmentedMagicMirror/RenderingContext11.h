#pragma once

class Camera;

namespace D3DX11
{
	class GraphicsContext;

	class RenderingContext
	{
	public:
		RenderingContext(_In_ GraphicsContext & DeviceContext);
		~RenderingContext() = default;

		void Create();

		void Prepare(_In_ const Camera & Camera);
		void SetObjectMatrix(_In_ const DirectX::XMFLOAT4X4 & ObjectMatrix);

	private:
		struct CameraConstantBufferType {
			DirectX::XMFLOAT4X4 View;
			DirectX::XMFLOAT4X4 Projection;
		};

		struct ObjectConstantBufferType {
			DirectX::XMFLOAT4X4 Object;
		};

		GraphicsContext & DeviceContext;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

		Microsoft::WRL::ComPtr<ID3D11Buffer> CameraConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> ObjectConstantBuffer;
		
		void CreateShaders(_In_ const Microsoft::WRL::ComPtr<ID3DBlob> & VertexShaderBlob, _In_ const Microsoft::WRL::ComPtr<ID3DBlob> & PixelShaderBlob);
		void CreateInputLayout(_In_ const Microsoft::WRL::ComPtr<ID3DBlob> & VertexShaderBlob);
		void CreateConstantBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & ConstantBuffer, _In_ UINT BufferSize);

		void UpdateConstantBuffer(_Out_ Microsoft::WRL::ComPtr<ID3D11Buffer> & ConstantBuffer, _In_ const void * Data, _In_ size_t DataSize);
	};
}
