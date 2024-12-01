#pragma once
#include "utility.hpp"

class icon_button {
public:
	string info;
	Vector2 size;
	Vector2 position;
	Color color_default;
	Color color_hovered;
	Texture texture;
	void Draw();
	void Draw_hovered();
	bool button_chosen();
	void Update();
};

class text_button {
public:
	string info;
	Vector2 size;
	Vector2 position;
	Color color_default;
	Color color_hovered;
	bool is_allow;
	void Draw();
	void Draw_hovered();
	bool button_chosen();
	void Update();
};

class text_box {
	string info;
	Vector2 size;
	Vector2 position;
	Color color_default;
	Color color_hovered;
	bool is_allow;
	void Draw();
	void Draw_hovered();
	bool button_chosen();
	void Update();
};