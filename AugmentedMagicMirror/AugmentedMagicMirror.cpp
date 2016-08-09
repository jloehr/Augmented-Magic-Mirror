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
	float MonitorHeight = SettingsFile::Monitor::GetMonitorHeight();
	float MonitorHalfHeight = MonitorHeight / 2.f;

	Cubes = { 
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight, -1.5f),		 Quaternion(),                   Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight, -1.5f),		 Quaternion( 90.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3( MonitorHalfHeight,-MonitorHalfHeight, -1.5f),		 Quaternion( 90.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3( MonitorHalfHeight, MonitorHalfHeight, -1.5f),		 Quaternion(180.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight,-100.f), Quaternion(  0.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight,-100.f), Quaternion(  0.f,   0.f,  0.f), Vector3(2.f)),
		Transform(Vector3( MonitorHalfHeight,-MonitorHalfHeight,-100.f), Quaternion(  0.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3( MonitorHalfHeight, MonitorHalfHeight,-100.f), Quaternion(180.f,  90.f,  0.f), Vector3(2.f)),
		Transform(Vector3(-MonitorHalfHeight,-MonitorHalfHeight, -50.f), Quaternion(), Vector3(1.0f, 1.0f, 100.f - 2.0f)),
		Transform(Vector3(-MonitorHalfHeight, MonitorHalfHeight, -50.f), Quaternion(), Vector3(1.0f, 1.0f, 100.f - 2.0f)),
		Transform(Vector3( MonitorHalfHeight,-MonitorHalfHeight, -50.f), Quaternion(), Vector3(1.0f, 1.0f, 100.f - 2.0f)),
		Transform(Vector3( MonitorHalfHeight, MonitorHalfHeight, -50.f), Quaternion(), Vector3(1.0f, 1.0f, 100.f - 2.0f)),
		Transform(Vector3( 30.f, 0.0f, -70.0f), Quaternion(  0.f,   0.f,  0.f), Vector3(7.5f)),
		Transform(Vector3(-30.f, 0.0f, -70.0f), Quaternion(  0.f, -45.f,  0.f), Vector3(5.0f)),
		Transform(Vector3(0.f, 0.0f, -80.0f), Quaternion(0.f, -45.f,  180.f), Vector3(10.0f))
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
