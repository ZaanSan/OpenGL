#pragma once

#include <GLFW/glfw3.h>

// Very basic input system

struct MousePosition
{
	float x;
	float y;
};

struct ScrollOffset
{
	float x;
	float y;
};

class InputSystem
{
private:
	int				m_Keys[GLFW_KEY_END] = {0};
	MousePosition	m_MousePosition;
	ScrollOffset	m_ScrollOffset;

public:
	static InputSystem &Instance();

	void			SetKeyState(int key, int state);
	int				GetKeyState(int key) const;
	void			SetMousePosition(MousePosition pos);
	MousePosition	GetMousePosition() const;
	void			SetScrollOffset(ScrollOffset scroll);
	ScrollOffset	GetScrollOffset();

private:
	InputSystem();
	~InputSystem();
};

