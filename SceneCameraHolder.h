#pragma once
#include "Camera.h"
class SceneCameraHolder
{
	public:
		static SceneCameraHolder* getInstance();
		static void initialize();
		static void destroy();
		
		void updateCamera();
		Camera* getCamera();

	private:
		SceneCameraHolder() = default;
		~SceneCameraHolder() = default;
		SceneCameraHolder(const SceneCameraHolder&) = delete;
		SceneCameraHolder& operator=(const SceneCameraHolder&) = delete;

		static SceneCameraHolder* sharedInstance;
		Camera* currentCam = nullptr;

		Camera* cameraList[3] = { nullptr, nullptr, nullptr };
		
};

