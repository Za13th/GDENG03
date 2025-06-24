#include "FogSystem.h"
#include <iostream>
#include "InputSystem.h"
#include <algorithm>


FogSystem* FogSystem::sharedInstance = nullptr;

void FogSystem::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new FogSystem();
	}
}

FogSystem* FogSystem::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new FogSystem();
	}
	return sharedInstance;
}

void FogSystem::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void FogSystem::update()
{
	// Fog start distance controls
	if (InputSystem::get()->isKeyDown('Z'))
	{
		fog_start = std::max(0.0f, fog_start - 0.1f);
	}
	if (InputSystem::get()->isKeyDown('C'))
	{
		fog_start += 0.1f;
		fog_start = std::min(fog_start, fog_end - 0.1f); // Ensure fog start is less than fog end)
	}

	// Fog end distance controls
	if (InputSystem::get()->isKeyDown('V'))
	{
		fog_end = std::max(fog_start + 0.1f, fog_end - 0.1f);
	}
	if (InputSystem::get()->isKeyDown('B'))
	{
		fog_end += 0.1f;
	}

	// Fog density controls
	if (InputSystem::get()->isKeyDown('N'))
	{
		fog_density = std::max(0.001f, fog_density - 0.001f);
	}
	if (InputSystem::get()->isKeyDown('M'))
	{
		fog_density += 0.001f;
	}
}

FogSystem::FogSystem()
{
	// Initialize default values
	fog_color = { 0.6f, 0.6f, 0.6f };
	fog_start = 2.0f;
	fog_end = 10.0f;
	fog_density = 0.1f;
	fog_state = 1; // Default state
	culling_enabled = false; // Default culling state
}

FogSystem::~FogSystem()
{
	
}

void FogSystem::toggleCullingEnabled()
{
	if (culling_enabled == 1)
		culling_enabled = 0;
	else
		culling_enabled = 1;
}



