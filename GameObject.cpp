#include "GameObject.h"

GameObject::GameObject(std::string name) : name(name), localPosition(0.0f, 0.0f, 0.0f), localScale(1.0f, 1.0f, 1.0f), localRotation(0.0f, 0.0f, 0.0f)
{

}

GameObject::~GameObject()
{

}

void GameObject::setPosition(float x, float y, float z)
{
	localPosition = Vector3D(x, y, z);
}

void GameObject::setPosition(Vector3D position)
{
	localPosition = position;
}

Vector3D GameObject::getLocalPosition()
{
	return localPosition;
}

void GameObject::setScale(float x, float y, float z)
{
	localScale = Vector3D(x, y, z);
}

void GameObject::setScale(Vector3D scale)
{
	localScale = scale;
}

Vector3D GameObject::getLocalScale()
{
	return localScale;
}

void GameObject::setRotation(float x, float y, float z)
{
	localRotation = Vector3D(x, y, z);
}

void GameObject::setRotation(Vector3D rotation)
{
	localRotation = rotation;
}

Vector3D GameObject::getLocalRotation()
{
	return localRotation;
}


