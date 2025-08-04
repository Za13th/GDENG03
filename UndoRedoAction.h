#pragma once
#include "GameObject.h"
#include "Vector3D.h"
#include <unordered_map>
#include <vector>

class UndoRedoAction
{
public:
	enum actType{Transforms = 0, Spawn = 1, Delete = 2};

	actType getType();
	GameObject* getObj();

	Vector3D getOldPos();
	Vector3D getNewPos();

	Vector3D getOldRot();
	Vector3D getNewRot();

	Vector3D getOldSca();
	Vector3D getNewSca();

	UndoRedoAction();
	UndoRedoAction(actType typ, GameObject* obj, Vector3D oldPos, Vector3D newPos, Vector3D oldRot, Vector3D newRot, Vector3D oldSca, Vector3D newSca);
	UndoRedoAction(actType typ, GameObject* obj);
	~UndoRedoAction();

	bool hasPhys = false;
	bool hasTex = false;
	TextureComponent* texCom;
private:
	actType type;
	GameObject* cloneObj;

	Vector3D oldPos;
	Vector3D newPos;

	Vector3D oldRot;
	Vector3D newRot;

	Vector3D oldSca;
	Vector3D newSca;

	
};

