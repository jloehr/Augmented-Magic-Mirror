#pragma once

#include "Kinect.h"

class Camera;

class HeadTracker
{
public:
	HeadTracker(Camera & Camera, Kinect & Kinect);

private:
	Camera & BoundCamera;
	Kinect & Kinect;

	void FaceModelUpdatedCallback(const Kinect::CameraSpacePointList & FaceVertices, const Vector3 & Offset, const float & RealWorldToVirutalScale);
};

