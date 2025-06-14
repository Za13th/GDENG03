#pragma once
#include <iostream>
#include "Vector3D.h"

class VertexShader;
class PixelShader;

class GameObject
{
	public: 
		GameObject(std::string name);
		~GameObject();

		virtual void update(float deltaTime) = 0;
		virtual void draw(int width, int height, VertexShader* vs, PixelShader* ps) = 0;

		void setPosition(float x, float y, float z);
		void setPosition(Vector3D position);
		Vector3D getLocalPosition();

		void setScale(float x, float y, float z);
		void setScale(Vector3D scale);
		Vector3D getLocalScale();

		void setRotation(float x, float y, float z);
		void setRotation(Vector3D rotation);
		Vector3D getLocalRotation();

		std::string name;
	private:
		Vector3D localPosition;
		Vector3D localScale;
		Vector3D localRotation;


};

