#pragma once
#include <Windows.h>
#include "GameObject.h"
#include "structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class VertexBuffer;

class Quad : public GameObject
{
	public:
		Quad();
		

		void Create(void** shader_byte_code, size_t* size_shader);
		void setColor(Vector3D color);
		void setColor(Vector3D color1, Vector3D color2, Vector3D color3, Vector3D color4);

		void setColor1(Vector3D color);
		void setColor1(Vector3D color1, Vector3D color2, Vector3D color3, Vector3D color4);
		void update(float deltaTime);
		void draw(int width, int height, VertexShader* vs, PixelShader* ps);
		void Release();

		float speed = 1.57f; //default is 1.57f for PardCode
	private:
		VertexBuffer* m_vbuffer = nullptr;
		IndexBuffer* m_ibuffer = nullptr;
		ConstantBuffer* m_cbuffer = nullptr;
		vertex vertexes[4];
		UINT size_vertexes;
		float deltaPos = 0.0f;
		float m_angle = 0.0f;







};

