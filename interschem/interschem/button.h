#pragma once
#include "raylib.h"
#include <string>
using namespace std;

class Button {
public:
    Button(float x, float y, float width, float height, const std::string& text,
        Color normalColor = GRAY, Color hoverColor = LIGHTGRAY, Color pressColor = DARKGRAY)
        : x(x), y(y), width(width), height(height), text(text),
        normalColor(normalColor), hoverColor(hoverColor), pressColor(pressColor),
        isHovered(false), isPressed(false) {
    }

    void Update() {
        Vector2 mousePoint = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePoint, GetRect());
        isPressed = isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void Draw() const {
        Color color = normalColor;
        if (isPressed) {
            color = pressColor;
        }
        else if (isHovered) {
            color = hoverColor;
        }

        DrawRectangleRounded(GetRect(), 0.5f, 5, color);
        int fontSize = 25;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);
        Vector2 textPosition = { x + (width - textSize.x) / 2, y + (height - textSize.y) / 2 };
        DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, BLACK);
    }

    bool WasPressed() const {
        return isPressed;
    }

    void SetColors(Color normal, Color hover, Color press) {
        normalColor = normal;
        hoverColor = hover;
        pressColor = press;
    }

private:
    float x, y, width, height;
    string text;
    bool isHovered;
    bool isPressed;
    Color normalColor;
    Color hoverColor;
    Color pressColor;

    Rectangle GetRect() const {
        return { x, y, width, height };
    }
};
