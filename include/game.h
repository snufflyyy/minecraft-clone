#pragma once

#include "window.h"

typedef struct Game {
    Window window;

} Game;

Game game_create();
void game_update(Game* game);
void game_draw(Game* game);
void game_delete(Game* game);