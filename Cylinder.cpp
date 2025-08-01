#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Cylinder.h"
#include "structs.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "EngineTime.h"
#include "SwapChain.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "SceneCameraHolder.h"
#include "FogSystem.h"

__declspec(align(16))
struct constant
{
    Matrix4x4 m_world;
    Matrix4x4 m_view;
    Matrix4x4 m_proj;
    float m_angle;
};

Cylinder::Cylinder(std::string name, void* shaderByteCode, size_t sizeShader) : GameObject(name)
{
    this->objectType = GameObject::Cylinder;
    this->cylinderMesh = MeshManager::getInstance()->createMeshFromFile(L"Assets\\Meshes\\cylinder.obj");
	this->texture = TextureManager::getInstance()->createTextureFromFile(L"Assets\\Textures\\default.jpg");

    constant cc;
    cc.m_angle = 0;
    this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();

    this->constantBuffer->load(&cc, sizeof(constant));
}

Cylinder::~Cylinder()
{

}

void Cylinder::update(float deltaTime)
{
	m_angle += deltaTime;

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

void Cylinder::draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	constant cc;
	cc.m_angle = m_angle;

	Matrix4x4 temp;

	Vector3D cylinderScale = this->localScale;
	cylinderScale.x *= 0.5f; 
	cylinderScale.y *= 2 / (double)2.850626;
	cylinderScale.z *= 0.5f;

	cc.m_world.setScale(cylinderScale); //to make spheres and cubes 1 to 1 in size

	temp.setRotationZ(this->getLocalRotation().z);
	cc.m_world *= temp;
	temp.setRotationY(this->getLocalRotation().y);
	cc.m_world *= temp;
	temp.setRotationX(this->getLocalRotation().x);
	cc.m_world *= temp;

	temp.setTranslation(this->getLocalPosition());
	cc.m_world *= temp;

	cc.m_world.setScale(cylinderScale);
	cc.m_world *= this->localMatrix;


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

	GraphicsEngine::get()->getDeviceContext()->setVertexBuffer(this->cylinderMesh->getVertexBuffer());
	GraphicsEngine::get()->getDeviceContext()->setIndexBuffer(this->cylinderMesh->getIndexBuffer());

	GraphicsEngine::get()->getDeviceContext()->drawIndexedTriangleList(this->cylinderMesh->getIndexBuffer()->getSizeIndexList(), 0, 0);


}

void Cylinder::release()
{

}