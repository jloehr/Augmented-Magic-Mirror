#pragma once

#include "RenderContext.h"
#include "Kinect.h"
#include "Mesh.h"

class GraphicsContext;
class Kienct;

class DepthMesh
{
public:
	DepthMesh(_In_ GraphicsContext & DeviceContext);

	void Create(_In_ Kinect & Kinect);

	RenderContext::ObjectList GetRenderObjectList() const;

	void KeyPressedCallback(_In_ const WPARAM & VirtualKey);

private:
	Mesh PlaneMesh;
	TransformList Instances;

	Mesh::VertexList VertexCache;

	bool ColorizeDepth;

	void OffsetUpdatedCallback(_In_ const Vector3 & Offset);
	void DepthVerticesUpdatedCallback(_In_ const Kinect::CameraSpacePointList & DepthVertices);
};

