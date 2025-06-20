#include "SceneCameraHolder.h"

SceneCameraHolder* SceneCameraHolder::sharedInstance = nullptr;

SceneCameraHolder* SceneCameraHolder::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new SceneCameraHolder();
		sharedInstance->camera = new Camera("SceneCamera");
	}
	return sharedInstance;
}

void SceneCameraHolder::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new SceneCameraHolder();
		sharedInstance->camera = new Camera("SceneCamera");
	}
}

void SceneCameraHolder::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance->camera;
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

Camera* SceneCameraHolder::getCamera()
{
	return this->camera;
}


