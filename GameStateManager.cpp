#include "GameStateManager.h"

GameStateManager* GameStateManager::sharedInstance = nullptr;

GameStateManager* GameStateManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new GameStateManager();
	}
	return sharedInstance;
}

void GameStateManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

GameStateManager::GameStateManager()
{

}

GameStateManager::~GameStateManager()
{

}
