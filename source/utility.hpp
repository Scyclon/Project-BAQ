#pragma once
#include <vector>
#include <string>
#include <algorithm> // for sort
#include "raylib.h"
#include "raymath.h"

using namespace std;


void DrawTextMiddle(const char* text, Rectangle rec, int font_size, Color color); //C?n l? trái, gi?a
void DrawTextCentered(const char* text, Rectangle rec, int font_size, Color color); // Trung tâm hình ch? nh?t
void DrawRecWithLines(float x, float y, float width, float height, Color color, int lineThick);
void DrawTextHover(const char* text, Color color);
bool is_chosen(Rectangle rect);
bool is_mouse_hovered(Rectangle rect);
void SortStrings(vector<string>& strings);
