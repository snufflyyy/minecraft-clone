#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>

#include "window.h"
#include "camera.h"
#include "world.h"

// temp input for camera
void processInput(Window* window, Fly_Camera* camera) {
    if (glfwGetKey(window->glfw_window, GLFW_KEY_W) == GLFW_PRESS) {
        fly_camera_move(camera, FORWARD);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_S) == GLFW_PRESS) {
        fly_camera_move(camera, BACKWARD);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_A) == GLFW_PRESS) {
        fly_camera_move(camera, LEFT);
    }
    if (glfwGetKey(window->glfw_window, GLFW_KEY_D) == GLFW_PRESS) {
        fly_camera_move(camera, RIGHT);
    }

    if (glfwGetKey(window->glfw_window, GLFW_KEY_O) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    double mouse_x, mouse_y;
    glfwGetCursorPos(window->glfw_window, &mouse_x, &mouse_y);
    fly_camera_change_angle(camera, (float) mouse_x, (float) mouse_y);
}

int main() {
    Window window = window_create(1280, 720, "Minecraft Clone");
    Fly_Camera camera = fly_camera_create(window_get_width(&window), window_get_height(&window));

    World world = world_create(12);

    while (!window_should_close(&window)) {
        processInput(&window, &camera);
        fly_camera_update(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world_draw(&world, &camera);

        window_update(&window);
    }

    window_destroy(&window);

    return 0;
}