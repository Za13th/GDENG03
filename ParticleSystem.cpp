#include "ParticleSystem.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
ParticleSystem* ParticleSystem::sharedInstance = nullptr;

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

ParticleSystem* ParticleSystem::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new ParticleSystem();
	}
	return sharedInstance;
}

void ParticleSystem::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new ParticleSystem();
	}
}

void ParticleSystem::destroy()
{
	if (sharedInstance != nullptr)
	{

		sharedInstance->templateParticle.Release();

	}
}


void ParticleSystem::Create(Particle templateParticle, void** shader_byte_code, size_t* size_shader)
{
	this->templateParticle = templateParticle;

	this->templateParticle.Create(shader_byte_code, size_shader);

	this->templateParticle.setScale(Vector3D(0.04f, 0.04f, 1.f));

	srand(time(0));
}

void ParticleSystem::Update(float deltaTime)
{
	static float timePassed = this->interval + 1;
	timePassed += deltaTime;

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].update(deltaTime);
		if (!particles[i].checkActive())
		{
			particles.erase(particles.begin() + i);
			i--; // Adjust index after erasing
			//std::cout << "Particle was Deleted\n";
		}
	}

	if (sharedInstance->max_size > sharedInstance->particles.size() && timePassed >= interval)
	{
		auto newParticle = templateParticle; // Copy the template particle


			if (sharedInstance->spawnAreaWidth == 0 && sharedInstance->spawnAreaHeight == 0)
				newParticle.setPosition(Vector3D(sharedInstance->spawnAreaCenter.x,
					sharedInstance->spawnAreaCenter.y,
					-2.0f));
			else if(sharedInstance->spawnAreaWidth == 0)
				newParticle.setPosition(Vector3D(sharedInstance->spawnAreaCenter.x,
					sharedInstance->spawnAreaCenter.y + (rand() % (int)(100 * sharedInstance->spawnAreaHeight) - (int)(50 * sharedInstance->spawnAreaHeight)) * 0.01f,
					-2.0f));
			else if (sharedInstance->spawnAreaHeight == 0)
				newParticle.setPosition(Vector3D(sharedInstance->spawnAreaCenter.x + (rand() % (int)(100 * sharedInstance->spawnAreaWidth) - (int)(50 * sharedInstance->spawnAreaWidth)) * 0.01f,
					sharedInstance->spawnAreaCenter.y,
					-2.0f));
			else
			newParticle.setPosition(Vector3D(sharedInstance->spawnAreaCenter.x + (rand() % (int)(100 * sharedInstance->spawnAreaWidth) - (int)(50 * sharedInstance->spawnAreaWidth)) * 0.01f,
				sharedInstance->spawnAreaCenter.y + (rand() % (int)(100 * sharedInstance->spawnAreaHeight) - (int)(50 * sharedInstance->spawnAreaHeight)) * 0.01f,
				-2.0f));

			if (particleMovementRandom)
			{
				newParticle.particleVelocity = Vector3D(((rand() % 11) - 5) * 0.001, ((rand() % 11) - 5) * 0.001, 0);
				newParticle.particleAcceleration = Vector3D(((rand() % 7) - 3) * 0.001, ((rand() % 7) - 3) * 0.001, 0);

				//std::cout << newParticle.particleVelocity.x << " " << newParticle.particleVelocity.y << "\n";
				//std::cout << newParticle.particleAcceleration.x << " " << newParticle.particleAcceleration.y << "\n";
			}
			else if (particleMovementRandomUp)
			{
				newParticle.particleVelocity = Vector3D(((rand() % 7) - 3) * 0.001, ((rand() % 6) + 5) * 0.001, 0);
				newParticle.particleAcceleration = Vector3D(0.0f, -0.002f, 0);

				//std::cout << newParticle.particleVelocity.x << " " << newParticle.particleVelocity.y << "\n";
				//std::cout << newParticle.particleAcceleration.x << " " << newParticle.particleAcceleration.y << "\n";

			}
			else if (particleMovementRandomDown)
			{
				newParticle.particleVelocity = Vector3D(((rand() % 7) - 3) * 0.001, ((rand() % 6) - 5) * 0.001, 0);
				newParticle.particleAcceleration = Vector3D(0.0f, -0.001f , 0);

				//std::cout << newParticle.particleVelocity.x << " " << newParticle.particleVelocity.y << "\n";
				//std::cout << newParticle.particleAcceleration.x << " " << newParticle.particleAcceleration.y << "\n";

			}

		particles.push_back(newParticle);
		timePassed = 0.0f; // Reset the timer
		//std::cout << "Particle was Created\n";
	}
	
}

void ParticleSystem::Draw(int width, int height, VertexShader* vs, PixelShader* ps)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].draw(width, height, vs, ps);
	}
}