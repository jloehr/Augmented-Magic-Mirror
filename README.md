# Augmented Magic Mirror

A Magic Mirror that displays 3D objects and augments the mirror image.

## Description

For a detailed project description and setup see its [project site](https://www.julianloehr.de/educational-work/augmented-magic-mirror/).

*By default the DirectX 11 Renderer is used, as DirectX 12 isn't compatible with Stereoscopic 3D. The Renderer can only be changed on compile time (__USE_D3DX11__ in stdafx.h).*

This projects goal was to research whether it is possible to display 3D object on a Magic Mirror. Head tracking via Kinect is used for motion parallax. The depth information of the Kinect is used for occlusion as well. Besides motion parallax the project also features stereoscopic 3D for further depth perception. 

## Features

* DirectX 11 Renderer with Stereo (e.g. NVIDIA 3D Vision)
* DirectX 12 Renderer
* Head Tracking & Motion Parallax
* Occlusion

## Requirements

* Semi-transparent mirror
* Kinect v2 Sensor (including PC-Adapter)
* Windows System that meets the Kinect v2 requirements
* DirectX 11.1 Feature Level Support
* [Microsoft Visual C++ 2015 Redistributable Update 3](https://www.microsoft.com/en-us/download/details.aspx?id=52982)

_For Stereo_

* 120 Hz 3D Vision Ready Display
* NVIDIA 3D Vision Glasses (or any other Stereo glasses)

## Build Requirements

* Visual Stduio 2015 Update 3
* Windows 10 SDK 10.0.10586
* Kinect SDK v2.0

## Setup

For setup connect your Kinect to the PC, place it beneath the monitor and adjust the Settings File. Place the semi-transparent mirror ontop of the monitor screen. Start the program and press __Alt+Enter__ for fullscreen.

## Controls

* **WASDQE:** Adjust Kinect offset
* **+-_(on Numpad)_:** Adjust monitor height
* **Space:** Pause head tracking
* **F:** Colorize depth mesh
* **Alt + Enter:** Toggle fullscreen

## Known Issues

* Changing 3D display mode while in fullscreen will let the app crash.
* Using the DirectX 12 Renderer while 3D display mode is enabled results in a crash on start up.

## License 

This project is licenced under the MIT license (MIT)

## Settings File

_All length units are in Centimetres_

### Monitor

* _Monitorheight_: The height of your display; used to calculate the oblique viewing.

### Kinect

* _OffsetX_, _OffsetY_, _OffsetZ_: The offset of your Kinect Sensor from your display center; used to map the real world face model to the virtual world. The face models origin is the IR camera, that is approx. 4cm to the left from the Kinect center.
