#pragma once
#include "GameObject.h"
#include "Vector3D.h"

class UndoRedoAction
{
public:
	bool getType();
	GameObject* getObj();

	Vector3D getOldPos();
	Vector3D getNewPos();

	Vector3D getOldRot();
	Vector3D getNewRot();

	Vector3D getOldSca();
	Vector3D getNewSca();

	UndoRedoAction();
	UndoRedoAction(bool isT, GameObject* obj, Vector3D oldPos, Vector3D newPos, Vector3D oldRot, Vector3D newRot, Vector3D oldSca, Vector3D newSca);
	UndoRedoAction(bool isT, GameObject* obj);
	~UndoRedoAction();
private:
	bool isTransform;
	GameObject* cloneObj;

	Vector3D oldPos;
	Vector3D newPos;

	Vector3D oldRot;
	Vector3D newRot;

	Vector3D oldSca;
	Vector3D newSca;
};

