#include "UI.hpp"


//===============================================================================================================================================
//TECHICAL FUNCTION
//===============================================================================================================================================


icon_button setting_button;
icon_button home_button;
text_button save_button;
text_button create_button;
text_button view_button;

void setting_page() {
	//Draw the page
	DrawRectangleRec({ 0,0,cell_size * window_x,cell_size * window_y, }, { 203,210,164,200 });
	DrawRectangleRec({ cell_size * (window_x - 40) / 2,cell_size * (window_y - 56) / 2,cell_size * 40,cell_size * 56 }, GRAY);
	DrawRectangleLinesEx({ cell_size * (window_x - 40) / 2,cell_size * (window_y - 56) / 2,cell_size * 40,cell_size * 56 }, 3, BLACK);

	//Disable all button exept setting page
	create_button.is_allow = !setting_popup;
	view_button.is_allow = !setting_popup;
}


//===============================================================================================================================================
//HOME PAGE
//===============================================================================================================================================
 




//===============================================================================================================================================
//THE MAIN APP FUNCTION
//===============================================================================================================================================


//Initialize object's value and resource
void Init() {

	//Setting button
	setting_button.size = { cell_size * 5,cell_size * 5 };
	setting_button.position = { (window_x - 1) * cell_size - setting_button.size.x, cell_size * 1 };
	setting_button.info = "Setting";
	setting_button.color_default = LIGHTGRAY;
	setting_button.color_hovered = WHITE;
	//setting_button.texture = LoadTexture("asset\Cog_Icon.png");

	//Home Button
	home_button.info = "Home";
	home_button.size = { cell_size * 5,cell_size * 5 };
	home_button.position = { (window_x - 1 - 5 - 2) * cell_size - home_button.size.x, cell_size * 1 };
	home_button.color_default = LIGHTGRAY;
	home_button.color_hovered = WHITE;

	//Create button
	create_button.info = "Create";
	create_button.size = { cell_size * 50, cell_size * 5 };
	create_button.position = { (window_x * cell_size - create_button.size.x) / 2, (window_y * cell_size) / 2 - 2 * create_button.size.y };
	create_button.color_default = LIGHTGRAY;
	create_button.color_hovered = WHITE;

	//View button
	view_button.info = "Open Library";
	view_button.size = { cell_size * 50, cell_size * 5 };
	view_button.position = { (window_x * cell_size - create_button.size.x) / 2, (window_y * cell_size) / 2 };
	view_button.color_default = LIGHTGRAY;
	view_button.color_hovered = WHITE;
}



//De-Initialize all object's resource
void DeInit() {
	UnloadTexture(setting_button.texture);
}

//Main 
void windowupdate() {
	switch (window_stat) {
	default:
		create_button.is_allow = true;
		view_button.is_allow = true;
		create_button.Update();
		view_button.Update();
		if (create_button.button_chosen()) {
			window_stat = 2;
			create_button.is_allow = false;
			view_button.is_allow = false;
		}
		if (view_button.button_chosen()) {
			window_stat = 2;
			create_button.is_allow = false;
			view_button.is_allow = false;
		}
		break;
	case 1:
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) window_stat = 0;
		break;
	case 2:
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) window_stat = 0;
		break;
	case 3:
		
		break;
	}

	//Setting Page Lock and Unlock
	if (setting_button.button_chosen()) {
		setting_popup = !setting_popup;
	}
	if (setting_popup) {
		setting_page();
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) setting_popup = false;
	}
	//Choose Page to Unlock
	else {
		if (home_button.button_chosen()) {
			window_stat = 0;
			create_button.is_allow = true;
			view_button.is_allow = true;
		}
	}
	setting_button.Update();
	home_button.Update();
	ClearBackground(window_bg_color);
}

