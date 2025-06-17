#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::sharedInstance = nullptr;

InputSystem* InputSystem::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new InputSystem();
	}
	return sharedInstance;
}
void InputSystem::initialize()
{
	if (!sharedInstance)
	{
		sharedInstance = new InputSystem();
	}
}
void InputSystem::destroy()
{
	if (sharedInstance != NULL)
		delete sharedInstance;
}


void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.x || current_mouse_pos.y != m_old_mouse_pos.y)
	{
		Point delta_mouse_pos = Point(current_mouse_pos.x - m_old_mouse_pos.x, current_mouse_pos.y - m_old_mouse_pos.y);
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			//(*it)->onMouseMove(delta_mouse_pos);
			(*it)->onMouseMove(Point(current_mouse_pos.x,current_mouse_pos.y));
			++it;
		}
	}

	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(m_key_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			if (m_key_state[i] & 0x80) // Key is pressed
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_key_state[i] != m_old_key_state[i])
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (m_key_state[i] != m_old_key_state[i])
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else 
						(*it)->onKeyDown(i);

					++it;
				}
			}
			else // Key is released
			{
				if (m_key_state[i] != m_old_key_state[i]) // Check if the key state has changed
				{
					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						if (i == VK_LBUTTON)
							(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						else if (i == VK_RBUTTON)
							(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						else 
							(*it)->onKeyUp(i);
						++it;
					}
				}
			}
		} //store current key states to old key states buffer
		::memcpy(m_old_key_state, m_key_state, sizeof(unsigned char) * 256); 
	}
}

bool InputSystem::isKeyDown(int key)
{
	return (m_key_state[key] & 0x80) != 0;
}

bool InputSystem::isKeyUp(int key)
{
	return (m_key_state[key] & 0x80) == 0;
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);

}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::setCursorPositon(const Point& pos)
{
	::SetCursorPos(pos.x, pos.y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	if (!sharedInstance)
	{
		sharedInstance = new InputSystem();
	}
	return sharedInstance;
}

Point InputSystem::getOldMousePosition()
{
	return m_old_mouse_pos;
}