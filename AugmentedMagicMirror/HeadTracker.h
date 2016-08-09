#pragma once

#include "Kinect.h"

class Camera;

class HeadTracker
{
public:
	HeadTracker(_In_ Camera & Camera, _In_ Kinect & Kinect);

private:
	Camera & BoundCamera;
	Kinect & Kinect;

	void FaceModelUpdatedCallback(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale);
};

