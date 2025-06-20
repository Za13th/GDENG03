#include "Camera.h"
#include "InputSystem.h"

#include "EngineTime.h"

#include <iostream>

Camera::Camera() : GameObject("Camera")
{
	this->setPosition(Vector3D(0.0f, 0.0f, -2.0f)); // Default position)
	this->localMatrix.setTranslation(Vector3D(0.0f, 0.0f, -2.0f), false);
	this->updateViewMatrix();

	InputSystem::getInstance()->addListener(this);
}

Camera::Camera(std::string name) : GameObject(name)
{
	this->setPosition(Vector3D(0.0f, 0.0f, -2.0f)); // Default position)
	this->localMatrix.setTranslation(Vector3D(0.0f, 0.0f, -2.0f), false);
	this->updateViewMatrix();
	
	InputSystem::getInstance()->addListener(this);
}

Camera::~Camera()
{
	InputSystem::getInstance()->removeListener(this);
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam; worldCam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();

	Vector3D localRot = getLocalRotation();
	
	temp.setRotationX(m_rot_x, true);
	worldCam *= temp;
	temp.setRotationY(m_rot_y, true);
	worldCam *= temp;


	Vector3D new_pos = this->localMatrix.getTranslation()
		+ localMatrix.getZDirection() * (m_forward * movement_speed * EngineTime::getDeltaTime());
	new_pos = new_pos 
		+ localMatrix.getXDirection() * (m_rightward * movement_speed * EngineTime::getDeltaTime());
	new_pos = new_pos 
		+ localMatrix.getYDirection() * (m_upward * movement_speed * EngineTime::getDeltaTime());
	worldCam.setTranslation(new_pos, false);

	this->localMatrix = worldCam;

	this->setPosition(localMatrix.getTranslation());
	




	//this->localMatrix = worldCam;


}

void Camera::update(float deltaTime)
{
	if (InputSystem::getInstance()->isKeyDown('W'))
		m_forward = 1.0f;
	else if (InputSystem::getInstance()->isKeyDown('S'))
		m_forward = -1.0f;
	if (InputSystem::getInstance()->isKeyDown('A'))
		m_rightward = -1.0f;
	else if (InputSystem::getInstance()->isKeyDown('D'))
		m_rightward = 1.0f;
	if (InputSystem::getInstance()->isKeyDown('Q'))
		m_upward = 1.0f;
	else if (InputSystem::getInstance()->isKeyDown('E'))
		m_upward = -1.0f;

	static bool pressed = false;
	if (InputSystem::get()->isKeyDown('X') && !pressed)
	{
		if (!this->cameraRotation)
			this->cameraRotation = true;
		else this->cameraRotation = false;

		InputSystem::get()->showCursor(!this->cameraRotation);
		pressed = true;
	}
	else if (InputSystem::get()->isKeyUp('X'))
		{
			pressed = false;
		}
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

void Camera::onKeyDown(int key)
{
	if (key == 'W' || key == 'S' || key == 'A' || key == 'D' || key == 'Q' || key == 'E')
	this->updateViewMatrix();
}

void Camera::onKeyUp(int key)
{
	if(key == 'W' || key == 'S')
	m_forward = 0.0f;
	if (key == 'A' || key == 'D')
	m_rightward = 0.0f;
	if (key == 'Q' || key == 'E')
	m_upward = 0.0f;
}

void Camera::onMouseMove(const Point& mouse_pos)
{

	//Fix Later: Most probably doesn't work do to update removing the old position
	if (cameraRotation)
	{
			m_rot_x += (mouse_pos.y - (height/ 2.0f)) * EngineTime::getDeltaTime() * 0.2f;
			m_rot_y += (mouse_pos.x - (width / 2.0f)) * EngineTime::getDeltaTime() * 0.2f;
			InputSystem::get()->setCursorPositon(Point(width / 2.0f, (height/ 2.0f))); // Reset cursor position to center of the window

			this->setRotation(m_rot_x, m_rot_y, 0.0f);
			this->updateViewMatrix();
	}
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{

}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{

}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	// Handle right mouse button down if needed
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	// Handle right mouse button up if needed
}




