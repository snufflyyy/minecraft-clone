#include "game.h"

#include "window.h"
#include "camera.h"

Game game_create() {
    Game game = {0};

    game.window = window_create(1280, 720, "minecraft");
    game.world = world_create(5);
    game.fly_camera = fly_camera_create(window_get_width(&game.window), window_get_height(&game.window));

    game.fly_camera.position.y = (game.world.render_distance / 2.0f * CHUNK_SIZE) + 1.0f;

    return game;
}

void game_update(Game* game) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // wireframe
    if (glfwGetKey(game->window.glfw_window, GLFW_KEY_O) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    world_update(&game->world);
    fly_camera_keyboard_mouse_movement(&game->fly_camera, &game->window, game->window.delta_time);
    fly_camera_update(&game->fly_camera);
}

void game_draw(Game* game) {
    world_draw(&game->world, &game->fly_camera);

    window_update(&game->window);
}

void game_delete(Game* game) {
    //fly_camera_destory(&game->fly_camera);
    world_destory(&game->world);
    window_destroy(&game->window);
}
