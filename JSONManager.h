#pragma once
#include "Libs/nlohmannjson/json.hpp"
using json = nlohmann::json;

class JSONManager
{
public:
	static JSONManager* getInstance();
	static void initialize(void* shaderByteCode, size_t sizeShader);
	static void destroy();

	void save(bool bg);
	void load(bool bg);

private:
	JSONManager();
	~JSONManager();
	JSONManager(const JSONManager&) {}
	JSONManager& operator=(const JSONManager&) {}
	static JSONManager* sharedInstance;
};

