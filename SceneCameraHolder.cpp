#include "SceneCameraHolder.h"
#include "GameStateManager.h"
#include "InputSystem.h"

SceneCameraHolder* SceneCameraHolder::sharedInstance = nullptr;

SceneCameraHolder* SceneCameraHolder::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new SceneCameraHolder();
		sharedInstance->cameraList[0] = new Camera("SceneCamera");
		sharedInstance->cameraList[1] = new Camera("GameCamera");
		sharedInstance->cameraList[2] = new Camera("GameCamera");
		sharedInstance->currentCam = sharedInstance->sharedInstance->cameraList[0];
	}
	return sharedInstance;
}

void SceneCameraHolder::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new SceneCameraHolder();
		sharedInstance->cameraList[0] = new Camera("SceneCamera");
		sharedInstance->cameraList[1] = new Camera("GameCamera");
		sharedInstance->cameraList[2] = new Camera("GameCamera");
		sharedInstance->currentCam = sharedInstance->sharedInstance->cameraList[0];
	}
}

void SceneCameraHolder::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance->cameraList[0];
		delete sharedInstance->cameraList[1];
		delete sharedInstance->cameraList[2];
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void SceneCameraHolder::updateCamera()
{
	static bool first = true;
	if (GameStateManager::getInstance()->getGameState() == GameStateManager::Edit)
	{
		this->currentCam = this->cameraList[0];
		first = true;
	}
	else if (first)
	{	
		*(this->cameraList[1]) = *(this->cameraList[2]);

		this->cameraList[1]->setCameraRotation(false);

		this->cameraList[1]->width = this->cameraList[0]->width;
		this->cameraList[1]->height = this->cameraList[0]->height;

		this->currentCam = this->cameraList[1];

		first = false;
	}
}

Camera* SceneCameraHolder::getCamera()
{
	return this->currentCam;
}


