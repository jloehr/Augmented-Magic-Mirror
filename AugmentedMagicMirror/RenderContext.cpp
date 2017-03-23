// RenderContext.cpp : Interface for rendering frames into a single window.
//

#include "stdafx.h"
#include "RenderContext.h"

#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"
#include "Window.h"

RenderContext::RenderContext(_In_ Window & TargetWindow, _In_ ::Camera & Camera)
	:TargetWindow(TargetWindow), Camera(Camera)
{
}
