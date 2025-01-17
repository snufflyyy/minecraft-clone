#include "game.h"

int main() {
    Game game = game_create();
    
    while (!window_should_close(&game.window)) {
        game_update(&game);
        game_draw(&game);
    }

    game_delete(&game);

    return 0;
}