#include "Camera.h"
#include "InputSystem.h"

Camera::Camera() : GameObject("Camera")
{

}

Camera::Camera(std::string name) : GameObject(name)
{

}

Camera::~Camera()
{

}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam; worldCam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();

	Vector3D localRot = getLocalRotation();
	
	temp.setRotationX(localRot.x, true);
	worldCam *= temp;

	temp.setRotationY(localRot.y, true);
	worldCam *= temp;

	temp.setTranslation(getLocalPosition(), false);
	worldCam *= temp;

	worldCam.inverse();
	this->localMatrix = worldCam;
}

void Camera::update(float deltaTime)
{
	Vector3D localPos = getLocalPosition();
	float x = localPos.x;
	float y = localPos.y;
	float z = localPos.z;

	if (InputSystem::get()->isKeyDown('W'))
	{
		z += movement_speed * deltaTime;
		this->setPosition(Vector3D(x, y, z));
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('S'))
	{
		z -= movement_speed * deltaTime;
		this->setPosition(Vector3D(x, y, z));
		this->updateViewMatrix();
	}

	if (InputSystem::get()->isKeyDown('A'))
	{
		x -= movement_speed * deltaTime;
		this->setPosition(Vector3D(x, y, z));
		this->updateViewMatrix();
	}
	else if (InputSystem::get()->isKeyDown('D'))
	{
		x += movement_speed * deltaTime;
		this->setPosition(Vector3D(x, y, z));
		this->updateViewMatrix();
	}
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

void Camera::onKeyDown(int key)
{
	// Handle key down events if needed
}

void Camera::onKeyUp(int key)
{
	// Handle key up events if needed
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	//Fix Later: Most probably doesn't work do to update removing the old position
	if (cameraRotation)
	{
		m_rot_x += mouse_pos.x - InputSystem::get()->getOldMousePosition().x;;
		m_rot_y += mouse_pos.y - InputSystem::get()->getOldMousePosition().y;;
		this->setRotation(Vector3D(m_rot_x, m_rot_y, 0.0f));
		this->updateViewMatrix();
	}
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
	if(!cameraRotation)
	cameraRotation = true;
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
	if (cameraRotation)
		cameraRotation = false;
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	// Handle right mouse button down if needed
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	// Handle right mouse button up if needed
}




