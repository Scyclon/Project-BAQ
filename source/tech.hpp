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
    Color hoverColor = { 130,130,130,100 };   // Color when hovered over
    Color textColor;
public:
    // Constructor
    Button(float x, float y, float width, float height, const string& label,
        Color color, Color textColor)
        : bounds{ x, y, width, height }, label(label), color(color), textColor(textColor) {}

    // 
    void SetY(float setY) {
        bounds.y = setY;
    }

    float GetY() {
        return bounds.y;
    }

    // Draw the button
    void Draw() const {
        DrawRectangleRec(bounds, color);
        if (CheckCollisionPointRec(GetMousePosition(), bounds)) DrawRectangleRec(bounds, hoverColor);
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
    Color colorDefault = WHITE;
    Color colorHovered = { 130,130,130,100 };
    float flippProgress = 0.0;
    float showFront = true;
    bool flipping = false;
public:
    FlipCard(float x, float y, float w, float h, string fr, float sizeFront, string ba, float sizeBack)
        : x(x), y(y), width(w), height(h), frontText(fr), backText(ba), sizeFront(sizeFront), sizeBack(sizeBack) {}

    bool IsFront() { return showFront; }
    bool IsFlipping() { return flipping; }
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
        DrawRectangleRec({ centerX,y,currentWidth,height },colorDefault);
        if (CheckCollisionPointRec(GetMousePosition(), { centerX,y,currentWidth,height }) || flipping) DrawRectangle(centerX, y, currentWidth, height, colorHovered);
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
        DrawRectangleRec({ x,centerY,width,currentHeight }, colorDefault);
        if (CheckCollisionPointRec(GetMousePosition(), { x,centerY,width,currentHeight }) || flipping) DrawRectangle(x, centerY, width, currentHeight, colorHovered);
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

class InputBox {
private:
    float x, y, width, height, fontSize;
    string label;
    string text = "\0";
    Color color = WHITE;
    int framecounter = 0;
    bool isTyping = false;
    int maxChar = 40;
public:
    InputBox(float x, float y, float w, float h, const string& label,float fontSize) : x(x), y(y), width(w), height(h), label(label), fontSize(fontSize) {}

    void SetY(float setY) {
        y = setY;
    }
    float GetY() {
        return y;
    }
    float GetHeight() {
        return height;
    }
    void Draw() {
        if (CheckCollisionPointRec(GetMousePosition(), { x,y,width,height }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            isTyping = true;
        }
        if (!CheckCollisionPointRec(GetMousePosition(), { x,y,width,height }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            isTyping = false;
        }

        if (isTyping) {
            framecounter++;
            int key = GetKeyPressed();
            if (key != 0 && key != KEY_ENTER && key != KEY_BACKSPACE && text.length() < maxChar)
                text += key;
            if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
                text.pop_back();
        }

        DrawRectangle(x, y, width, height, color);
        if (isTyping) DrawRectangle(x, y, width, height, { 130,130,130,100 });
        DrawRectangleLinesEx({ x, y, width, height }, 3, BLACK);
        if (text.empty()) DrawTextMiddle(label.c_str(), { x,y,width,height }, fontSize, BLACK);
        else DrawTextMiddle(text.c_str(), { x,y,width,height }, fontSize, BLACK);
    }

    string GetText() {
        return text;
    }
    void SetText(string setText) {
        text = setText;
    }
    void Clear() {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        isTyping = false;
        text.clear();
    }
};

class Roller {
private:
    float x, y, width, height;
    float offset = 0;
    float maxOffset;
    bool isVertical;
    float speed = 10;
public:
    Roller(float x, float y, float width, float height, float maxOffset, bool isVertical)
        : x(x), y(y), width(width), height(height), maxOffset(maxOffset), isVertical(isVertical) {}

    void Draw() {
        Rectangle roller = { x,y,isVertical ? width : (width / (maxOffset / speed)) ,isVertical ? (height / (maxOffset / speed)) : height };
        float scroll = -GetMouseWheelMove();
        offset += scroll * speed;
        if (offset > maxOffset) offset = maxOffset;
        if (offset < 0) offset = 0;
        if (isVertical) {
            roller.y += offset;
            if (roller.y + roller.height > y + height) roller.y = y + height - roller.height;
        }
        else {
            roller.x += offset;
            if (roller.x + roller.width > x + width) roller.x = x + width - roller.width;
        }
       
        DrawRectangle(x, y, width, height, GRAY);
        DrawRectangleRounded(roller, 10, 5, WHITE);
        DrawRectangleLinesEx({ x, y, width, height }, 3, BLACK);
    }

    float getOffset() const{
        return offset;
    }
};