#include "UndoRedoAction.h"

UndoRedoAction::actType UndoRedoAction::getType()
{
    return type;
}

GameObject* UndoRedoAction::getObj()
{
    return cloneObj;
}

Vector3D UndoRedoAction::getOldPos()
{
    return oldPos;
}

Vector3D UndoRedoAction::getNewPos()
{
    return newPos;
}

Vector3D UndoRedoAction::getOldRot()
{
    return oldRot;
}

Vector3D UndoRedoAction::getNewRot()
{
    return newRot;
}

Vector3D UndoRedoAction::getOldSca()
{
    return oldSca;
}

Vector3D UndoRedoAction::getNewSca()
{
    return newSca;
}

UndoRedoAction::UndoRedoAction()
{
}

UndoRedoAction::UndoRedoAction(actType typ, GameObject* obj, Vector3D oldPos, Vector3D newPos, Vector3D oldRot, Vector3D newRot, Vector3D oldSca, Vector3D newSca)
{
    this->type = typ;
    this->cloneObj = obj;

    this->oldPos = oldPos;
    this->newPos = newPos; 
    
    this->oldRot = oldRot;
    this->newRot = newRot;
    
    this->oldSca = oldSca;
    this->newSca = newSca;
}

UndoRedoAction::UndoRedoAction(actType typ, GameObject* obj)
{
    this->type = typ;
    this->cloneObj = obj;
}

UndoRedoAction::~UndoRedoAction()
{
}
