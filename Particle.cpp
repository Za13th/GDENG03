#include "Particle.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Matrix4x4.h"
#include "EngineTime.h"

#include <cmath>

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

Particle::Particle() : GameObject("Particle")
{
	this->vertexes[0] = { Vector3D{ 0.5f, 0.0f, 0.0f }, Vector3D(1, 0, 0), Vector3D(1, 0, 1) };  // Vertex 0
	this->vertexes[1] = { Vector3D{ 0.3536f, 0.3536f, 0.0f }, Vector3D(0, 1, 0), Vector3D(0.5f, 0.5f, 1) };  // Vertex 1
	this->vertexes[2] = { Vector3D{ 0.0f, 0.5f, 0.0f }, Vector3D(0, 0, 0.5f), Vector3D(0.4f, 0.4f, 0.4f) };  // Vertex 2
	this->vertexes[3] = { Vector3D{ -0.3536f, 0.3536f, 0.0f }, Vector3D(1, 1, 0), Vector3D(0, 1, 1) };  // Vertex 3
	this->vertexes[4] = { Vector3D{ -0.5f, 0.0f, 0.0f }, Vector3D(1, 0, 0), Vector3D(0, 0, 0) };  // Vertex 4
	this->vertexes[5] = { Vector3D{ -0.3536f, -0.3536f, 0.0f }, Vector3D(0, 1, 0), Vector3D(0.5f, 0.5f, 1) };  // Vertex 5
	this->vertexes[6] = { Vector3D{ 0.0f, -0.5f, 0.0f }, Vector3D(0, 0, 0.5f), Vector3D(0.4f, 0.4f, 0.4f) };  // Vertex 6
	this->vertexes[7] = { Vector3D{ 0.3536f, -0.3536f, 0.0f }, Vector3D(1, 1, 0), Vector3D(0, 1, 1) };  // Vertex 7


	this->size_vertexes = ARRAYSIZE(vertexes);
}

Particle::~Particle()
{

}

void Particle::Create(void** shader_byte_code, size_t* size_shader)
{
	this->m_vbuffer = GraphicsEngine::get()->createVertexBuffer();
	unsigned int index_list[] =
	{
		0, 1, 2,  
		0, 2, 3,  
		0, 3, 4,  
		0, 4, 5,  
		0, 5, 6,  
		0, 6, 7,  
		0, 7, 1   
	};

	this->m_ibuffer = GraphicsEngine::get()->createIndexBuffer();

	constant cc;
	cc.m_angle = 0;
	this->m_cbuffer = GraphicsEngine::get()->createConstantBuffer();

	m_ibuffer->load(index_list, ARRAYSIZE(index_list));
	m_vbuffer->load(this->vertexes, sizeof(vertex), this->size_vertexes, *shader_byte_code, *size_shader);
	m_cbuffer->load(&cc, sizeof(constant));
}

void Particle::setColor(Vector3D color)
{
	this->vertexes[0].color = color;
	this->vertexes[1].color = color;
	this->vertexes[2].color = color;
	this->vertexes[3].color = color;
	this->vertexes[4].color = color;
	this->vertexes[5].color = color;
	this->vertexes[6].color = color;
	this->vertexes[7].color = color;
}

void Particle::setColor(Vector3D color, int index)
{
	if (index >= 0 && index < 8)
	this->vertexes[index].color = color;
}

void Particle::setColor1(Vector3D color)
{
	for (int i = 0; i < 8; ++i)
	{
		this->vertexes[i].color1 = color;
	}
}

void Particle::setColor1(Vector3D color, int index)
{
	if (index >= 0 && index < 8)
	this->vertexes[index].color1 = color;
}

void Particle::setColorsToEachOther()
{
	for (int i = 0; i < 8; ++i)
	{
		this->vertexes[i].color1 = this->vertexes[i].color;
	}
}

void Particle::update(float deltaTime)
{
	this->lifeTime += deltaTime;

	if(!constantVelocity)
	this->particleVelocity = this->particleVelocity + (this->particleAcceleration * deltaTime);

}

bool Particle::checkActive()
{
	if (this->lifeTime > this->lifeTimeLimit || 
		this->getLocalPosition().x < -1.3f ||
		this->getLocalPosition().x > 1.3f || 
		this->getLocalPosition().y < -1.3f || 
		this->getLocalPosition().y > 1.3f )
	{
		return false;
	}

	return true;

}

void Particle::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	m_angle += EngineTime::getDeltaTime();
	constant cc;
	cc.m_angle = m_angle;


	deltaPos += EngineTime::getDeltaTime() / 10.0f;
	if (deltaPos > 1.0f) deltaPos = 0.0f;


	Matrix4x4 temp;

	cc.m_world.setScale(this->getLocalScale());

	temp.setRotationZ(-(speed * cc.m_angle));
	cc.m_world *= temp;

	this->setPosition(this->getLocalPosition() + particleVelocity);

	temp.setTranslation(this->getLocalPosition());
	cc.m_world *= temp;


	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, -4.0f, 4.0f);
	this->m_cbuffer->update(GraphicsEngine::get()->getDeviceContext(), &cc);


	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(vs, this->m_cbuffer);
	GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(ps, this->m_cbuffer);
	GraphicsEngine::get()->getDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getDeviceContext()->setPixelShader(ps);
	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->m_vbuffer);
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->m_ibuffer);

	//Draw Particle:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleStrip(this->m_ibuffer->getSizeIndexList(), 0, 0);
}

void Particle::Release()
{
	this->m_vbuffer->release();
	this->m_ibuffer->release();
	this->m_cbuffer->release();
}
