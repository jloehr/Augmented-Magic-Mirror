#pragma once

class Mesh;
typedef std::unique_ptr<Mesh> PMesh;

class Mesh
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};
	typedef std::vector<Vertex> VertexList;

	virtual ~Mesh() = default; 
	
	void CreateCube();
	void CreatePlane(_In_ unsigned Width, _In_ unsigned Height);

	virtual void UpdateVertices(const VertexList & Vertices) = 0;

protected:
	typedef uint32_t Index;
	typedef std::vector<Index> IndexList;

	virtual void Create(_In_ const VertexList & Vertices, _In_ const IndexList & Indices) = 0;
};

