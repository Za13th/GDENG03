#include "ObjectParentingManager.h"

ObjectParentingManager* ObjectParentingManager::sharedInstance = nullptr;

ObjectParentingManager* ObjectParentingManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new ObjectParentingManager();
	}
	return sharedInstance;
}

void ObjectParentingManager::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new ObjectParentingManager();
	}
}

void ObjectParentingManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

ObjectParentingManager::ObjectParentingManager()
{

}
ObjectParentingManager::~ObjectParentingManager()
{

}

