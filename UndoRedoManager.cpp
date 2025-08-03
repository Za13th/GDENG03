#include "UndoRedoManager.h"
#include "GameObjectManager.h"
#include "PhysicsComponent.h"
#include <iostream>

UndoRedoManager* UndoRedoManager::sharedInstance = nullptr;

UndoRedoManager* UndoRedoManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new UndoRedoManager();
	}
	return sharedInstance;
}

void UndoRedoManager::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new UndoRedoManager();
	}
}

void UndoRedoManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void UndoRedoManager::addToHistory(UndoRedoAction* action)
{
	
	if (current + 1 < history.size()) {std::cout << "added to history" << std::endl;
		for (int i = history.size() - 1; i > current; i--) {
			history.pop_back();
		}
		history.push_back(action);

	} else if (history.size()>5) {
		history.push_back(action);
		history.erase(history.begin());
	}
	else {
		std::cout << "added to history3" << std::endl;
		history.push_back(action);
	}
	
	current = history.size() - 1;
}

void UndoRedoManager::undo()
{
	if (history.size() > 0) {
		//std::cout << "current: " << current << "\nsize: " << history.size() << std::endl;
		if (history[current]->getType()) { //is transform
			history[current]->getObj()->setPosition(history[current]->getOldPos());
			history[current]->getObj()->setRotation(history[current]->getOldRot());
			history[current]->getObj()->setScale(history[current]->getOldSca());

			history[current]->getObj()->reconstructMatrix();

			PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(history[current]->getObj()->findComponentByType(Component::Physics, history[current]->getObj()->name + " P6 Component"));
			if (physicsComponent)
			{
				physicsComponent->adjustRigidbody();
			}
		}
		else { // object spawn
			GameObjectManager::getInstance()->removeGameObject(history[current]->getObj());
		}

		current--;
	}
}

void UndoRedoManager::redo()
{
	if (current+1 < history.size()) {
		current++;
		//std::cout << "current: " << current << "\nsize: " << history.size() << std::endl;
		if (history[current]->getType()) { //is transform
			history[current]->getObj()->setPosition(history[current]->getNewPos());
			history[current]->getObj()->setRotation(history[current]->getNewRot());
			history[current]->getObj()->setScale(history[current]->getNewSca());

			history[current]->getObj()->reconstructMatrix();

			PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(history[current]->getObj()->findComponentByType(Component::Physics, history[current]->getObj()->name + " P6 Component"));
			if (physicsComponent)
			{
				physicsComponent->adjustRigidbody();
			}
		}
		else { // object spawn
			GameObjectManager::getInstance()->removeGameObject(history[current]->getObj());
		}
	}

}

UndoRedoManager::UndoRedoManager()
{
}

UndoRedoManager::~UndoRedoManager()
{
}
