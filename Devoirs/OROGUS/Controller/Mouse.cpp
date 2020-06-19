#include "Mouse.h"

MouseController::MouseController()
    : m_xPos(0)
    , m_yPos(0)
    , m_lastMovX(0)
    , m_lastMovY(0)
    , m_buttonState{ false, false }
{
}

bool MouseController::LButtonPressed() const
{
    return m_buttonState[0];
}

bool MouseController::RButtonPressed() const
{
    return m_buttonState[1];
}

bool MouseController::isButtonPressed(int button) const
{
    return m_buttonState[button];
}

float MouseController::xPos() const
{
    return m_xPos;
}

float MouseController::yPos() const
{
    return m_yPos;
}

float MouseController::lastMovX() const
{
    return m_lastMovX;
}

float MouseController::lastMovY() const
{
    return m_lastMovY;
}

void MouseController::updateButtonState(int button, bool pressed)
{
    m_buttonState[button] = pressed;
}

void MouseController::updatePosition(float x, float y)
{
    m_lastMovX = x - m_xPos;
    m_xPos = x;

    m_lastMovY = m_yPos - y;
    m_yPos = y;
}
