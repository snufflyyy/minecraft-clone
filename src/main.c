#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "chunk.h"
#include "math/matrix4f.h"

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

// temp globals
double previous_time;
double current_time;
double time_diffrence;
unsigned int frames;
void calculate_fps_and_deltatime() {
    current_time = glfwGetTime();
    time_diffrence = current_time - previous_time;
    frames++;

    if (time_diffrence >= 1.0 / 5.0) {
        printf("FPS: %.2f\n", (1.0 / time_diffrence) * frames);
        previous_time = current_time;
        frames = 0;
    }
}

int main() {
    Window window = window_create(1280, 720, "Minecraft Clone");
    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");
    Texture texture = texture_create("../assets/textures/dirt.jpg");
    Fly_Camera camera = fly_camera_create(window_get_width(&window), window_get_height(&window));

    Chunk test_chunk = chunk_create();

    while (!window_should_close(&window)) {
        calculate_fps_and_deltatime();

        processInput(&window, &camera);
        fly_camera_update(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture_bind(&texture);
        shader_bind(&test_shader);
        test_chunk.model = matrix4f_identity();
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, test_chunk.model.values);
        chunk_draw(&test_chunk);

        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "view"), 1, GL_FALSE, camera.view.values);
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, camera.projection.values);

        window_update(&window);
    }

    shader_delete(&test_shader);
    texture_delete(&texture);
    window_destory(&window);

    return 0;
}