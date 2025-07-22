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

		if (texture)
		{
			GraphicsEngine::get()->getDeviceContext()->setTexture(ps, this->texture);
		}

		GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->mesh->getVertexBuffer());
		GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->mesh->getIndexBuffer());

		GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

}

void MeshObject::release()
{

}
