#pragma once

#include "Kinect.h"
#include "Mesh.h"
#include "RenderContext.h"
#include "Transform.h"

class GraphicsContext;

class DepthMesh
{
public:
	DepthMesh(_In_ GraphicsContext & DeviceContext);

	void Create(_In_ Kinect & Kinect);

	RenderContext::ObjectList GetRenderObjectList() const;

	void KeyPressedCallback(_In_ const WPARAM & VirtualKey);

private:
	PMesh PlaneMesh;
	TransformList Instances;

	Mesh::VertexList VertexCache;

	bool ColorizeDepth;

	void OffsetUpdatedCallback(_In_ const Vector3 & Offset);
	void DepthVerticesUpdatedCallback(_In_ const Kinect::CameraSpacePointList & DepthVertices);
};

