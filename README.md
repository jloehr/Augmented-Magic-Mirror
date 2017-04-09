# Augmented Magic Mirror

A Magic Mirror that displays/projects 3D objects into the mirror image.

## Features

* DirectX 12 Renderer
* DirectX 11 Renderer with Stereo Support
* NVIDIA 3D Vision Support

## Requirements

* Kinect v2 Sensor (including PC-Adapter)
* Windows 10 Version 1511 System (that meets the Kinect v2 requirements)
* DirectX 11 Feature Level graphics card
* [Microsoft Visual C++ 2015 Redistributable Update 3](https://www.microsoft.com/en-us/download/details.aspx?id=52982)

_For Stereo_

* 120 Hz 3D Vision Ready Display
* DirectX 11.1 Feature Support

## Build Requirements

* Visual Stduio 2015 Update 3
* Windows 10 SDK 10.0.10586
* Kinect SDK v2.0

_For Stereo_

* NVAPI in Solution Directory

## License 

This project is licenced under the MIT license (MIT)

## Settings File

_All length units are in Centimetres_

### Monitor

* _Monitorheight_: The height of your display; used to calculate the oblique viewing .

### Kinect

* _OffsetX_, _OffsetY_, _OffsetZ_: The offset of your Kinect Sensor from your display center; used to map the real world face model to the virtual world. The face models origin is the IR camera, that is approx. 4cm to the left from the Kinect center.
