#include "utility.hpp"


//Common use function
void DrawTextMiddle(const char* text, Rectangle rec, int font_size, Color color) {
	int text_width = MeasureText(text, font_size);
	float offset = rec.width / 6;
	while (text_width + 10 > rec.width || font_size + 10 > rec.height) {
		font_size--;
		text_width = MeasureText(text, font_size);
	}
	Vector2 text_position = { (rec.x + offset), rec.y + ((rec.height - font_size) / 2) };
	DrawText(text, text_position.x, text_position.y, font_size, color);
}
void DrawTextCentered(const char* text, Rectangle rec, int font_size, Color color) {
	int text_width = MeasureText(text, font_size);
	while (text_width + 10 > rec.width || font_size + 10 > rec.height) {
		font_size--;
		text_width = MeasureText(text, font_size);
	}
	Vector2 text_position = { rec.x + (rec.width - text_width) / 2, rec.y + ((rec.height - font_size) / 2) };
	DrawText(text, text_position.x, text_position.y, font_size, color);
}

void DrawTextHover(const char* text, Color color) {
	Vector2 mouse = GetMousePosition();
	int font_size = 15;
	int text_width = MeasureText(text, font_size);
	Vector2 text_position = { mouse.x - text_width / 2.0, mouse.y - font_size };
	DrawText(text, text_position.x, text_position.y, font_size, color);
}

bool is_mouse_hovered(Rectangle rect) {
	return CheckCollisionPointRec(GetMousePosition(), rect);
}
bool is_chosen(Rectangle rect) {
	return (is_mouse_hovered(rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}


