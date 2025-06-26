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

#include <cstdlib>
#include <ctime>

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;    
	Matrix4x4 m_view;     
	Matrix4x4 m_proj;     
	float m_angle;        
	float padding[3];     

	float fogStart = 5.0f;       
	float fogEnd = 20.0f;       
	float padding2[2];          

	Vector3D fogColor = { 0.5f, 0.6f, 0.7f }; 
	float fogDensity = 0.0f;                    

	Vector3D cameraPos = { 0.0f, 0.0f, 0.0f }; 
	int fogState;
	int culling;
};

Plane::Plane(std::string name, void* shaderByteCode, size_t sizeShader) : GameObject(name)
{

	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ Vector3D(-0.5f, -0.0001f, -0.5f) , Vector3D(1),  Vector3D(1)},
		{ Vector3D(-0.5f, 0.0001f, -0.5f) ,   Vector3D(1),   Vector3D(1) },
		{ Vector3D(0.5f, 0.0001f, -0.5f) ,  Vector3D(1), Vector3D(1) },
		{ Vector3D(0.5f, -0.0001f, -0.5f),  Vector3D(1),    Vector3D(1)},

		{ Vector3D(0.5f, -0.0001f, 0.5f) ,   Vector3D(1), Vector3D(1)},
		{ Vector3D(0.5f, 0.0001f, 0.5f) ,   Vector3D(1),  Vector3D(1) },
		{ Vector3D(-0.5f, 0.0001f, 0.5f) , Vector3D(1),   Vector3D(1) },
		{ Vector3D(-0.5f, -0.0001f, 0.5f),  Vector3D(1),   Vector3D(1)}
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
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

Plane::Plane(std::string name, Vector3D color ,void* shaderByteCode, size_t sizeShader) : GameObject(name)
{

	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ Vector3D(-0.5f, -0.0001f, -0.5f) , color,  color},
		{ Vector3D(-0.5f, 0.0001f, -0.5f) ,   color,   color },
		{ Vector3D(0.5f, 0.0001f, -0.5f) , color, color },
		{ Vector3D(0.5f, -0.0001f, -0.5f),  color,    color},

		{ Vector3D(0.5f, -0.0001f, 0.5f) ,   color, color},
		{ Vector3D(0.5f, 0.0001f, 0.5f) ,   color,  color},
		{ Vector3D(-0.5f, 0.0001f, 0.5f) , color,   color },
		{ Vector3D(-0.5f, -0.0001f, 0.5f),  color,   color}
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 6, 5,
		5, 2, 1,
		7, 0, 3,
		3, 4, 7,
		3, 2, 5,
		5, 4, 3,
		7, 6, 1,
		1, 0, 7
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
	this->ticks += deltaTime;
	this->deltaPos = this->speed * deltaTime;
}

void Plane::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	static float m_angle = 0;
	m_angle += EngineTime::getDeltaTime();
	constant cc;
	cc.m_angle = m_angle;



	deltaPos += EngineTime::getDeltaTime() / 10.0f;
	if (deltaPos > 1.0f) deltaPos = 0.0f;


	Matrix4x4 temp;

	this->deltaScale += EngineTime::getDeltaTime() / 10.f;
	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(this->deltaScale) + 1.0f)/2.0f));
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5, -1.5, 0), Vector3D(1.5, 1.5, 0), this->deltaPos));
	//cc.m_world *= temp;


	cc.m_world.setScale(this->getLocalScale());
	temp.setRotationZ(this->getLocalRotation().z);
	//temp.setRotationZ(m_angle * deltaPos);
	cc.m_world *= temp;
	temp.setRotationY(this->getLocalRotation().y);
	cc.m_world *= temp;
	temp.setRotationX(this->getLocalRotation().x);
	//temp.setRotationX(m_angle * deltaPos);
	cc.m_world *= temp;

	temp.setTranslation(this->getLocalPosition());
	cc.m_world *= temp;

	auto world_cam = SceneCameraHolder::getInstance()->getCamera()->getViewMatrix();
	world_cam.inverse();
	cc.m_view = world_cam;

	cc.fogStart = FogSystem::getInstance()->getFogStart();
	cc.fogEnd = FogSystem::getInstance()->getFogEnd();
	cc.fogDensity = FogSystem::getInstance()->getFogDensity();
	cc.fogColor = FogSystem::getInstance()->getFogColor();
	cc.fogState = FogSystem::getInstance()->getFogState();
	cc.culling = FogSystem::getInstance()->isCullingEnabled();
	cc.cameraPos = SceneCameraHolder::getInstance()->getCamera()->getLocalPosition();


	//cc.m_view.setIdentity();
	//cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f); 
	//For Fog
	cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);

	//For Culling (Use With Simple Fog)
	//cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, fog_end * 0.21);
	this->constantBuffer->update(GraphicsEngine::get()->getDeviceContext(), &cc);



	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(vs, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(ps, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(ps);
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->indexBuffer);

	//Draw Plane:
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