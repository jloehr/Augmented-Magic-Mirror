#pragma once

#include "Kinect.h"

class Camera;

class HeadTracker
{
public:
	HeadTracker(_In_ Camera & NoseCamera, _In_ Camera & LeftEyeCamera, _In_ Camera & RighEyeCamera, _In_ Kinect & Kinect);

	void KeyPressedCallback(_In_ const WPARAM & VirtualKey);
private:
	Camera & NoseCamera;
	Camera & LeftEyeCamera;
	Camera & RighEyeCamera;
	Kinect & Kinect;

	bool UpdateCameras;

	void FaceModelUpdatedCallback(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale);
	void UpdateCamera(_In_ const Kinect::CameraSpacePointList & FaceVertices, _In_ const Vector3 & Offset, _In_ const float & RealWorldToVirutalScale, _In_ Camera & Camera, _In_ HighDetailFacePoints VertexPoint);
};

