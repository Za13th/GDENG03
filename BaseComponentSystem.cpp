#include "BaseComponentSystem.h"

BaseComponentSystem* BaseComponentSystem::sharedInstance = nullptr;

BaseComponentSystem* BaseComponentSystem::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new BaseComponentSystem();
	}
	return sharedInstance;
}

void BaseComponentSystem::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new BaseComponentSystem();
	}
}

void BaseComponentSystem::destroy()
{
	if (sharedInstance)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

BaseComponentSystem::BaseComponentSystem()
{

}

BaseComponentSystem::~BaseComponentSystem()
{

}

PhysicsSystem* BaseComponentSystem::getPhysicsSystem()
{
	if (!m_physics_system)
	{
		m_physics_system = new PhysicsSystem();
	}
	return m_physics_system;
}