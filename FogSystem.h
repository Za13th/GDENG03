#pragma once
#include "Vector3D.h"


class FogSystem
{
public:
	static void initialize();
	static FogSystem* getInstance();
	static void destroy();

	void update();

	Vector3D getFogColor() { return fog_color; }
	void setFogColor(Vector3D color) { fog_color = color; }

	float getFogStart() { return fog_start; }
	void setFogStart(float start) { fog_start = start; }

	float getFogEnd() { return fog_end; }
	void setFogEnd(float end) { fog_end = end; }

	float getFogDensity() { return fog_density; }
	void setFogDensity(float density) { fog_density = density; }

	int getFogState() { return fog_state; }
	void setFogState(int state) { fog_state = state; }


	int isCullingEnabled() { return culling_enabled; }
	void toggleCullingEnabled();



private:
	FogSystem();
	~FogSystem();
	FogSystem(const FogSystem&) = delete;
	FogSystem& operator=(const FogSystem&) = delete;

	static FogSystem* sharedInstance;

	Vector3D fog_color = { 0.6f,0.6f,0.6f };
	float fog_start = 2.f;
	float fog_end = 10.0f;
	float fog_density = 0.1f;
	int fog_state = 0;
	int culling_enabled = 0;

};	

