#include "Mesh.h"

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <vector>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <iostream>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
	tinyobj::attrib_t attribs;

	bool t = false, n = false;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn, err;
	auto inputfile = std::filesystem::path(full_path).string();

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (attribs.texcoords.size() > 0)
		t = true;

	if (attribs.normals.size() > 0)
		n = true;

	if (!err.empty() || !res || shapes.size() > 1)
		throw std::exception("Mesh Creation Failed!\n");

	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;


		vertices.reserve(shapes[s].mesh.indices.size());
		indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_vertices = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_vertices; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[3 * index.vertex_index + 0];
				tinyobj::real_t vy = attribs.vertices[3 * index.vertex_index + 1];
				tinyobj::real_t vz = attribs.vertices[3 * index.vertex_index + 2];

				tinyobj::real_t tx;
				tinyobj::real_t ty;

				tinyobj::real_t nx;
				tinyobj::real_t ny;
				tinyobj::real_t nz;
				if (t)
				{
					tx = attribs.texcoords[2 * index.texcoord_index + 0];
					ty = attribs.texcoords[2 * index.texcoord_index + 1];
				}

				if (n)
				{
					nx = attribs.normals[3 * index.normal_index + 0];
					ny = attribs.normals[3 * index.normal_index + 1];
					nz = attribs.normals[3 * index.normal_index + 2];
				}

				if (t && n)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
					vertices.push_back(vertex);
				}
				else if (n)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector3D(nx, ny, nz));
					vertices.push_back(vertex);
				}
				else if (t)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					vertices.push_back(vertex);
				}
				else
				{
					VertexMesh vertex(Vector3D(vx, vy, vz));
					vertices.push_back(vertex);
				}


				indices.push_back(index_offset + v);
			}

			index_offset += num_face_vertices;

		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	this->m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->m_vertexBuffer->load(&vertices[0], sizeof(VertexMesh), (UINT)vertices.size(), shader_byte_code, size_shader);

	this->m_indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->m_indexBuffer->load(&indices[0], (UINT)indices.size());

}

Mesh::Mesh(const wchar_t* full_path, bool t, bool n) : Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn, err;
	auto inputfile = std::filesystem::path(full_path).string();

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty() || !res || shapes.size() > 1)
		throw std::exception("Mesh Creation Failed!\n");

	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;


		vertices.reserve(shapes[s].mesh.indices.size());
		indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_vertices = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_vertices; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[3 * index.vertex_index + 0];
				tinyobj::real_t vy = attribs.vertices[3 * index.vertex_index + 1];
				tinyobj::real_t vz = attribs.vertices[3 * index.vertex_index + 2];

				tinyobj::real_t tx;
				tinyobj::real_t ty;

				tinyobj::real_t nx;
				tinyobj::real_t ny;
				tinyobj::real_t nz;
				if (t)
				{
					tx = attribs.texcoords[2 * index.texcoord_index + 0];
					ty = attribs.texcoords[2 * index.texcoord_index + 1];
				}

				if (n)
				{
					nx = attribs.normals[3 * index.normal_index + 0];
					ny = attribs.normals[3 * index.normal_index + 1];
					nz = attribs.normals[3 * index.normal_index + 2];
				}

				if (t && n)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx,ny,nz));
					vertices.push_back(vertex);
				}
				else if (n)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector3D(nx, ny, nz));
					vertices.push_back(vertex);
				}
				else if (t)
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					vertices.push_back(vertex);
				}
				else
				{
					VertexMesh vertex(Vector3D(vx, vy, vz));
					vertices.push_back(vertex);
				}


				indices.push_back(index_offset + v);
			}

			index_offset += num_face_vertices;

		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	this->m_vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->m_vertexBuffer->load(&vertices[0], sizeof(VertexMesh), (UINT)vertices.size(), shader_byte_code, size_shader);

	this->m_indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->m_indexBuffer->load(&indices[0], (UINT)indices.size());

}

Mesh::~Mesh()
{
	this->m_vertexBuffer->release();
	this->m_indexBuffer->release();
}