#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Texture.h"
class MeshObject :public GameObject
{
	public:
		MeshObject(std::string name, Mesh* mesh, Texture* texture);
		~MeshObject();

		void update(float deltaTime) override;
		void draw(int width, int height, VertexShader* vs, PixelShader* ps) override;
		void release();

	private:
		Mesh* mesh = nullptr;
		Texture* texture = nullptr;


		ConstantBuffer* constantBuffer = nullptr;

		float ticks = 0.0f;
		float deltaPos = 0.0f;
		float deltaScale = 0.0f;
		float deltaTime = 0.0f;
		float speed = 1.57f; //default is 1.57f for PardCode

};

