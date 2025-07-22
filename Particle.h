#pragma once
#include <Windows.h>
#include "GameObject.h"
#include "structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"


class Particle : public GameObject
{
public:
	Particle();
	~Particle();


	void Create(void** shader_byte_code, size_t* size_shader);
	void setColor(Vector3D color);
	void setColor(Vector3D color, int index);

	void setColor1(Vector3D color);
	void setColor1(Vector3D color, int index);
	void setColorsToEachOther();

	void update(float deltaTime);
	void draw(int width, int height, VertexShader* vs, PixelShader* ps);
	void Release();

	bool checkActive(); //if false, destroy the particle

	float speed = 1.57f; //default is 1.57f for PardCode
	float lifeTimeLimit = 20.0f; 

	Vector3D particleVelocity = Vector3D(0.0f, -0.001f, 0.0f); // Velocity of the particle
	Vector3D particleAcceleration = Vector3D(0, -0.001f, 0); // Acceleration of the particle 

	bool constantVelocity = false;
private:


	VertexBuffer* m_vbuffer = nullptr;
	IndexBuffer* m_ibuffer = nullptr;
	ConstantBuffer* m_cbuffer = nullptr;
	vertex vertexes[32];
	UINT size_vertexes;
	float deltaPos = 0.0f;
	float m_angle = 0.0f;

	float lifeTime = 0.0f; // Lifetime of the particle

};

