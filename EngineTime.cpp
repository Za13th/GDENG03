#include "EngineTime.h"

EngineTime* EngineTime::sharedInstance = nullptr;

void EngineTime::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new EngineTime();
	}
}

double EngineTime::getDeltaTime()
{
	return sharedInstance->deltaTime;
}

EngineTime::EngineTime()
{

}

EngineTime::~EngineTime()
{

}

void EngineTime::LogFrameStart()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void EngineTime::LogFrameEnd()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = sharedInstance->end - sharedInstance->start;

	sharedInstance->deltaTime = elapsed_seconds.count();
	//std::cout << "Frame time: " << sharedInstance->deltaTime * 1000.0 << " ms" << std::endl;
}