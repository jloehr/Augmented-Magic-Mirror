#pragma once

#include "MainWindow.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Kinect.h"
#include "HeadTracker.h"

#include "DirectionalFoVCamera.h"
#include "FrameCamera.h"

#include "Model.h"
#include "Transform.h"

class AugmentedMagicMirror
{
public:
	AugmentedMagicMirror(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	MainWindow Window;
	GraphicsContext GraphicsDevice;
	Renderer Renderer;
	Kinect Kinect;
	HeadTracker HeadTracker;

	DirectionalFoVCamera DCamera;
	FrameCamera FCamera;

	Model CubeModel;
	TransformList Cubes;

	void Initialize(_In_ int CmdShow);
	void Release();
};
