#pragma once

#include "RenderContext.h"
#include "Kinect.h"
#include "Mesh.h"

class GraphicsContext;

class DepthMesh
{
public:
	DepthMesh(_In_ GraphicsContext & DeviceContext);

	void Create(_In_ Kinect & Kinect);

	RenderContext::ObjectList GetRenderObjectList() const;

private:
	Mesh PlaneMesh;
	TransformList Instances;

	Mesh::VertexList VertexCache;

	void DepthVerticesUpdatedCallback(_In_ const Kinect::CameraSpacePointList & DepthVertices);

};

