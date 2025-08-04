#include "PhysicsSystem.h"
#include "GameObject.h"
#include "EngineTime.h"
#include <iostream>


PhysicsSystem::PhysicsSystem()
{
	this->physicsCommon = new PhysicsCommon();
	PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 50;
	settings.gravity = Vector3(0.0f, -9.81f, 0.0f);
	this->physicsWorld = this->physicsCommon->createPhysicsWorld(settings);
	std::cout << "Physics system initialized." << std::endl;
}

PhysicsSystem::~PhysicsSystem()
{

}

void PhysicsSystem::registerComponent(PhysicsComponent* component)
{
	if (component)
	{
		this->componentTable[component->getName()] = component;
		this->componentList.push_back(component);
	}
}

void PhysicsSystem::unregisterComponent(PhysicsComponent* component)
{
	bool x = false, y = false;
	if (component)
	{
		auto it = this->componentTable.find(component->getName());
		if (it != this->componentTable.end())
		{
			this->componentTable.erase(it);
			x = true;

		}
		auto list_it = std::find(this->componentList.begin(), this->componentList.end(), component);
		if (list_it != this->componentList.end())
		{
			this->componentList.erase(list_it);
			y = true;
		}

		if (component->getRigidBody() && x && y)
		{
			this->physicsWorld->destroyRigidBody(component->getRigidBody());
		}
	}
}

void PhysicsSystem::unregisterComponentByName(const String& name)
{
	auto it = this->componentTable.find(name);
	if (it != this->componentTable.end())
	{
		this->componentList.erase(std::remove(this->componentList.begin(), this->componentList.end(), it->second), this->componentList.end());
		this->componentTable.erase(it);
	}
}

void PhysicsSystem::unregisterAllComponents()
{
	for (int i = 0; i < this->componentList.size(); i++)
		this->physicsWorld->destroyRigidBody(this->componentList[i]->getRigidBody());

	this->componentList.clear();
	this->componentTable.clear();
}

PhysicsComponent* PhysicsSystem::findComponentByName(const String& name)
{
	auto it = this->componentTable.find(name);
	if (it != this->componentTable.end())
	{
		return it->second;
	}
	return nullptr;
}

PhysicsSystem::ComponentList PhysicsSystem::getAllComponents()
{
	return this->componentList;
}

void PhysicsSystem::updateAllComponents()
{
	const float timeStep = 1.0f / 60.0f;

	for (int i = 0; i < this->componentList.size(); i++)
	{
		if(this->componentList[i]->getOwner()->isActive())
		this->componentList[i]->perform(timeStep);
	}

	this->physicsWorld->update(timeStep);
}

PhysicsWorld* PhysicsSystem::getPhysicsWorld()
{
	return this->physicsWorld;
}

PhysicsCommon* PhysicsSystem::getPhysicsCommon()
{
	return this->physicsCommon;
}