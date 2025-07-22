#include "Quad.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Matrix4x4.h"
#include "EngineTime.h"

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

Quad::Quad() : GameObject("Quad")
{   //default rainbow quad 

	this->size_vertexes = ARRAYSIZE(vertexes);
}

void Quad::Create(void** shader_byte_code, size_t* size_shader)
{
	this->m_vbuffer = GraphicsEngine::get()->createVertexBuffer();
	unsigned int index_list[] = 
	{
		0, 1, 2,  
		2, 1, 3   
	};

	this->m_ibuffer = GraphicsEngine::get()->createIndexBuffer();

	constant cc;
	cc.m_angle = 0;
	this->m_cbuffer = GraphicsEngine::get()->createConstantBuffer();

	m_ibuffer->load(index_list, ARRAYSIZE(index_list));
	m_vbuffer->load(this->vertexes, sizeof(vertex), this->size_vertexes, *shader_byte_code, *size_shader);
	m_cbuffer->load(&cc, sizeof(constant));
}

void Quad::setColor(Vector3D color)
{

}

void Quad::setColor(Vector3D color1, Vector3D color2, Vector3D color3, Vector3D color4)
{

}

void Quad::setColor1(Vector3D color)
{

}

void Quad::setColor1(Vector3D color1, Vector3D color2, Vector3D color3, Vector3D color4)
{

}

void Quad::update(float deltaTime)
{

}

void Quad::draw(int width, int height, VertexShader* vs, PixelShader* ps)
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


	
	Vector3D down = Vector3D(0,-0.1f,0) * EngineTime::getDeltaTime();
	this->setPosition(this->getLocalPosition() + down);

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

	//Draw Quad:
	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->m_ibuffer->getSizeIndexList(), 0, 0);
}

void Quad::Release()
{
	this->m_vbuffer->release();
	this->m_ibuffer->release();
	this->m_cbuffer->release();
}