#pragma once
#include <string>
#include <vector>

class DebugUIManager
{
public:
	static DebugUIManager* getInstance();
	static void initialize(void* shaderByteCode, size_t sizeShader);
	static void destroy();

	void draw();
	void Log(std::string msg);

private:
	DebugUIManager();
	~DebugUIManager();
	DebugUIManager(const DebugUIManager&) {}
	DebugUIManager& operator=(const DebugUIManager&) {}
	static DebugUIManager* sharedInstance;

	std::vector<std::string> debugLines;
};

