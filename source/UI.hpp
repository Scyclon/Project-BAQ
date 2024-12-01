#pragma once
#include "utility.hpp"
#include "tech.hpp"

#define title "FC Project"
#define window_x 80
#define window_y 60
#define cell_size 10
#define window_fps 60
#define window_bg_color {203,210,164}

static int window_stat;
static bool setting_popup = false;
static bool allow_page;

void setting_page();

void Init();
void DeInit();
void windowupdate();
