#ifndef _CONTROLLER_MOUSE_H_
#define _CONTROLLER_MOUSE_H_

class MouseController
{
public:
    MouseController();

    bool LButtonPressed() const;
    bool RButtonPressed() const;
    bool isButtonPressed(int button) const;

    float xPos() const;
    float yPos() const;

    float lastMovX() const;
    float lastMovY() const;

    void updateButtonState(int button, bool pressed);
    void updatePosition(float x, float y);

private:
    float m_xPos;
    float m_yPos;
    float m_lastMovX;
    float m_lastMovY;
    bool m_buttonState[8];
};


#endif
