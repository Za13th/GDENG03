#pragma once
#include "GameObject.h"
#include "MeshManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Vector3D.h"
#include "TextureManager.h"


class Cylinder : public GameObject
{
public:
	Cylinder(std::string name, void* shaderByteCode, size_t sizeShader);
	~Cylinder();

	void update(float deltaTime) override;
	void draw(int width, int height, VertexShader* vs, PixelShader* ps) override;

	void release();

private:
	Mesh* cylinderMesh = nullptr;
	Texture* texture = nullptr;
	ConstantBuffer* constantBuffer;
	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaScale = 0.0f;
	float deltaTime = 0.0f;
	float speed = 1.57f; //default is 1.57f for PardCode
};

