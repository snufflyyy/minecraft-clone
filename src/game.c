#include "game.h"

#include "window.h"

Game game_create() {
    Game game = {0};

    game.window = window_create(1280, 720, "minecraft");

    return game;
}

void game_update(Game* game) {

}

void game_draw(Game* game) {

}

void game_delete(Game* game) {

}
