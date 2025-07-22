#pragma once
#include "Camera.h"
class SceneCameraHolder
{
	public:
		static SceneCameraHolder* getInstance();
		static void initialize();
		static void destroy();
		
		Camera* getCamera();

	private:
		SceneCameraHolder() = default;
		~SceneCameraHolder() = default;
		SceneCameraHolder(const SceneCameraHolder&) = delete;
		SceneCameraHolder& operator=(const SceneCameraHolder&) = delete;

		static SceneCameraHolder* sharedInstance;
		Camera* camera = nullptr;
		
};

