// GraphicsContext.cpp : Interface for the graphics context.
//

#include "stdafx.h"
#include "GraphicsContext.h"

#include "Resource.h"

void GraphicsContext::LoadAndCompileShader(_Out_ Microsoft::WRL::ComPtr<ID3DBlob> & VertexShader, _Out_ Microsoft::WRL::ComPtr<ID3DBlob> & PixelShader, _In_ DWORD ShaderResourceId, _In_ const std::string & ShaderModel)
{
	Microsoft::WRL::ComPtr<ID3DBlob> Error;

	HRSRC ShaderResource = FindResource(nullptr, MAKEINTRESOURCE(ShaderResourceId), RT_RCDATA);
	HGLOBAL ShaderResourceHandle = LoadResource(nullptr, ShaderResource);
	DWORD ContentSize = SizeofResource(nullptr, ShaderResource);
	LPVOID Content = LockResource(ShaderResourceHandle);

#if defined(_DEBUG)
	UINT CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT CompileFlags = 0;
#endif

	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "VShader", (std::string("vs_") + ShaderModel).c_str(), CompileFlags, 0, &VertexShader, &Error), Error);
	Utility::ThrowOnFail(D3DCompile(Content, ContentSize, nullptr, nullptr, nullptr, "PShader", (std::string("ps_") + ShaderModel).c_str(), CompileFlags, 0, &PixelShader, &Error), Error);
}