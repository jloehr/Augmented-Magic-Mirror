// HeadTracker.cpp : Wrapper around the Kinect to get the Face/Eye Position
//

#include "stdafx.h"
#include "HeadTracker.h"

#include "Camera.h"

HeadTracker::HeadTracker(_In_ Camera & NoseCamera, _In_ Camera & LeftEyeCamera, _In_ Camera & RighEyeCamera, _In_ ::Kinect & Kinect)
	:NoseCamera(NoseCamera), LeftEyeCamera(LeftEyeCamera), RighEyeCamera(RighEyeCamera), Kinect(Kinect)
	, UpdateCameras(true)
{
	Kinect.FaceModelUpdated += std::make_pair(this, &HeadTracker::FaceModelUpdatedCallback);
}

void HeadTracker::KeyPressedCallback(const WPARAM & VirtualKey)
{
	if (VirtualKey == VK_SPACE)
	{
		UpdateCameras = !UpdateCameras;
	}
}

void HeadTracker::FaceModelUpdatedCallback(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale)
{
	if (!UpdateCameras)
		return;

	UpdateCamera(FaceVertices, Offset, RealWorldToVirutalScale, NoseCamera, HighDetailFacePoints_NoseTop);
	UpdateCamera(FaceVertices, Offset, RealWorldToVirutalScale, LeftEyeCamera, HighDetailFacePoints_LefteyeMidtop);
	UpdateCamera(FaceVertices, Offset, RealWorldToVirutalScale, RighEyeCamera, HighDetailFacePoints_RighteyeMidtop);
}
void HeadTracker::UpdateCamera(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale, _In_ Camera & Camera, _In_ HighDetailFacePoints VertexPoint)
{
	const CameraSpacePoint & Vertex = FaceVertices[VertexPoint];

	Camera.UpdateCamera(Vector3((Vertex.X * RealWorldToVirutalScale) + Offset.X, (Vertex.Y * RealWorldToVirutalScale) + Offset.Y, (Vertex.Z * RealWorldToVirutalScale) + Offset.Z));
}
