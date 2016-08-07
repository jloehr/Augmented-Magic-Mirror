// Kinect.cpp : Wrapper around the Kinect
//

#include "stdafx.h"
#include "Kinect.h"


Kinect::Kinect(const Vector3 & Offset)
	:Offset(Offset)
	,RealWorldToVirutalScale(100.f) // Kinect Sensor reports its values in "Meters"; Virtual World uses "Centimeters"
{
}

void Kinect::Initialize()
{
	Utility::ThrowOnFail(GetDefaultKinectSensor(&KinectSensor));
	KinectSensor->Open();

	SetupBodyFrameReader();
	SetupHighDefinitionFaceFrameReader();
	SetupFaceModel();
}

void Kinect::Release()
{
	if (KinectSensor)
	{
		KinectSensor->Close();
		KinectSensor.Reset();
	}
}

void Kinect::Update()
{
	for (Event & Event : Events)
	{
		CheckEvent(Event);
	}
}

const Vector3 & Kinect::GetOffset() const
{
	return Offset;
}

float Kinect::GetRealWorldToVirutalScale() const
{
	return RealWorldToVirutalScale;
}

void Kinect::SetupBodyFrameReader()
{
	Utility::ThrowOnFail(KinectSensor->get_BodyFrameSource(&BodyFrameSource));

	INT32 BodyCount = 0;
	Utility::ThrowOnFail(BodyFrameSource->get_BodyCount(&BodyCount));
	Bodies.resize(BodyCount);

	Utility::ThrowOnFail(BodyFrameSource->OpenReader(&BodyFrameReader));

	AddEvent<IBodyFrameReader>(BodyFrameReader, &IBodyFrameReader::SubscribeFrameArrived, &Kinect::BodyFrameRecieved);
}

void Kinect::SetupHighDefinitionFaceFrameReader()
{
	Utility::ThrowOnFail(CreateHighDefinitionFaceFrameSource(KinectSensor.Get(), &HighDefinitionFaceFrameSource));
	Utility::ThrowOnFail(HighDefinitionFaceFrameSource->OpenReader(&HighDefinitionFaceFrameReader));

	AddEvent<IHighDefinitionFaceFrameReader>(HighDefinitionFaceFrameReader, &IHighDefinitionFaceFrameReader::SubscribeFrameArrived, &Kinect::HighDefinitionFaceFrameRecieved);
}

void Kinect::SetupFaceModel()
{
	float Deformation[FaceShapeDeformations_Count] = {};
	Utility::ThrowOnFail(CreateFaceModel(1.0f, FaceShapeDeformations_Count, Deformation, &FaceModel));
	Utility::ThrowOnFail(CreateFaceAlignment(&FaceAlignment));

	UINT32 VertexCount;
	Utility::ThrowOnFail(GetFaceModelVertexCount(&VertexCount));
	FaceVertices.resize(VertexCount);
}

void Kinect::CheckEvent(Event & Event)
{
	HANDLE EventHandle = reinterpret_cast<HANDLE>(Event.first);

	switch (WaitForSingleObject(EventHandle, 0))
	{
	case WAIT_TIMEOUT:
		// Non signaled, so there is no new data
		return;
	case WAIT_FAILED:
	{
		std::wstringstream Error(L"Error Code: ");
		Error << GetLastError();
		Utility::Throw(Error.str().c_str());
		return;
	}
	case WAIT_OBJECT_0:
		(this->*Event.second)(Event.first);
		return;
	}
}

void Kinect::BodyFrameRecieved(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IBodyFrame> BodyFrame = GetBodyFrame(EventHandle);

	if (BodyFrame == nullptr)
	{
		return;
	}

	UpdateBodies(BodyFrame);
	UpdateTrackedBody();
}

Microsoft::WRL::ComPtr<IBodyFrame> Kinect::GetBodyFrame(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IBodyFrameArrivedEventArgs> BodyFrameArrivedEventArgs;
	Microsoft::WRL::ComPtr<IBodyFrameReference> BodyFrameReference;
	Microsoft::WRL::ComPtr<IBodyFrame> BodyFrame;

	Utility::ThrowOnFail(BodyFrameReader->GetFrameArrivedEventData(EventHandle, &BodyFrameArrivedEventArgs));
	Utility::ThrowOnFail(BodyFrameArrivedEventArgs->get_FrameReference(&BodyFrameReference));
	BodyFrameReference->AcquireFrame(&BodyFrame);

	return BodyFrame;
}

void Kinect::UpdateBodies(Microsoft::WRL::ComPtr<IBodyFrame>& BodyFrame)
{
	std::vector<IBody *> NewBodyData(Bodies.size(), nullptr);
	std::transform(Bodies.begin(), Bodies.end(), NewBodyData.begin(), [](auto ComPtr)->auto { return ComPtr.Get(); });
	Utility::ThrowOnFail(BodyFrame->GetAndRefreshBodyData(static_cast<UINT>(NewBodyData.size()), NewBodyData.data()));
	std::transform(NewBodyData.begin(), NewBodyData.end(), Bodies.begin(), Bodies.begin(), [](auto RawPtr, auto ComPtr)->auto { return ComPtr = RawPtr; });
}

void Kinect::UpdateTrackedBody()
{
	if (!UpdateCurrentTrackedBody())
	{
		TrackNewBody();
	}
}

bool Kinect::UpdateCurrentTrackedBody()
{
	if (TrackedBody == nullptr)
	{
		return false;
	}

	UINT64 CurrentTrackingID;
	Utility::ThrowOnFail(TrackedBody->get_TrackingId(&CurrentTrackingID));

	auto IsTrackedBody = [=](auto Body)
	{
		BOOLEAN IsTracked;
		Utility::ThrowOnFail(Body->get_IsTracked(&IsTracked));
		if (!IsTracked)
		{
			return false;
		}

		UINT64 BodyTrackingID;
		Utility::ThrowOnFail(Body->get_TrackingId(&BodyTrackingID));

		return (CurrentTrackingID == BodyTrackingID);
	};

	auto Result = std::find_if(Bodies.begin(), Bodies.end(), IsTrackedBody);
	TrackedBody = (Result != Bodies.end()) ? (*Result) : nullptr;

	return (TrackedBody != nullptr);
}

void Kinect::TrackNewBody()
{
	UINT64 NewTrackingID = 0;

	for (auto & Body : Bodies)
	{
		BOOLEAN IsTracked;
		Utility::ThrowOnFail(Body->get_IsTracked(&IsTracked));
		if (IsTracked)
		{
			TrackedBody = Body;
			Utility::ThrowOnFail(Body->get_TrackingId(&NewTrackingID));
			break;
		}
	}

	Utility::ThrowOnFail(HighDefinitionFaceFrameSource->put_TrackingId(NewTrackingID));
}

void Kinect::HighDefinitionFaceFrameRecieved(WAITABLE_HANDLE EventHandle)
{
	if (UpdateFaceModel(GetFaceFrame(EventHandle)))
	{
		FaceModelUpdated(FaceVertices, Offset, RealWorldToVirutalScale);
	}
}

bool Kinect::UpdateFaceModel(Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> FaceFrame)
{
	if (FaceFrame == nullptr)
	{
		return false;
	}

	BOOLEAN IsFaceTracket;
	Utility::ThrowOnFail(FaceFrame->get_IsFaceTracked(&IsFaceTracket));

	if (!IsFaceTracket)
	{
		return false;
	}

	Utility::ThrowOnFail(FaceFrame->GetAndRefreshFaceAlignmentResult(FaceAlignment.Get()));
	Utility::ThrowOnFail(FaceModel->CalculateVerticesForAlignment(FaceAlignment.Get(), static_cast<UINT>(FaceVertices.size()), FaceVertices.data()));

	return true;
}

Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> Kinect::GetFaceFrame(WAITABLE_HANDLE EventHandle)
{
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameArrivedEventArgs> HighDefinitionFaceFrameArrivedEventArgs;
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrameReference> HighDefinitionFaceFrameReference;
	Microsoft::WRL::ComPtr<IHighDefinitionFaceFrame> HighDefinitionFaceFrame;

	Utility::ThrowOnFail(HighDefinitionFaceFrameReader->GetFrameArrivedEventData(EventHandle, &HighDefinitionFaceFrameArrivedEventArgs));
	Utility::ThrowOnFail(HighDefinitionFaceFrameArrivedEventArgs->get_FrameReference(&HighDefinitionFaceFrameReference));
	HighDefinitionFaceFrameReference->AcquireFrame(&HighDefinitionFaceFrame);

	return HighDefinitionFaceFrame;
}

