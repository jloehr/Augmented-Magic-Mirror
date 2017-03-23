#pragma once

#include "Callback.h"

class RenderContext;

class Window
{
public:
	typedef std::pair<ULONG, ULONG> WindowSize;

	Window();

	void Create(_In_ HINSTANCE Instance);
	void Show(_In_ int CmdShow);

	const HWND & GetHandle() const;
	const WindowSize & GetWindowSize() const;

	Callback<WPARAM> KeyPressed;
	Callback<WindowSize> WindowResized;

protected:
	HWND WindowHandle;
	WindowSize Size;

	WindowSize QueryWindowSize();
	void UpdateWindowSize();

	virtual const std::wstring & RegisterWindowClass(_In_ HINSTANCE Instance) = 0;
};

