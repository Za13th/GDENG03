#pragma once
#include "GameObject.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "InputSystem.h"


class Camera : public GameObject, public InputListener
{
	public:
		Camera();
		Camera(std::string name);
		~Camera();

		void update(float deltaTime) override;
		void updateViewMatrix();
		Matrix4x4 getViewMatrix();
		void setCameraMovement(bool movement) { this->cameraMovement = movement; }


		void onKeyDown(int key) override;
		void onKeyUp(int key) override;
		void onMouseMove(const Point& mouse_pos) override;
		void onLeftMouseDown(const Point& mouse_pos) override;
		void onLeftMouseUp(const Point& mouse_pos) override;
		void onRightMouseDown(const Point& mouse_pos) override;
		void onRightMouseUp(const Point& mouse_pos) override;


		void draw(int width, int height, VertexShader* vs, PixelShader* ps) override {};

		bool getCameraRotation() { return this->cameraRotation; }
		void setCameraRotation(bool cr) { cameraRotation = cr; }
		int height = 0;
		int width = 0;

	private:
		bool cameraMovement = true;
		bool cameraRotation = false;
		float m_rot_x = 0.0f;
		float m_rot_y = 0.0f;
		float m_forward = 0.0f;
		float m_rightward = 0.0f;
		float m_upward = 0.0f;

		Matrix4x4 localMatrix;
		float movement_speed = 4.0f; // Movement speed
};

