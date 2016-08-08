#pragma once

class RenderContext;

class Window
{
public:
	typedef std::pair<ULONG, ULONG> WindowSize;

	Window(_In_ RenderContext & WindowRenderer);

	void Create(_In_ HINSTANCE Instance);
	void Show(_In_ int CmdShow);

	const HWND & GetHandle() const;
	const WindowSize & GetWindowSize() const;

protected:
	HWND WindowHandle;
	WindowSize Size;
	RenderContext & WindowRenderer;

	WindowSize QueryWindowSize();
	void UpdateWindowSize();

	virtual const std::wstring & RegisterWindowClass(_In_ HINSTANCE Instance) = 0;
};

