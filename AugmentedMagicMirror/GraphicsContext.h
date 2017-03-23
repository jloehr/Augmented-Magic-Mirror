#pragma once

#include "RenderContext.h"
#include "Mesh.h"

class Window;
class Camera;

class GraphicsContext;
typedef std::unique_ptr<GraphicsContext> PGraphicsContext;

PGraphicsContext CreateGraphicsContext();


class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default; 

	virtual void Initialize() = 0;
	virtual void Release() = 0;

	virtual PRenderContext CreateRenderContext(_In_ Window & TargetWindow, _In_ Camera & Camera) = 0;
	virtual PMesh CreateMesh() = 0;
};

