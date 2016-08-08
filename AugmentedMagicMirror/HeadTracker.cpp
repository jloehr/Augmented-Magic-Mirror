// HeadTracker.cpp : Wrapper around the Kinect to get the Face/Eye Position
//

#include "stdafx.h"
#include "HeadTracker.h"

#include "Camera.h"

HeadTracker::HeadTracker(_In_ Camera & Camera, _In_ ::Kinect & Kinect)
	:BoundCamera(Camera), Kinect(Kinect)
{
	Kinect.FaceModelUpdated += std::make_pair(this, &HeadTracker::FaceModelUpdatedCallback);
}

void HeadTracker::FaceModelUpdatedCallback(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale)
{	
	const CameraSpacePoint & NoseTop = FaceVertices[HighDetailFacePoints_NoseTop];

	BoundCamera.UpdateCamera(Vector3((NoseTop.X * RealWorldToVirutalScale) + Offset.X, (NoseTop.Y * RealWorldToVirutalScale) + Offset.Y, (NoseTop.Z * RealWorldToVirutalScale) + Offset.Z));
}
