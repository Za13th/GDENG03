#include "Particle.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Matrix4x4.h"
#include "EngineTime.h"

#include <cmath>

__declspec(align(16))
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;    // 64 bytes
	Matrix4x4 m_view;     // 64 bytes
	Matrix4x4 m_proj;     // 64 bytes
	float m_angle;        // 4 bytes
	float padding[3];     // 12 bytes to align next float4

	float fogStart = 5.0f;       // 4 bytes
	float fogEnd = 20.0f;        // 4 bytes
	float padding2[2];           // 8 bytes

	Vector3D fogColor = { 0.5f, 0.6f, 0.7f };  // 12 bytes
	float fogDensity = 0.0f;                    // 4 bytes to align

	Vector3D cameraPos = { 0.0f, 0.0f, 0.0f }; // 12 bytes
	float padding4 = 0.0f;                    // 4 bytes to align
};

Particle::Particle() : GameObject("Particle")
{
	this->vertexes[0] = { Vector3D{ 0.5000f,  0.0000f, 0.0f }, Vector3D(1.0, 0.0, 0.0), Vector3D(1.0000f, 0.5000f, 1.0) };  // Vertex 0
	this->vertexes[1] = { Vector3D{ 0.4904f,  0.0980f, 0.0f }, Vector3D(1.0, 0.1, 0.0), Vector3D(0.9936f, 0.5327f, 1.0) };  // Vertex 1
	this->vertexes[2] = { Vector3D{ 0.4619f,  0.1913f, 0.0f }, Vector3D(0.9, 0.3, 0.0), Vector3D(0.9810f, 0.5647f, 1.0) };  // Vertex 2
	this->vertexes[3] = { Vector3D{ 0.4156f,  0.2778f, 0.0f }, Vector3D(0.8, 0.4, 0.0), Vector3D(0.9578f, 0.5951f, 1.0) };  // Vertex 3
	this->vertexes[4] = { Vector3D{ 0.3536f,  0.3536f, 0.0f }, Vector3D(0.7, 0.5, 0.0), Vector3D(0.9268f, 0.6236f, 1.0) };  // Vertex 4
	this->vertexes[5] = { Vector3D{ 0.2772f,  0.4156f, 0.0f }, Vector3D(0.6, 0.6, 0.0), Vector3D(0.8886f, 0.6495f, 1.0) };  // Vertex 5
	this->vertexes[6] = { Vector3D{ 0.1888f,  0.4619f, 0.0f }, Vector3D(0.4, 0.7, 0.0), Vector3D(0.8444f, 0.6719f, 1.0) };  // Vertex 6
	this->vertexes[7] = { Vector3D{ 0.0915f,  0.4904f, 0.0f }, Vector3D(0.2, 0.8, 0.0), Vector3D(0.7958f, 0.6902f, 1.0) };  // Vertex 7
	this->vertexes[8] = { Vector3D{ 0.0000f,  0.5000f, 0.0f }, Vector3D(0.0, 0.8, 0.0), Vector3D(0.7500f, 0.7039f, 1.0) };  // Vertex 8
	this->vertexes[9] = { Vector3D{-0.0915f,  0.4904f, 0.0f }, Vector3D(-0.2, 0.9, 0.0), Vector3D(0.7042f, 0.7122f, 1.0) };  // Vertex 9
	this->vertexes[10] = { Vector3D{-0.1888f,  0.4619f, 0.0f }, Vector3D(-0.4, 0.9, 0.0), Vector3D(0.6556f, 0.7154f, 1.0) };  // Vertex 10
	this->vertexes[11] = { Vector3D{-0.2772f,  0.4156f, 0.0f }, Vector3D(-0.6, 0.9, 0.0), Vector3D(0.6114f, 0.7131f, 1.0) };  // Vertex 11
	this->vertexes[12] = { Vector3D{-0.3536f,  0.3536f, 0.0f }, Vector3D(-0.7, 0.8, 0.0), Vector3D(0.5732f, 0.7058f, 1.0) };  // Vertex 12
	this->vertexes[13] = { Vector3D{-0.4156f,  0.2778f, 0.0f }, Vector3D(-0.8, 0.7, 0.0), Vector3D(0.5422f, 0.6934f, 1.0) };  // Vertex 13
	this->vertexes[14] = { Vector3D{-0.4619f,  0.1913f, 0.0f }, Vector3D(-0.9, 0.6, 0.0), Vector3D(0.5190f, 0.6768f, 1.0) };  // Vertex 14
	this->vertexes[15] = { Vector3D{-0.4904f,  0.0980f, 0.0f }, Vector3D(-1.0, 0.5, 0.0), Vector3D(0.5064f, 0.6564f, 1.0) };  // Vertex 15
	this->vertexes[16] = { Vector3D{-0.5000f,  0.0000f, 0.0f }, Vector3D(-1.0, 0.4, 0.0), Vector3D(0.5000f, 0.6332f, 1.0) };  // Vertex 16
	this->vertexes[17] = { Vector3D{-0.4904f, -0.0980f, 0.0f }, Vector3D(-1.0, 0.3, 0.0), Vector3D(0.4936f, 0.6078f, 1.0) };  // Vertex 17
	this->vertexes[18] = { Vector3D{-0.4619f, -0.1913f, 0.0f }, Vector3D(-0.9, 0.2, 0.0), Vector3D(0.4810f, 0.5812f, 1.0) };  // Vertex 18
	this->vertexes[19] = { Vector3D{-0.4156f, -0.2778f, 0.0f }, Vector3D(-0.8, 0.1, 0.0), Vector3D(0.4578f, 0.5539f, 1.0) };  // Vertex 19
	this->vertexes[20] = { Vector3D{-0.3536f, -0.3536f, 0.0f }, Vector3D(-0.7, 0.0, 0.0), Vector3D(0.4268f, 0.5265f, 1.0) };  // Vertex 20
	this->vertexes[21] = { Vector3D{-0.2772f, -0.4156f, 0.0f }, Vector3D(-0.6, -0.0, 0.0), Vector3D(0.3886f, 0.5000f, 1.0) };  // Vertex 21
	this->vertexes[22] = { Vector3D{-0.1888f, -0.4619f, 0.0f }, Vector3D(-0.4, -0.1, 0.0), Vector3D(0.3444f, 0.4735f, 1.0) };  // Vertex 22
	this->vertexes[23] = { Vector3D{-0.0915f, -0.4904f, 0.0f }, Vector3D(-0.2, -0.2, 0.0), Vector3D(0.2958f, 0.4461f, 1.0) };  // Vertex 23
	this->vertexes[24] = { Vector3D{ 0.0000f, -0.5000f, 0.0f }, Vector3D(-0.0, -0.3, 0.0), Vector3D(0.2500f, 0.4188f, 1.0) };  // Vertex 24
	this->vertexes[25] = { Vector3D{ 0.0915f, -0.4904f, 0.0f }, Vector3D(0.2, -0.4, 0.0), Vector3D(0.2042f, 0.3922f, 1.0) };  // Vertex 25
	this->vertexes[26] = { Vector3D{ 0.1888f, -0.4619f, 0.0f }, Vector3D(0.4, -0.5, 0.0), Vector3D(0.1556f, 0.3668f, 1.0) };  // Vertex 26
	this->vertexes[27] = { Vector3D{ 0.2772f, -0.4156f, 0.0f }, Vector3D(0.6, -0.6, 0.0), Vector3D(0.1114f, 0.3436f, 1.0) };  // Vertex 27
	this->vertexes[28] = { Vector3D{ 0.3536f, -0.3536f, 0.0f }, Vector3D(0.7, -0.7, 0.0), Vector3D(0.0732f, 0.3232f, 1.0) };  // Vertex 28
	this->vertexes[29] = { Vector3D{ 0.4156f, -0.2778f, 0.0f }, Vector3D(0.8, -0.8, 0.0), Vector3D(0.0422f, 0.3066f, 1.0) };  // Vertex 29
	this->vertexes[30] = { Vector3D{ 0.4619f, -0.1913f, 0.0f }, Vector3D(0.9, -0.9, 0.0), Vector3D(0.0190f, 0.2942f, 1.0) };  // Vertex 30
	this->vertexes[31] = { Vector3D{ 0.4904f, -0.0980f, 0.0f }, Vector3D(1.0, -0.9, 0.0), Vector3D(0.0064f, 0.2869f, 1.0) };  // Vertex 31


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
	0, 7, 8,
	0, 8, 9,
	0, 9, 10,
	0, 10, 11,
	0, 11, 12,
	0, 12, 13,
	0, 13, 14,
	0, 14, 15,
	0, 15, 16,
	0, 16, 17,
	0, 17, 18,
	0, 18, 19,
	0, 19, 20,
	0, 20, 21,
	0, 21, 22,
	0, 22, 23,
	0, 23, 24,
	0, 24, 25,
	0, 25, 26,
	0, 26, 27,
	0, 27, 28,
	0, 28, 29,
	0, 29, 30,
	0, 30, 31,
	0, 31, 32,
	0, 32, 1
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
	for (int i = 0; i < 32; ++i)
	{
		this->vertexes[i].color = color;
	}
}

void Particle::setColor(Vector3D color, int index)
{
	if (index >= 0 && index < 32)
	this->vertexes[index].color = color;
}

void Particle::setColor1(Vector3D color)
{
	for (int i = 0; i < 32; ++i)
	{
		this->vertexes[i].color1 = color;
	}
}

void Particle::setColor1(Vector3D color, int index)
{
	if (index >= 0 && index < 32)
	this->vertexes[index].color1 = color;
}

void Particle::setColorsToEachOther()
{
	for (int i = 0; i < 32; ++i)
	{
		this->vertexes[i].color1 = this->vertexes[i].color;
	}
}

void Particle::update(float deltaTime)
{
	this->lifeTime += deltaTime;

	if(!constantVelocity)
	this->particleVelocity = this->particleVelocity + (this->particleAcceleration * deltaTime);
	else
	{
		if (this->getLocalPosition().x + 0.08 >= 1.28f || this->getLocalPosition().x - 0.08 <= -1.28f)
			this->particleVelocity.x *= -1.0f;
		if (this->getLocalPosition().y + 0.1 >= 0.96f || this->getLocalPosition().y - 0.1 <= -0.96f)
			this->particleVelocity.y *= -1.0f;
	}



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

	cc.fogStart = 1.0f; 
	cc.fogEnd = 10.0f; 
	cc.fogDensity = 0.0f; // No fog density for this particle
	cc.fogColor = Vector3D(0.5f, 0.6f, 0.7f); 

	cc.cameraPos = this->getLocalPosition();

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
