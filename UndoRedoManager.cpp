#include "UndoRedoManager.h"
#include "GameObjectManager.h"
#include "PhysicsComponent.h"
#include "BaseComponentSystem.h"
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

	if (current + 1 < history.size()) {
		for (int i = history.size() - 1; i > current; i--) {
			history.pop_back();
		}
		history.push_back(action);

	}
	else if (history.size() > 5) {
		history.push_back(action);
		history.erase(history.begin());
	}
	else {

		history.push_back(action);
	}

	current = history.size() - 1;
}

void UndoRedoManager::undo()
{
	if (history.size() > 0) {
		//std::cout << "current: " << current << "\nsize: " << history.size() << std::endl;
		if (history[current]->getType() == UndoRedoAction::Transforms) { //is transform
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
		else if (history[current]->getType() == UndoRedoAction::Spawn) { // object spawn
			PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(history[current]->getObj()->findComponentByType(Component::Physics, history[current]->getObj()->name + " P6 Component"));
			if (physicsComponent)
			{
				history[current]->hasPhys = true;
				BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(physicsComponent);

			}
			if (history[current]->getObj()->findComponentByType(Component::Material, history[current]->getObj()->name + " TX Component"))
			{
				history[current]->hasTex = true;
				history[current]->texCom = static_cast<TextureComponent*>(history[current]->getObj()->findComponentByType(Component::Material, history[current]->getObj()->name + " TX Component"));
			}

			GameObjectManager::getInstance()->removeGameObject(history[current]->getObj());
		}

		current--;
	}
}

void UndoRedoManager::redo()
{
	if (current + 1 < history.size()) {
		current++;
		//std::cout << "current: " << current << "\nsize: " << history.size() << std::endl;
		if (history[current]->getType() == UndoRedoAction::Transforms) { //is transform
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
		else if (history[current]->getType() == UndoRedoAction::Spawn) { // object spawn
			GameObjectManager::getInstance()->addGameObject(history[current]->getObj());
			history[current]->getObj()->reconstructMatrix();

			if (history[current]->hasPhys)
				history[current]->getObj()->attachComponent(new PhysicsComponent(history[current]->getObj()->name + " P6 Component", history[current]->getObj()));
			if (history[current]->hasTex)
			{
				history[current]->getObj()->attachComponent(history[current]->texCom);
			}
		}
	}

}

UndoRedoManager::UndoRedoManager()
{
}

UndoRedoManager::~UndoRedoManager()
{
}
