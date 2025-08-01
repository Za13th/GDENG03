#define _USE_MATH_DEFINES
#include <cmath>

#include "MeshObject.h"
#include <iostream>
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "Matrix4x4.h"
#include "SceneCameraHolder.h"
#include "MeshManager.h"
#include "TextureManager.h"




__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};

MeshObject::MeshObject(std::string name, Mesh* mesh, Texture* texture) : GameObject(name)
{
	this->objectType = GameObject::MeshObject;
	this->texture = texture;
	this->mesh = mesh;


	constant cc;
	cc.m_angle = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();

	this->constantBuffer->load(&cc, sizeof(constant));
}

MeshObject::~MeshObject()
{

}

void MeshObject::update(float deltaTime)
{
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

void MeshObject::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
		static float m_angle = 0;
		m_angle += EngineTime::getDeltaTime();
		constant cc;
		cc.m_angle = m_angle;

		Matrix4x4 temp;
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
		cc.m_proj.setPerspectiveFovLH(1.57, (float)width / (float)height, 0.1f, 100.0f);

		this->constantBuffer->update(GraphicsEngine::get()->getDeviceContext(), &cc);

		GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(vs, this->constantBuffer);
		GraphicsEngine::get()->getDeviceContext()->setConstantBuffer(ps, this->constantBuffer);

		GraphicsEngine::get()->getDeviceContext()->setVertexShader(vs);
		GraphicsEngine::get()->getDeviceContext()->setPixelShader(ps);

		if (!this->findComponentByType(Component::Material, name + " TX Component"))
		{
			GraphicsEngine::get()->getDeviceContext()->setTexture(ps, this->texture);
		}
		else
		{
			GraphicsEngine::get()->getDeviceContext()->setTexture(ps, ((TextureComponent*)(this->findComponentByType(Component::Material, name + " TX Component")))->getTexture());
		}

		GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->mesh->getVertexBuffer());
		GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->mesh->getIndexBuffer());

		GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

}

void MeshObject::release()
{

}
