// AugmentedMagicMirror.cpp : Defines the application class.
//

#include "stdafx.h"
#include "AugmentedMagicMirror.h"

#include "SettingsFile.h"

AugmentedMagicMirror::AugmentedMagicMirror(_In_ HINSTANCE Instance)
	:Instance(Instance), Window(RenderContext), RenderContext(GraphicsDevice, Window, FCamera)
	,Kinect(SettingsFile::Kinect::GetKinectOffset())
	,HeadTracker(FCamera, Kinect), DepthMesh(GraphicsDevice)
	,RenderingContext(GraphicsDevice), CubeMesh(GraphicsDevice)
	,DCamera(Vector3(0.0f, 0.0f, 50.0f))
	,FCamera(Vector3(0.0f, 0.0f, 50.0f), SettingsFile::Monitor::GetMonitorHeight())
{
	Window.KeyPressed += std::make_pair(&Kinect, &Kinect::KeyPressedCallback);
	Window.KeyPressed += std::make_pair(&DepthMesh, &DepthMesh::KeyPressedCallback);

	float MonitorHeight = SettingsFile::Monitor::GetMonitorHeight();
	float MonitorHalfHeight = MonitorHeight / 2.f;
	constexpr float ElementsDistance = -180.f;
	constexpr float ElementsLength = 50.f;

	Cubes = {
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance - ElementsLength),		 Quaternion(),                   Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight,ElementsDistance - ElementsLength),		 Quaternion(90.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3(MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance - ElementsLength),		 Quaternion(90.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(MonitorHalfHeight, MonitorHalfHeight, ElementsDistance - ElementsLength),		 Quaternion(180.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance + ElementsLength), Quaternion(0.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight,ElementsDistance + ElementsLength), Quaternion(0.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3(MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance + ElementsLength), Quaternion(0.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(MonitorHalfHeight, MonitorHalfHeight, ElementsDistance + ElementsLength), Quaternion(180.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance), Quaternion(), Vector3(1.0f, 1.0f, (2 * ElementsLength) - 2.0f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight, ElementsDistance), Quaternion(), Vector3(1.0f, 1.0f, (2 * ElementsLength) - 2.0f)),
		Transform(Vector3(MonitorHalfHeight,-MonitorHalfHeight, ElementsDistance), Quaternion(), Vector3(1.0f, 1.0f, (2 * ElementsLength) - 2.0f)),
		Transform(Vector3(MonitorHalfHeight, MonitorHalfHeight, ElementsDistance), Quaternion(), Vector3(1.0f, 1.0f, (2 * ElementsLength) - 2.0f)),
		Transform(Vector3(30.f, 0.0f, ElementsDistance), Quaternion(0.f,   0.f,  0.f), Vector3(7.5f)),
		Transform(Vector3(-30.f, 0.0f, ElementsDistance), Quaternion(0.f, -45.f,  0.f), Vector3(5.0f)),
		Transform(Vector3(0.f, -10.0f, -100.0f), Quaternion(0.f, -45.f,  180.f), Vector3(5.0f))
	};
}

int AugmentedMagicMirror::Run(_In_ int CmdShow)
{
	Initialize(CmdShow);

	// Loop
	MSG Message = {};
	do {
		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		Kinect.Update();
		RenderContext.Render({ RenderContext::RenderParameter(RenderingContext, { RenderContext::ObjectList(CubeMesh, Cubes), DepthMesh.GetRenderObjectList() }) });

	} while (Message.message != WM_QUIT);

	Release();

	return static_cast<int>(Message.wParam);
}

void AugmentedMagicMirror::Initialize(_In_ int CmdShow)
{
	Window.Create(Instance);

	GraphicsDevice.Initialize();
	RenderContext.Initialize();
	RenderingContext.Create();
	CubeMesh.CreateCube();

	Kinect.Initialize();
	DepthMesh.Create(Kinect);

	Window.Show(CmdShow);
}

void AugmentedMagicMirror::Release()
{
	GraphicsDevice.Release(); 
	Kinect.Release();
}
