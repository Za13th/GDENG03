#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class DeviceContext;

class Mesh : public Resource
{
public:
	Mesh(const wchar_t* full_path);
	Mesh(const wchar_t* full_path, bool t, bool n);
	~Mesh();

	VertexBuffer* getVertexBuffer() { return m_vertexBuffer; }
	IndexBuffer* getIndexBuffer() { return m_indexBuffer; }

private:
	VertexBuffer* m_vertexBuffer = nullptr;
	IndexBuffer* m_indexBuffer = nullptr;


	friend class DeviceContext;
};

