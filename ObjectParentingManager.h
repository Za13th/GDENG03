#pragma once
#include "GameObject.h"
class ObjectParentingManager
{
public:
	static ObjectParentingManager* getInstance();
	static void initialize();
	static void destroy();

	bool isParenting() { return this->parentingMode; }
	void setParenting(bool b) { this->parentingMode = b; }
	GameObject* getObject() { return this->selectedObject; }
	void setObject(GameObject* g) { this->selectedObject = g; }

private:
	ObjectParentingManager();
	~ObjectParentingManager();
	ObjectParentingManager(const ObjectParentingManager&) {}
	ObjectParentingManager& operator=(const ObjectParentingManager&) {}
	static ObjectParentingManager* sharedInstance;

	GameObject* selectedObject = nullptr;
	bool parentingMode = false;


};

