#include "Cube.h"
#include "structs.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "SwapChain.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "SceneCameraHolder.h"
#include <iostream>

#include <cstdlib>
#include <ctime>

/*__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};*/


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
	float padding4 = 0.0f;                    
};

Cube::Cube(std::string name, void* shaderByteCode, size_t sizeShader) : GameObject(name)
{

	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ Vector3D(-0.5f, -0.5f, -0.5f) , Vector3D(1,0,0),  Vector3D(1,0,0)},
		{ Vector3D(-0.5f, 0.5f, -0.5f) ,   Vector3D(1,1,0),   Vector3D(1,1,0) },
		{ Vector3D(0.5f, 0.5f, -0.5f) ,  Vector3D(1,0,1), Vector3D(1,0,1) },
		{ Vector3D(0.5f, -0.5f, -0.5f),  Vector3D(0,1,0),    Vector3D(0,1,0)},

		{ Vector3D(0.5f, -0.5f, 0.5f) ,   Vector3D(1,1,1), Vector3D(1,1,1)},
		{ Vector3D(0.5f, 0.5f, 0.5f) ,   Vector3D(0,0,1),  Vector3D(0,0,1) },
		{ Vector3D(-0.5f, 0.5f, 0.5f) , Vector3D(0,1,1),   Vector3D(0,1,1) },
		{ Vector3D(-0.5f, -0.5f, 0.5f),  Vector3D(0.1,0.1,0.1),   Vector3D(0.1,0.1,0.1)}
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

Cube::Cube(std::string name,Vector3D Color ,void* shaderByteCode, size_t sizeShader) : GameObject(name)
{

	vertex vertex_list[] =
	{//    X     Y     Z
		//Rainbow
		{ Vector3D(-0.5f, -0.5f, -0.5f) , Color,  Color},
		{ Vector3D(-0.5f, 0.5f, -0.5f) ,   Color,   Color },
		{ Vector3D(0.5f, 0.5f, -0.5f) ,  Color, Color },
		{ Vector3D(0.5f, -0.5f, -0.5f),  Color,    Color},

		{ Vector3D(0.5f, -0.5f, 0.5f) ,   Color, Color},
		{ Vector3D(0.5f, 0.5f, 0.5f) ,   Color,  Color},
		{ Vector3D(-0.5f, 0.5f, 0.5f) , Color,   Color },
		{ Vector3D(-0.5f, -0.5f, 0.5f),  Color,  Color}
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

Cube::~Cube()
{

}

void Cube::update(float deltaTime)
{
	this->ticks += deltaTime;
	this->deltaPos = this->speed * deltaTime;
}

void Cube::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	static float m_angle = 0;
	m_angle +=  EngineTime::getDeltaTime();
	constant cc;
	cc.m_angle = m_angle;



	deltaPos += EngineTime::getDeltaTime() / 10.0f;
	if (deltaPos > 1.0f) deltaPos = 0.0f;


	Matrix4x4 temp;

	this->deltaScale += EngineTime::getDeltaTime() / 10.f;
	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(this->deltaScale) + 1.0f)/2.0f));
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5, -1.5, 0), Vector3D(1.5, 1.5, 0), this->deltaPos));
	//cc.m_world *= temp;

	//for continuous rotation :
		//this->setRotation(this->getLocalRotation().x + EngineTime::getDeltaTime() * 2.0f,
		//this->getLocalRotation().y + EngineTime::getDeltaTime() * 2.0f, 
		//this->getLocalRotation().z + EngineTime::getDeltaTime() * 2.0f);

	//for Test Case #5:
	/*	static bool reverse = false;
	float deltaSize = 0.5f;

	if (!reverse)
	{
		float changeinSize = deltaSize * EngineTime::getDeltaTime();
		this->setScale(this->getLocalScale() + Vector3D(changeinSize, -changeinSize, changeinSize));

		if (getLocalScale().y <= 0.1f)
		{
			this->setScale(Vector3D(this->getLocalScale().x, 0.1f, this->getLocalScale().z));
			reverse = true;
		}
	}
	else if (reverse)
	{
		float changeinSize = deltaSize * EngineTime::getDeltaTime();
		this->setScale(this->getLocalScale() - Vector3D(changeinSize, -changeinSize, changeinSize));

		if (getLocalScale().y >= 1.0f)
		{
			this->setScale(Vector3D(1.0f, 1.0f, 1.0f));
			reverse = false;
		}
	}*/





	cc.m_world.setScale(this->getLocalScale());
	temp.setRotationZ(this->getLocalRotation().z);
	cc.m_world *= temp;
	temp.setRotationY(this->getLocalRotation().y);
	cc.m_world *= temp;
	temp.setRotationX(this->getLocalRotation().x);
	cc.m_world *= temp;

	temp.setTranslation(this->getLocalPosition());
	cc.m_world *= temp;


	auto world_cam = SceneCameraHolder::getInstance()->getCamera()->getViewMatrix();
	world_cam.inverse();
	cc.m_view = world_cam;


	static float fog_start = 1.5f;
	static float fog_end = 5.0f; 
	static float fog_density = 3.5f; 

	// Fog start distance controls
	if (InputSystem::get()->isKeyDown('Z'))
	{
		fog_start = max(0.0f, fog_start - 0.1f);
	}
	if (InputSystem::get()->isKeyDown('C'))
	{
		fog_start += 0.1f;
		fog_start = min(fog_start, fog_end - 0.1f); // Ensure fog start is less than fog end)
	}

	// Fog end distance controls
	if (InputSystem::get()->isKeyDown('V'))
	{
		fog_end = max(fog_start + 0.1f, fog_end - 0.1f);
	}
	if (InputSystem::get()->isKeyDown('B'))
	{
		fog_end += 0.1f;
	}

	// Fog density controls
	if (InputSystem::get()->isKeyDown('N'))
	{
		fog_density = max(0.001f, fog_density - 0.01f);
	}
	if (InputSystem::get()->isKeyDown('M'))
	{
		fog_density += 0.01f;
	}


	cc.fogStart = fog_start; 
	cc.fogEnd = fog_end; 
	cc.fogDensity = fog_density; 
	cc.fogColor = { 0.6f, 0.6f, 0.6f }; 

	cc.cameraPos = SceneCameraHolder::getInstance()->getCamera()->getLocalPosition();


	//cc.m_view.setIdentity();
	//cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f); 
	//cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);
	cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, fog_end * 0.21);
	this->constantBuffer->update(GraphicsEngine::get()->getDeviceContext(), &cc);

	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(vs, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(ps, this->constantBuffer);
	GraphicsEngine::get()->getDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(ps);
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->vertexBuffer);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->indexBuffer);

	//Draw Cube:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(indexBuffer->getSizeIndexList(), 0, 0);

}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void Cube::release()
{
	if (this->vertexBuffer)
		this->vertexBuffer->release();
	if (this->indexBuffer)
		this->indexBuffer->release();
	if (this->constantBuffer)
		this->constantBuffer->release();
}