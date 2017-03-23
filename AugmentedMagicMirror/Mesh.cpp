// Mesh.cpp : Represents a Mesh with its VertexBuffer, VertexIndices and Shader
//

#include "stdafx.h"
#include "Mesh.h"

void Mesh::CreateCube()
{
	VertexList CubeVertices =
	{
		{ { -0.5f, -0.5f, -0.5f },{ 0.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.5f,  0.5f, -0.5f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.5f,  0.5f,  0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
	};

	IndexList CubeIndices =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	Create(CubeVertices, CubeIndices);
}

void Mesh::CreatePlane(unsigned Width, unsigned Height)
{
	size_t VerticesCount = Width * Height;

	VertexList Vertices(VerticesCount);
	IndexList Indices;
	Indices.reserve((Width - 1) * (Height - 1) * 3 * 2);

	for (size_t Y = 0; Y < Height; ++Y)
		for (size_t X = 0; X < Width; ++X)
		{
			Index UpperLeftIndex = static_cast<Index>(X + (Y * Width));

			Indices.push_back(UpperLeftIndex);
			Indices.push_back(UpperLeftIndex + 1);
			Indices.push_back(UpperLeftIndex + Width);

			Indices.push_back(UpperLeftIndex + 1);
			Indices.push_back(UpperLeftIndex + Width + 1);
			Indices.push_back(UpperLeftIndex + Width);
		}

	Create(Vertices, Indices);
}
