// RenderingContext.cpp : Context for rendering a mesh, i.e. Shader, PipelineState, RootSignature
//

#include "stdafx.h"
#include "RenderingContext.h"

#include "Resource.h"
#include "GraphicsContext.h"
#include "Camera.h"

RenderingContext::RenderingContext(_In_ GraphicsContext & DeviceContext)
	:DeviceContext(DeviceContext)
{
}

void RenderingContext::Create()
{
	CreateRootSignature();
	CreatePipelineState();
}

void RenderingContext::Prepare(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const Camera & Camera) const
{
	CommandList->SetPipelineState(PipelineState.Get());
	CommandList->SetGraphicsRootSignature(RootSignature.Get());

	// Set Object Constant
	const DirectX::XMFLOAT4X4 & ViewMatrix = Camera.GetViewMatrix();
	const DirectX::XMFLOAT4X4 & ProjectionMatrix = Camera.GetProjectionMatrix();

	CommandList->SetGraphicsRoot32BitConstants(0, Num32BitPerMatrix, &ViewMatrix, 0);
	CommandList->SetGraphicsRoot32BitConstants(0, Num32BitPerMatrix, &ProjectionMatrix, Num32BitPerMatrix);
}

void RenderingContext::SetObjectMatrix(_In_ const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> & CommandList, _In_ const DirectX::XMFLOAT4X4 & ObjectMatrix) const
{
	CommandList->SetGraphicsRoot32BitConstants(1, Num32BitPerMatrix, &ObjectMatrix, 0);
}

void RenderingContext::LoadAndCompileShader(_Out_ Microsoft::WRL::ComPtr<ID3DBlob> & VertexShader, _Out_ Microsoft::WRL::ComPtr<ID3DBlob> & PixelShader)
{
	Microsoft::WRL::ComPtr<ID3DBlob> Error;

	HRSRC ShaderResource = FindResource(nullptr, MAKEINTRESOURCE(IDR_SHADER), RT_RCDATA);
	HGLOBAL ShaderResourceHandle = LoadResource(nullptr, ShaderResource);
	DWORD ContentSize = SizeofResource(nullptr, ShaderResource);
	LPVOID Content = LockResource(ShaderResourceHandle);

#if defined(_DEBUG)
	UINT CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT CompileFlags = 0;
#endif

	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "VShader", "vs_5_1", CompileFlags, 0, &VertexShader, &Error), Error);
	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "PShader", "ps_5_1", CompileFlags, 0, &PixelShader, &Error), Error);
}

void RenderingContext::CreateRootSignature()
{
	std::array<CD3DX12_ROOT_PARAMETER, 2> RootParameters;
	RootParameters[0].InitAsConstants(2 * Num32BitPerMatrix, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	RootParameters[1].InitAsConstants(1 * Num32BitPerMatrix, 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	D3D12_ROOT_SIGNATURE_FLAGS RootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(static_cast<UINT>(RootParameters.size()), RootParameters.data(), 0, nullptr, RootSignatureFlags);

	Microsoft::WRL::ComPtr<ID3DBlob> SerializedSignature;
	Microsoft::WRL::ComPtr<ID3DBlob> Error;

	Utility::ThrowOnFail(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SerializedSignature, &Error), Error);
	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateRootSignature(0, SerializedSignature->GetBufferPointer(), SerializedSignature->GetBufferSize(), IID_PPV_ARGS(&RootSignature)));
}

void RenderingContext::CreatePipelineState()
{
	Microsoft::WRL::ComPtr<ID3DBlob> VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> PixelShader;

	LoadAndCompileShader(VertexShader, PixelShader);

	std::array<D3D12_INPUT_ELEMENT_DESC, 2> InputElementDesc
	{ {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		} };

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	PipelineStateDesc.InputLayout = { InputElementDesc.data(), static_cast<UINT>(InputElementDesc.size()) };
	PipelineStateDesc.pRootSignature = RootSignature.Get();
	PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader.Get());
	PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader.Get());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.SampleDesc.Count = 1;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	Utility::ThrowOnFail(DeviceContext.GetDevice()->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&PipelineState)));
}
