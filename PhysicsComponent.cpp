#include "PhysicsComponent.h"
#include "BaseComponentSystem.h"
#include "GameObject.h"
#include "Cube.h"
#include "MyQuaternion.h"

PhysicsComponent::PhysicsComponent(String name, GameObject* owner) : Component(name, ComponentType::Physics, owner)
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
	PhysicsCommon* physicsCommon = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	Vector3D scale = this->owner->getLocalScale();
	Vector3D pos = this->owner->getLocalPosition();
	Vector3D rot = this->owner->getLocalRotation();

	Transform transform;
	transform.setToIdentity();
	transform.setOrientation(Quaternion::fromEulerAngles(rot.x, rot.y, rot.z));
	transform.setPosition(Vector3(pos.x, pos.y, pos.z));
	//transform.setFromOpenGL(this->owner->getPhysicsLocalMatrix());
	this->rigidBody = physicsWorld->createRigidBody(transform);

	if (owner->objectType == GameObject::Cube)
	{
		BoxShape* boxshape = physicsCommon->createBoxShape(Vector3(scale.x / 2, scale.y / 2, scale.z / 2));
		transform.setToIdentity();
		this->rigidBody->addCollider(boxshape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Plane)
	{
		BoxShape* boxshape = physicsCommon->createBoxShape(Vector3(scale.x * 5, 0.05, scale.z * 5));
		transform.setToIdentity();
		this->rigidBody->addCollider(boxshape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::STATIC);
	}
	else if (owner->objectType == GameObject::Sphere)
	{
		SphereShape* sphereShape = physicsCommon->createSphereShape(scale.y / 2);
		transform.setToIdentity();
		this->rigidBody->addCollider(sphereShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Cylinder)
	{
		CapsuleShape* capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2));
		if (((scale.y * 2) - (scale.x)) > 0)
			capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2) - (scale.x));

		transform.setToIdentity();
		this->rigidBody->addCollider(capsuleShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Capsule)
	{
		CapsuleShape* capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2));
		if (((scale.y * 2) - (scale.x)) > 0)
			capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2) - (scale.x));

		transform.setToIdentity();
		this->rigidBody->addCollider(capsuleShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	transform = this->rigidBody->getTransform();
	float matrix[16];

	transform.getOpenGLMatrix(matrix);

	this->owner->setLocalMatrix(matrix);
}

void PhysicsComponent::adjustRigidbody()
{
	this->rigidBody->removeCollider(this->rigidBody->getCollider(0)); // Remove the previous collider

	PhysicsCommon* physicsCommon = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	Vector3D scale = this->owner->getLocalScale();
	Vector3D pos = this->owner->getLocalPosition();
	Vector3D rot = this->owner->getLocalRotation();

	Transform transform;
	transform.setToIdentity();
	transform.setOrientation(Quaternion::fromEulerAngles(rot.x, rot.y, rot.z));
	transform.setPosition(Vector3(pos.x, pos.y, pos.z));
	this->rigidBody = physicsWorld->createRigidBody(transform);

	if (owner->objectType == GameObject::Cube)
	{
		BoxShape* boxshape = physicsCommon->createBoxShape(Vector3(scale.x / 2, scale.y / 2, scale.z / 2));
		transform.setToIdentity();
		this->rigidBody->addCollider(boxshape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Plane)
	{
		BoxShape* boxshape = physicsCommon->createBoxShape(Vector3(scale.x * 5, 0.05, scale.z * 5));
		transform.setToIdentity();
		this->rigidBody->addCollider(boxshape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::STATIC);
	}
	else if (owner->objectType == GameObject::Sphere)
	{
		SphereShape* sphereShape = physicsCommon->createSphereShape(scale.y / 2);
		transform.setToIdentity();
		this->rigidBody->addCollider(sphereShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Cylinder)
	{
		CapsuleShape* capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, 0.01f);
		if (((scale.y * 2) - (scale.x))> 0)
			capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2) - (scale.x));

		transform.setToIdentity();
		this->rigidBody->addCollider(capsuleShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}
	else if (owner->objectType == GameObject::Capsule)
	{
		CapsuleShape* capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, 0.01f);
		if (((scale.y * 2) - (scale.x)) > 0)
			capsuleShape = physicsCommon->createCapsuleShape(scale.x / 2, (scale.y * 2) - (scale.x));

		transform.setToIdentity();
		this->rigidBody->addCollider(capsuleShape, transform);
		this->rigidBody->updateMassFromColliders();
		this->rigidBody->enableGravity(true);
		this->rigidBody->setType(BodyType::DYNAMIC);
	}


	transform = this->rigidBody->getTransform();
	float matrix[16];

	transform.getOpenGLMatrix(matrix);

	this->owner->setLocalMatrix(matrix);
}

const wchar_t* PhysicsComponent::getTexturePath()
{
	return nullptr;
}

PhysicsComponent::~PhysicsComponent()
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(this);
}

void PhysicsComponent::perform(float deltaTime)
{
	const Transform transform = this->rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);
	this->owner->setLocalMatrix(matrix);
	//std::cout << "My Component is Updating: " << this->name << " : " << matrix[13] << std::endl;
}

RigidBody* PhysicsComponent::getRigidBody()
{
	return this->rigidBody;
}
