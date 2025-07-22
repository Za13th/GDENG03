#pragma once
#include "Component.h"
#include <reactphysics3d//reactphysics3d.h>
using namespace reactphysics3d;

class PhysicsComponent : public Component
{
	public:
		PhysicsComponent(String name, GameObject* owner);
		~PhysicsComponent();
		void perform(float deltaTime) override;
		RigidBody* getRigidBody();
		
		void disableGravity()
		{
			if (this->rigidBody)
			{
				this->rigidBody->enableGravity(false);
				this->rigidBody->setType(BodyType::STATIC);
			}
		}

	private:
		float mass = 1000; //in kilograms
		RigidBody* rigidBody = nullptr;
};

