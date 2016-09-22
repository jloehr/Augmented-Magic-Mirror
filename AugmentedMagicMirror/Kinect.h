#pragma once

#include "Callback.h"

class Kinect
{
public:
	typedef std::vector<CameraSpacePoint> CameraSpacePointList;

	static const unsigned DepthImageWidth = 512;
	static const unsigned DepthImageHeigth = 424;

	Kinect(_In_ const Vector3 & Offset);

	void Initialize();
	void Release();
	void Update();

	const Vector3 & GetOffset() const;
	float GetRealWorldToVirutalScale() const;

	Callback<Vector3> OffsetUpdated;
	Callback<CameraSpacePointList, Vector3, float> FaceModelUpdated;
	Callback<CameraSpacePointList> DepthVerticesUpdated;

	void KeyPressedCallback(_In_ const WPARAM & VirtualKey);
	
private:
	typedef void(Kinect::*EventCallback)(WAITABLE_HANDLE EventHandle);
	typedef std::pair<WAITABLE_HANDLE, EventCallback> Event;
	typedef std::vector<Event> EventList;
	typedef std::vector<Microsoft::WRL::ComPtr<IBody>> BodyVector;

	Vector3 Offset;
	const float RealWorldToVirutalScale;

	Microsoft::WRL::ComPtr<IKinectSensor> KinectSensor;
	EventList Events;

	Microsoft::WRL::ComPtr<IBodyFrameSource> BodyFrameSource;
	Microsoft::WRL::ComPtr<IBodyFrameReader> BodyFrameReader;
	Microsoft::WRL::ComPtr<IBody> TrackedBody;
	BodyVector Bodies;

	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameSource> HighDefinitionFaceFrameSource;
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameReader> HighDefinitionFaceFrameReader;
	Microsoft::WRL::ComPtr<IFaceModel> FaceModel;
	Microsoft::WRL::ComPtr<IFaceAlignment> FaceAlignment;
	CameraSpacePointList FaceVertices;

	Microsoft::WRL::ComPtr<ICoordinateMapper> CoordinateMapper;
	Microsoft::WRL::ComPtr<IDepthFrameSource> DepthFrameSource;
	Microsoft::WRL::ComPtr<IDepthFrameReader> DepthFrameReader;
	CameraSpacePointList DepthVertices;

	void SetupBodyFrameReader();
	void SetupHighDefinitionFaceFrameReader();
	void SetupFaceModel();
	void SetupDepthFrameReader();

	template<typename InterfaceType>
	void AddEvent(_In_ Microsoft::WRL::ComPtr<InterfaceType> Interface, _In_ HRESULT(_stdcall InterfaceType::*EventRegister)(WAITABLE_HANDLE *), _In_ EventCallback Callback)
	{
		WAITABLE_HANDLE EventHandle;
		Utility::ThrowOnFail((Interface.Get()->*EventRegister)(&EventHandle));

		Events.push_back(std::make_pair(EventHandle, Callback));
	}

	void CheckEvent(_In_ Event & Event);

	void BodyFrameRecieved(_In_ WAITABLE_HANDLE EventHandle);
	Microsoft::WRL::ComPtr<IBodyFrame> GetBodyFrame(_In_ WAITABLE_HANDLE EventHandle);
	void UpdateBodies(_In_ Microsoft::WRL::ComPtr<IBodyFrame> & BodyFrame);
	void UpdateTrackedBody();
	bool UpdateCurrentTrackedBody();
	void TrackNewBody();

	void HighDefinitionFaceFrameRecieved(_In_ WAITABLE_HANDLE EventHandle);
	bool UpdateFaceModel(_In_ Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> FaceFrame);
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> GetFaceFrame(_In_ WAITABLE_HANDLE EventHandle);

	void DepthFrameRecieved(_In_ WAITABLE_HANDLE EventHandle);
	Microsoft::WRL::ComPtr<IDepthFrame> GetDepthFrame(_In_ WAITABLE_HANDLE EventHandle);
};

