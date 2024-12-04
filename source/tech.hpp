#pragma once
#include "raylib.h"
#include <string>
#include "utility.hpp"

using namespace std;
class Button {
private:
    Rectangle bounds;   // Button rectangle (position and size)
    string label;  // Button label
    Color color;        // Button default color
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
        DrawRectangleLinesEx(bounds,3,BLACK);
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


class FlipCard {
private:
    float x, y, width, height;
    string frontText, backText;
    float sizeFront, sizeBack;
    Color colorDefault = GRAY;
    Color colorHovered = DARKGRAY;
    float flippProgress = 0.0;
    float showFront = true;
    bool flipping = false;
public:
    FlipCard(float x, float y, float w, float h, string fr, float sizeFront, string ba, float sizeBack)
        : x(x), y(y), width(w), height(h), frontText(fr), backText(ba), sizeFront(sizeFront), sizeBack(sizeBack) {}

    bool Is_Front() { return showFront; }
    bool Is_Flipping() { return flipping; }
    void DrawFlipH(float frameTime) {
        if (flipping) {
            flippProgress += frameTime; // flipp speed
            if (flippProgress >= 1.0) {
                flippProgress = 1.0;
                flipping = false;
            }
        }
        float scaleX = fabs(flippProgress - 0.5) * 2.0;
        float currentWidth = width * scaleX; // schrink to 0
        float centerX = x + (width - currentWidth) / 2.0;
        DrawRectangleRec({ centerX,y,currentWidth,height }, (CheckCollisionPointRec(GetMousePosition(), { centerX,y,currentWidth,height }) && !flipping) ? colorHovered : colorDefault);
        DrawRectangleLinesEx({ centerX,y,currentWidth,height }, 3, BLACK);

        if (scaleX >= 1.0) DrawTextCentered(showFront ? frontText.c_str() : backText.c_str(), { centerX,y,currentWidth,height }, showFront ? sizeFront : sizeBack, BLACK);
    }

    void DrawFlipV(float frameTime) {
        if (flipping) {
            flippProgress += frameTime; // flipp speed
            if (flippProgress >= 1.0) {
                flippProgress = 1.0;
                flipping = false;
            }
        }
        float scaleY = fabs(flippProgress - 0.5) * 2.0;
        float currentHeight = height * scaleY; // schrink to 0
        float centerY = y + (height - currentHeight) / 2.0;
        DrawRectangleRec({ x,centerY,width,currentHeight }, (CheckCollisionPointRec(GetMousePosition(), { x,centerY,width,currentHeight }) && !flipping) ? colorHovered : colorDefault);
        DrawRectangleLinesEx({ x,centerY,width,currentHeight }, 3, BLACK);
        if (scaleY >= 1.0) DrawTextCentered(showFront ? frontText.c_str() : backText.c_str(), { x,centerY,width,currentHeight }, showFront ? sizeFront : sizeBack, BLACK);
    }

    void StartFlip() {
        if (is_chosen({ x,y,width,height }) && !flipping) {
            flipping = true;
            flippProgress = 0.0;
            showFront = !showFront;
        }
    }
};
