#pragma once
#include "PhysicsSystem.h"
class BaseComponentSystem
{
public:
	typedef std::string String;

	static BaseComponentSystem* getInstance();
	static void initialize();
	static void destroy();

	PhysicsSystem* getPhysicsSystem();

private:
	BaseComponentSystem();
	~BaseComponentSystem();
	BaseComponentSystem(const BaseComponentSystem&) {}
	BaseComponentSystem& operator=(const BaseComponentSystem&) {}

	static BaseComponentSystem* sharedInstance;
	PhysicsSystem* m_physics_system;

};

