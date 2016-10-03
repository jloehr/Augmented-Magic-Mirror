// DepthMesh.cpp : Renders the depth image of the Kinect as mesh
//

#include "stdafx.h"
#include "DepthMesh.h"

#include "GraphicsContext.h"
#include "Kinect.h"

DepthMesh::DepthMesh(_In_ GraphicsContext & DeviceContext)
	:PlaneMesh(DeviceContext), ColorizeDepth(false)
{
}

void DepthMesh::Create(_In_ Kinect & Kinect)
{
	PlaneMesh.CreatePlane(Kinect::DepthImageWidth, Kinect::DepthImageHeigth);

	Instances.push_back(Transform(Kinect.GetOffset(), Quaternion(), Vector3(Kinect.GetRealWorldToVirutalScale(), Kinect.GetRealWorldToVirutalScale(), -Kinect.GetRealWorldToVirutalScale())));

	Kinect.OffsetUpdated += std::make_pair(this, &DepthMesh::OffsetUpdatedCallback);
	Kinect.DepthVerticesUpdated += std::make_pair(this, &DepthMesh::DepthVerticesUpdatedCallback);
}

RenderContext::ObjectList DepthMesh::GetRenderObjectList() const
{
	return RenderContext::ObjectList( PlaneMesh, Instances );
}

void DepthMesh::KeyPressedCallback(const WPARAM & VirtualKey)
{
	if (VirtualKey == 'F')
	{
		ColorizeDepth = !ColorizeDepth;
	}
}

void DepthMesh::OffsetUpdatedCallback(const Vector3 & Offset)
{
	for (Transform & Object : Instances)
	{
		Object.UpdatePosition(Offset);
	}
}

void DepthMesh::DepthVerticesUpdatedCallback(_In_ const Kinect::CameraSpacePointList & DepthVertices)
{
	VertexCache.resize(DepthVertices.size());

	std::transform(DepthVertices.begin(), DepthVertices.end(), VertexCache.begin(), [ColorizeDepth = this->ColorizeDepth](auto Vertex)
	{ 
		constexpr float MinDist = 0.7f;
		constexpr float MaxDist = 3.0f;
		float Value = (!ColorizeDepth) ?  0.0f : std::fmaxf(0.f, std::fminf(1.f, 1.0f - (Vertex.Z - MinDist) / (MaxDist - MinDist)));
		return Mesh::Vertex({ { Vertex.X, Vertex.Y, Vertex.Z },{ 0.0f, 0.0f, Value, 0.0f } });
	});

	PlaneMesh.UpdateVertices(VertexCache);
}

