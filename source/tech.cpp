#pragma once
#include "tech.hpp"

void icon_button::Draw() {
	DrawRectangleRec({ position.x, position.y, size.x, size.y }, color_default);
	DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 3, BLACK);
	//DrawTexture(texture,position.x,position.y,RAYWHITE);
}
void icon_button::Draw_hovered() {
	DrawRectangleRec({ position.x, position.y, size.x, size.y }, color_hovered);
	DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 3, BLACK);
	DrawTextHover(info.c_str(), BLACK);
}
bool icon_button::button_chosen() {
	return (is_chosen({ position.x,position.y,size.x,size.y }));
}

void icon_button::Update() {
	Draw();
	if (is_mouse_hovered({ position.x, position.y, size.x, size.y })) Draw_hovered();
}


//===============================================================================================================================================


void text_button::Draw() {
	DrawRectangleRec({ position.x, position.y, size.x, size.y }, color_default);
	DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 3, BLACK);
	DrawTextMiddle(info.c_str(), { position.x,position.y,size.x,size.y }, 50, BLACK);
}

void text_button::Draw_hovered() {
	float offset = 5;
	DrawRectangleRec({ position.x - (offset / 2), position.y - (offset / 2), size.x + offset, size.y + offset }, color_hovered);
	DrawRectangleLinesEx({ position.x - (offset / 2), position.y - (offset / 2), size.x + offset, size.y + offset }, 3, BLACK);
	DrawTextMiddle(info.c_str(), { position.x,position.y,size.x,size.y }, 50, BLACK);
}
bool text_button::button_chosen() {
	return (is_chosen({ position.x,position.y,size.x,size.y }) && is_allow);
}

void text_button::Update() {
	Draw();
	if (is_mouse_hovered({ position.x, position.y, size.x, size.y }) && is_allow) Draw_hovered();
}


//===============================================================================================================================================


void text_box::Draw(){
	DrawRectangleRec({ position.x, position.y, size.x, size.y }, color_default);
	DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 3, BLACK);
	DrawTextMiddle(info.c_str(), { position.x,position.y,size.x,size.y }, 50, BLACK);
}



//===============================================================================================================================================