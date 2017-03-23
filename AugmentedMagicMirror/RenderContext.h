#pragma once

#include "Transform.h"

class Camera;
class Mesh;
class Window;

class RenderContext;
typedef std::unique_ptr<RenderContext> PRenderContext;

class RenderContext
{
public:
	RenderContext(_In_ Window & TargetWindow, _In_ Camera & Camera);
	virtual ~RenderContext() = default;

	virtual void Initialize() = 0;

	typedef std::pair<const Mesh &, const TransformList &> ObjectList;
	typedef std::vector<ObjectList> MeshList;
	virtual void Render(_In_ MeshList DrawCalls) = 0;

protected:
	Window & TargetWindow;
	Camera & Camera;
};
