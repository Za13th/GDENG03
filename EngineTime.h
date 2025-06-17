#pragma once
#include <iostream>
#include <chrono>
#include <thread>   
#include <ctime>

class Window;

class EngineTime
{
public:
	static void initialize();
	static double getDeltaTime();

private:
	EngineTime();
	~EngineTime();
	EngineTime(const EngineTime&) = delete;
	EngineTime& operator=(const EngineTime&) = delete;

	static EngineTime* sharedInstance;

	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

	double deltaTime = 0.0;

	static void LogFrameStart();
	static void LogFrameEnd();

	friend class Window;


};

