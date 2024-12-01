#pragma once
#include "raylib.h"
#include <string>
#include "utility.hpp"

using namespace std;
class Button {
private:
    Rectangle bounds;   // Button rectangle (position and size)
    string label;  // Button label
    Color color;        // Button color
    Color hoverColor;   // Color when hovered over
    Color textColor;

public:
    // Constructor
    Button(float x, float y, float width, float height, const string& label,
        Color color = LIGHTGRAY, Color hoverColor = GRAY, Color textColor = BLACK)
        : bounds{ x, y, width, height }, label(label), color(color), hoverColor(hoverColor), textColor(textColor) {}

    // Draw the button
    void Draw() const {
        Color currentColor = CheckCollisionPointRec(GetMousePosition(), bounds) ? hoverColor : color;
        DrawRectangleRec(bounds, currentColor);
        DrawRectangleLinesEx(bounds,2,BLACK);
        DrawTextCentered(label.c_str(), bounds, 25, textColor);
    }
    string getLabel() const {
        return label;
    }
    // Check if the button is clicked
    bool IsClicked() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
};
