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
	this->vertexes[0] = { Vector3D{ 0.5000f, 0.0000f, 0.0f }, Vector3D(1.0, 0.0, 0.0), Vector3D(1.0000f, 0.5000f, 1.0) };  // Vertex 0
	this->vertexes[1] = { Vector3D{ 0.4904f, 0.0654f, 0.0f }, Vector3D(1.0, 0.1, 0.0), Vector3D(0.9936f, 0.5327f, 1.0) };  // Vertex 1
	this->vertexes[2] = { Vector3D{ 0.4619f, 0.1294f, 0.0f }, Vector3D(0.9, 0.3, 0.0), Vector3D(0.9810f, 0.5647f, 1.0) };  // Vertex 2
	this->vertexes[3] = { Vector3D{ 0.4156f, 0.1902f, 0.0f }, Vector3D(0.8, 0.4, 0.0), Vector3D(0.9578f, 0.5951f, 1.0) };  // Vertex 3
	this->vertexes[4] = { Vector3D{ 0.3536f, 0.2472f, 0.0f }, Vector3D(0.7, 0.5, 0.0), Vector3D(0.9268f, 0.6236f, 1.0) };  // Vertex 4
	this->vertexes[5] = { Vector3D{ 0.2772f, 0.2989f, 0.0f }, Vector3D(0.6, 0.6, 0.0), Vector3D(0.8886f, 0.6495f, 1.0) };  // Vertex 5
	this->vertexes[6] = { Vector3D{ 0.1888f, 0.3438f, 0.0f }, Vector3D(0.4, 0.7, 0.0), Vector3D(0.8444f, 0.6719f, 1.0) };  // Vertex 6
	this->vertexes[7] = { Vector3D{ 0.0915f, 0.3804f, 0.0f }, Vector3D(0.2, 0.8, 0.0), Vector3D(0.7958f, 0.6902f, 1.0) };  // Vertex 7
	this->vertexes[8] = { Vector3D{ -0.0000f, 0.4077f, 0.0f }, Vector3D(0.0, 0.8, 0.0), Vector3D(0.7500f, 0.7039f, 1.0) };  // Vertex 8
	this->vertexes[9] = { Vector3D{ -0.0915f, 0.4245f, 0.0f }, Vector3D(-0.2, 0.9, 0.0), Vector3D(0.7042f, 0.7122f, 1.0) };  // Vertex 9
	this->vertexes[10] = { Vector3D{ -0.1888f, 0.4307f, 0.0f }, Vector3D(-0.4, 0.9, 0.0), Vector3D(0.6556f, 0.7154f, 1.0) };  // Vertex 10
	this->vertexes[11] = { Vector3D{ -0.2772f, 0.4263f, 0.0f }, Vector3D(-0.6, 0.9, 0.0), Vector3D(0.6114f, 0.7131f, 1.0) };  // Vertex 11
	this->vertexes[12] = { Vector3D{ -0.3536f, 0.4115f, 0.0f }, Vector3D(-0.7, 0.8, 0.0), Vector3D(0.5732f, 0.7058f, 1.0) };  // Vertex 12
	this->vertexes[13] = { Vector3D{ -0.4156f, 0.3869f, 0.0f }, Vector3D(-0.8, 0.7, 0.0), Vector3D(0.5422f, 0.6934f, 1.0) };  // Vertex 13
	this->vertexes[14] = { Vector3D{ -0.4619f, 0.3536f, 0.0f }, Vector3D(-0.9, 0.6, 0.0), Vector3D(0.5190f, 0.6768f, 1.0) };  // Vertex 14
	this->vertexes[15] = { Vector3D{ -0.4904f, 0.3129f, 0.0f }, Vector3D(-1.0, 0.5, 0.0), Vector3D(0.5064f, 0.6564f, 1.0) };  // Vertex 15
	this->vertexes[16] = { Vector3D{ -0.5000f, 0.2664f, 0.0f }, Vector3D(-1.0, 0.4, 0.0), Vector3D(0.5000f, 0.6332f, 1.0) };  // Vertex 16
	this->vertexes[17] = { Vector3D{ -0.4904f, 0.2157f, 0.0f }, Vector3D(-1.0, 0.3, 0.0), Vector3D(0.4936f, 0.6078f, 1.0) };  // Vertex 17
	this->vertexes[18] = { Vector3D{ -0.4619f, 0.1624f, 0.0f }, Vector3D(-0.9, 0.2, 0.0), Vector3D(0.4810f, 0.5812f, 1.0) };  // Vertex 18
	this->vertexes[19] = { Vector3D{ -0.4156f, 0.1077f, 0.0f }, Vector3D(-0.8, 0.1, 0.0), Vector3D(0.4578f, 0.5539f, 1.0) };  // Vertex 19
	this->vertexes[20] = { Vector3D{ -0.3536f, 0.0530f, 0.0f }, Vector3D(-0.7, 0.0, 0.0), Vector3D(0.4268f, 0.5265f, 1.0) };  // Vertex 20
	this->vertexes[21] = { Vector3D{ -0.2772f, -0.0000f, 0.0f }, Vector3D(-0.6, -0.0, 0.0), Vector3D(0.3886f, 0.5000f, 1.0) };  // Vertex 21
	this->vertexes[22] = { Vector3D{ -0.1888f, -0.0530f, 0.0f }, Vector3D(-0.4, -0.1, 0.0), Vector3D(0.3444f, 0.4735f, 1.0) };  // Vertex 22
	this->vertexes[23] = { Vector3D{ -0.0915f, -0.1077f, 0.0f }, Vector3D(-0.2, -0.2, 0.0), Vector3D(0.2958f, 0.4461f, 1.0) };  // Vertex 23
	this->vertexes[24] = { Vector3D{ -0.0000f, -0.1624f, 0.0f }, Vector3D(-0.0, -0.3, 0.0), Vector3D(0.2500f, 0.4188f, 1.0) };  // Vertex 24
	this->vertexes[25] = { Vector3D{ 0.0915f, -0.2157f, 0.0f }, Vector3D(0.2, -0.4, 0.0), Vector3D(0.2042f, 0.3922f, 1.0) };  // Vertex 25
	this->vertexes[26] = { Vector3D{ 0.1888f, -0.2664f, 0.0f }, Vector3D(0.4, -0.5, 0.0), Vector3D(0.1556f, 0.3668f, 1.0) };  // Vertex 26
	this->vertexes[27] = { Vector3D{ 0.2772f, -0.3129f, 0.0f }, Vector3D(0.6, -0.6, 0.0), Vector3D(0.1114f, 0.3436f, 1.0) };  // Vertex 27
	this->vertexes[28] = { Vector3D{ 0.3536f, -0.3536f, 0.0f }, Vector3D(0.7, -0.7, 0.0), Vector3D(0.0732f, 0.3232f, 1.0) };  // Vertex 28
	this->vertexes[29] = { Vector3D{ 0.4156f, -0.3869f, 0.0f }, Vector3D(0.8, -0.8, 0.0), Vector3D(0.0422f, 0.3066f, 1.0) };  // Vertex 29
	this->vertexes[30] = { Vector3D{ 0.4619f, -0.4115f, 0.0f }, Vector3D(0.9, -0.9, 0.0), Vector3D(0.0190f, 0.2942f, 1.0) };  // Vertex 30
	this->vertexes[31] = { Vector3D{ 0.4904f, -0.4263f, 0.0f }, Vector3D(1.0, -0.9, 0.0), Vector3D(0.0064f, 0.2869f, 1.0) };  // Vertex 31
	this->vertexes[32] = { Vector3D{ 0.5000f, -0.4307f, 0.0f }, Vector3D(1.0, -0.9, 0.0), Vector3D(0.0000f, 0.2846f, 1.0) };  // Vertex 32
	this->vertexes[33] = { Vector3D{ 0.4904f, -0.4245f, 0.0f }, Vector3D(1.0, -0.9, 0.0), Vector3D(0.0064f, 0.2878f, 1.0) };  // Vertex 33
	this->vertexes[34] = { Vector3D{ 0.4619f, -0.4077f, 0.0f }, Vector3D(0.9, -0.8, 0.0), Vector3D(0.0190f, 0.2961f, 1.0) };  // Vertex 34
	this->vertexes[35] = { Vector3D{ 0.4156f, -0.3804f, 0.0f }, Vector3D(0.8, -0.7, 0.0), Vector3D(0.0422f, 0.3098f, 1.0) };  // Vertex 35
	this->vertexes[36] = { Vector3D{ 0.3536f, -0.3438f, 0.0f }, Vector3D(0.7, -0.6, 0.0), Vector3D(0.0732f, 0.3281f, 1.0) };  // Vertex 36
	this->vertexes[37] = { Vector3D{ 0.2772f, -0.2989f, 0.0f }, Vector3D(0.6, -0.5, 0.0), Vector3D(0.1114f, 0.3505f, 1.0) };  // Vertex 37
	this->vertexes[38] = { Vector3D{ 0.1888f, -0.2472f, 0.0f }, Vector3D(0.4, -0.4, 0.0), Vector3D(0.1556f, 0.3764f, 1.0) };  // Vertex 38
	this->vertexes[39] = { Vector3D{ 0.0915f, -0.1902f, 0.0f }, Vector3D(0.2, -0.3, 0.0), Vector3D(0.2042f, 0.4049f, 1.0) };  // Vertex 39
	this->vertexes[40] = { Vector3D{ 0.0000f, -0.1294f, 0.0f }, Vector3D(0.0, -0.2, 0.0), Vector3D(0.2500f, 0.4353f, 1.0) };  // Vertex 40
	this->vertexes[41] = { Vector3D{ -0.0915f, -0.0654f, 0.0f }, Vector3D(-0.2, -0.1, 0.0), Vector3D(0.2958f, 0.4673f, 1.0) };  // Vertex 41
	this->vertexes[42] = { Vector3D{ -0.1888f, 0.0000f, 0.0f }, Vector3D(-0.4, 0.0, 0.0), Vector3D(0.3444f, 0.5000f, 1.0) };  // Vertex 42
	this->vertexes[43] = { Vector3D{ -0.2772f, 0.0654f, 0.0f }, Vector3D(-0.6, 0.1, 0.0), Vector3D(0.3886f, 0.5327f, 1.0) };  // Vertex 43
	this->vertexes[44] = { Vector3D{ -0.3536f, 0.1294f, 0.0f }, Vector3D(-0.7, 0.3, 0.0), Vector3D(0.4268f, 0.5647f, 1.0) };  // Vertex 44
	this->vertexes[45] = { Vector3D{ -0.4156f, 0.1902f, 0.0f }, Vector3D(-0.8, 0.4, 0.0), Vector3D(0.4578f, 0.5951f, 1.0) };  // Vertex 45
	this->vertexes[46] = { Vector3D{ -0.4619f, 0.2472f, 0.0f }, Vector3D(-0.9, 0.5, 0.0), Vector3D(0.4810f, 0.6236f, 1.0) };  // Vertex 46
	this->vertexes[47] = { Vector3D{ -0.4904f, 0.2989f, 0.0f }, Vector3D(-1.0, 0.6, 0.0), Vector3D(0.4936f, 0.6495f, 1.0) };  // Vertex 47



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
