#include "InputSystem.h"

InputSystem& InputSystem::Instance()
{
	static InputSystem _instance;
	return _instance;
}

void InputSystem::SetKeyState(int key, int state)
{
	m_Keys[key] = state;
}

int InputSystem::GetKeyState(int key) const
{
	return m_Keys[key];
}

void InputSystem::SetMousePosition(MousePosition pos)
{
	m_MousePosition.x = pos.x;
	m_MousePosition.y = pos.y;
}

MousePosition InputSystem::GetMousePosition() const
{
	return m_MousePosition;
}

void InputSystem::SetScrollOffset(ScrollOffset offset)
{
	m_ScrollOffset.x = offset.x;
	m_ScrollOffset.y = offset.y;
}

ScrollOffset InputSystem::GetScrollOffset()
{
	ScrollOffset offset = m_ScrollOffset;
	m_ScrollOffset = { 0, 0 };
	return offset;
}

InputSystem::InputSystem()
	: m_MousePosition({ 0, 0 }),
	m_ScrollOffset({ 0, 0 })
{
}

InputSystem::~InputSystem() = default;
