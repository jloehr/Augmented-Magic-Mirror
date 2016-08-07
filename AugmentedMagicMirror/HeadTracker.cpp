// KinectHeadTracking.cpp : Wrapper around the Kinect to get the Face/Eye Position
//

#include "stdafx.h"
#include "HeadTracker.h"

#include "Camera.h"

HeadTracker::HeadTracker(Camera & Camera, ::Kinect & Kinect)
	:BoundCamera(Camera), Kinect(Kinect)
{
	Kinect.FaceModelUpdated += std::make_pair(this, &HeadTracker::FaceModelUpdatedCallback);
}

void HeadTracker::FaceModelUpdatedCallback(const Kinect::CameraSpacePointList & FaceVertices)
{	
	const CameraSpacePoint & NoseTop = FaceVertices[HighDetailFacePoints_NoseTop];
	const Vector3 & Offset = Kinect.GetOffset();
	float RealWorldToVirutalScale = Kinect.GetRealWorldToVirutalScale();

	BoundCamera.UpdateCamera(Vector3((NoseTop.X * RealWorldToVirutalScale) + Offset.X, (NoseTop.Y * RealWorldToVirutalScale) + Offset.Y, (NoseTop.Z * RealWorldToVirutalScale) + Offset.Z));
}
