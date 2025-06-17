#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>

class InputSystem
{
	public:
		static InputSystem* getInstance();
		static void initialize();
		static void destroy();



		void addListener(InputListener* listener);
		void removeListener(InputListener* listener);
		void update();

		bool isKeyDown(int key);
		bool isKeyUp(int key);

		void setCursorPositon(const Point& pos);
		void showCursor(bool show);

		static InputSystem* get();
		Point getOldMousePosition();


	private:
		InputSystem() = default;
		~InputSystem() = default;
		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;
		static InputSystem* sharedInstance;

		std::unordered_set<InputListener*> m_set_listeners; // Map to hold key listeners
		unsigned char m_key_state[256] = {}; 
		unsigned char m_old_key_state[256] = {};
		Point m_old_mouse_pos;
		bool m_first_time = true;
};

