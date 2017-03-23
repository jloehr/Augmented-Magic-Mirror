#pragma once

#include "MainWindow.h"
#include "GraphicsContext.h"
#include "RenderContext.h"
#include "Kinect.h"
#include "HeadTracker.h"
#include "DepthMesh.h"

#include "DirectionalFoVCamera.h"
#include "FrameCamera.h"

#include "Mesh.h"
#include "Transform.h"

class AugmentedMagicMirror
{
public:
	AugmentedMagicMirror(_In_ HINSTANCE Instance);

	int Run(_In_ int nCmdShow);

private:
	HINSTANCE Instance;

	MainWindow Window;
	PGraphicsContext GraphicsDevice;
	PRenderContext RenderContext;
	Kinect Kinect;
	HeadTracker HeadTracker;
	DepthMesh DepthMesh;

	DirectionalFoVCamera DCamera;
	FrameCamera FCamera;

	PMesh CubeMesh;
	TransformList Cubes;

	void Initialize(_In_ int CmdShow);
	void Release();
};
