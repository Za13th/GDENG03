#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Plane.h"
#include "structs.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "SwapChain.h"
#include "Matrix4x4.h"
#include "SceneCameraHolder.h"
#include "InputSystem.h"
#include "FogSystem.h"
#include <iostream>

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

Plane::Plane(std::string name, void* shaderByteCode, size_t sizeShader) : GameObject(name)
{
	this->objectType = GameObject::Plane;
	this->texture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\defaultSquare.jpg");

	Vector3D position_list[] =
	{
		Vector3D(-5.0f, -0.005f, -5.0f),
		Vector3D(-5.0f, 0.005f, -5.0f),
		Vector3D(5.0f, 0.005f, -5.0f),
		Vector3D(5.0f, -0.005f, -5.0f),

		Vector3D(5.0f, -0.005f, 5.0f),
		Vector3D(5.0f, 0.005f, 5.0f),
		Vector3D(-5.0f, 0.005f, 5.0f),
		Vector3D(-5.0f, -0.005f, 5.0f)
	};

	Vector3D color_list[] =
	{
		Vector3D(1, 0, 0),   // Red
		Vector3D(1, 1, 0),   // Yellow
		Vector3D(1, 0, 1),   // Magenta
		Vector3D(0, 1, 0),   // Green
		Vector3D(1, 1, 1),   // White
		Vector3D(0, 0, 1),   // Blue
		Vector3D(0, 1, 1),   // Cyan
		Vector3D(0.1f, 0.1f, 0.1f) // Dark Gray
	};

	Vector2D texcoord_list[] =
	{
		Vector2D(0, 1),
		Vector2D(0, 0),
		Vector2D(1, 0),
		Vector2D(1, 1)
	};

	texcoord_list[0] *= 2.0f;
	texcoord_list[1] *= 2.0f;
	texcoord_list[2] *= 2.0f;
	texcoord_list[3] *= 2.0f;

	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ position_list[0], texcoord_list[0] },
		{ position_list[1], texcoord_list[1] },
		{ position_list[2], texcoord_list[2] },
		{ position_list[3], texcoord_list[3] },

		{ position_list[4], texcoord_list[0] },
		{ position_list[5], texcoord_list[1] },
		{ position_list[6], texcoord_list[2] },
		{ position_list[7], texcoord_list[3] },

		{ position_list[1], texcoord_list[0] },
		{ position_list[6], texcoord_list[1] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[2], texcoord_list[3] },

		{ position_list[7], texcoord_list[0] },
		{ position_list[0], texcoord_list[1] },
		{ position_list[3], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		{ position_list[3], texcoord_list[0] },
		{ position_list[2], texcoord_list[1] },
		{ position_list[5], texcoord_list[2] },
		{ position_list[4], texcoord_list[3] },

		{ position_list[7], texcoord_list[0] },
		{ position_list[6], texcoord_list[1] },
		{ position_list[1], texcoord_list[2] },
		{ position_list[0], texcoord_list[3] }
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	constant cc;
	cc.m_angle = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();

	this->indexBuffer->load(index_list, size_index_list);
	this->vertexBuffer->load(vertex_list, sizeof(vertex), size_list, shaderByteCode, sizeShader);
	this->constantBuffer->load(&cc, sizeof(constant));
}

Plane::~Plane()
{

}

void Plane::update(float deltaTime)
{
	m_angle += deltaTime;

	if (this->localScale.y != 1.0f)
	{
		this->localScale.y = 1.0f;
	}

	this->position[0] = this->getLocalPosition().x;
	this->position[1] = this->getLocalPosition().y;
	this->position[2] = this->getLocalPosition().z;

	this->scale[0] = this->getLocalScale().x;
	this->scale[1] = this->getLocalScale().y;
	this->scale[2] = this->getLocalScale().z;

	this->rotation[0] = this->getLocalRotation().x * (180.0 / M_PI);
	this->rotation[1] = this->getLocalRotation().y * (180.0 / M_PI);
	this->rotation[2] = this->getLocalRotation().z * (180.0 / M_PI);

}

void Plane::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	this->localScale.y = 1.0f;

	constant cc;
	cc.m_angle = m_angle;

	cc.m_world.setIdentity();
	if (this->findComponentByType(Component::Physics, name + " P6 Component"))
		cc.m_world.setScale(this->getLocalScale());
	cc.m_world *= this->localMatrix;


	auto world_cam = SceneCameraHolder::getInstance()->getCamera()->getViewMatrix();
	world_cam.inverse();
	cc.m_view = world_cam;


	//cc.m_view.setIdentity();
	//cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f); 
	//For Fog

	cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);

	//For Culling
	//cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, fog_end * 0.21);
	this->constantBuffer->update(GraphicsEngine::get()->getDeviceContext(), &cc);

	//std::cout << "My Gameobject is Updating: " << this->name << " : " << getLocalPosition().y << std::endl;

	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(vs, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(ps, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(ps);

	//Set Texture:
	if (!this->findComponentByType(Component::Material, name + " TX Component"))
	{
		GraphicsEngine::get()->getDeviceContext()->setTexture(ps, this->texture);
	}
	else
	{
		GraphicsEngine::get()->getDeviceContext()->setTexture(ps, ((TextureComponent*)(this->findComponentByType(Component::Material, name + " TX Component")))->getTexture());
	}

	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->indexBuffer);

	//Draw Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(indexBuffer->getSizeIndexList(), 0, 0);

}

void Plane::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void Plane::release()
{
	if (this->vertexBuffer)
		this->vertexBuffer->release();
	if (this->indexBuffer)
		this->indexBuffer->release();
	if (this->constantBuffer)
		this->constantBuffer->release();
}