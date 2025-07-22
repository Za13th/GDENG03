#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() :position(), texcoord(), normal() {}
	VertexMesh(Vector3D pos) : position(pos), texcoord(), normal(){}
	VertexMesh (Vector3D pos, Vector2D tcoord): position(pos), texcoord(tcoord) {}
	VertexMesh(Vector3D pos, Vector3D n) : position(pos), normal(n) {}
	VertexMesh(Vector3D pos, Vector2D tcoord, Vector3D n) : position(pos), texcoord(tcoord), normal(n) {}
	VertexMesh(const VertexMesh& vertex) : position(vertex.position), texcoord(vertex.texcoord), normal(vertex.normal) {}

	~VertexMesh() {}

	Vector3D position;
	Vector2D texcoord;
	Vector3D normal;
};