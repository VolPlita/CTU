#pragma once
#define TITLE "Super-puper city-building game 3000"

enum class eventlist {quit, motion, press, nothing};
enum class tilelist {grass, flowers, water, sand};
enum class buildinglist {halupa, lumber, mine, fisher, farm};

const int SCREEN_WIDTH = 1536; // 1280 + 256
const int SCREEN_HEIGHT = 832; // 640 + 96 * 2

const int MENU_AMOUNT = 2;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;
const int MAP_SIZE = 20;
const int INIT_X = SCREEN_WIDTH / 2 - TILE_HEIGHT;
const int INIT_Y = TILE_HEIGHT + TILE_WIDTH; // 64 + 32
const int TILE_SPRITE_AMOUNT = 20;
const int BUTTON_SPRITE_AMOUNT = 8;