#pragma once
#include <vector>
#include "UndoRedoAction.h"

class UndoRedoManager
{
public:
	static UndoRedoManager* getInstance();
	static void initialize();
	static void destroy();

	void addToHistory(UndoRedoAction* action);

	void undo();
	void redo();

private:
	UndoRedoManager();
	~UndoRedoManager();
	UndoRedoManager(const UndoRedoManager&) {}
	UndoRedoManager& operator=(const UndoRedoManager&) {}
	static UndoRedoManager* sharedInstance;

	std::vector<UndoRedoAction*> history;
	int current = -1;

	
};

