#include "Component.h"

Component::Component(String name, ComponentType type, GameObject* owner): name(name), type(type), owner(owner) 
{
	
}

Component::~Component()
{

}

void Component::attachOwner(GameObject* owner)
{
	this->owner = owner;
}

void Component::detachOwner()
{
	this->owner = nullptr;
}

GameObject* Component::getOwner()
{
	return this->owner;
}

Component::ComponentType Component::getType()
{
	return this->type;
}

Component::String Component::getName()
{
	return this->name;
}