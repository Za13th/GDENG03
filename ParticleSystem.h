#pragma once
#include <vector>
#include "Particle.h"
#include "VertexShader.h"
#include "PixelShader.h"

class ParticleSystem
{
	public:
		static ParticleSystem* getInstance();
		static void initialize();
		static void destroy();


		void Create(Particle templateParticle, void** shader_byte_code, size_t* size_shader);
		void Update(float deltaTime);
		void Draw(int width, int height, VertexShader* vs, PixelShader* ps);

		float interval = 0.01f;// interval between particle spawning
		int max_size = 1000; // Maximum number of particles

		Vector3D spawnAreaCenter = { 0.f,0.f,0.f };
		float spawnAreaWidth = 0.0f;
		float spawnAreaHeight = 0.0f;

		bool particleMovementRandom = false;
		bool particleMovementRandomUp = false;
		bool particleMovementRandomDown = false;
	private:
		ParticleSystem();
		~ParticleSystem();
		ParticleSystem(const ParticleSystem&) {};
		ParticleSystem& operator=(const ParticleSystem&) {};
		static ParticleSystem* sharedInstance;

		std::vector<Particle> particles; 

		Particle templateParticle;



};

