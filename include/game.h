#pragma once

#include "window.h"
#include "world.h"
#include "camera.h"

typedef struct Game {
    Window window;
    World world;
    Fly_Camera fly_camera;
} Game;

Game game_create();
void game_update(Game* game);
void game_draw(Game* game);
void game_delete(Game* game);