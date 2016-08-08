#pragma once

class GraphicsContext;
class Camera;

class RenderingContext
{
public:
	RenderingContext(GraphicsContext & DeviceContext);

	void Create();
	void Prepare(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, Camera & Camera);
	void SetObjectMatrix(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, const DirectX::XMFLOAT4X4 & ObjectMatrix);

private:
	static constexpr UINT Num32BitPerMatrix = 4 * 4;

	static void LoadAndCompileShader(_Out_ Microsoft::WRL::ComPtr<ID3DBlob> & VertexShader, _Out_ Microsoft::WRL::ComPtr<ID3DBlob> & PixelShader);

	GraphicsContext & DeviceContext;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PipelineState;

	void CreateRootSignature();
	void CreatePipelineState();
};

