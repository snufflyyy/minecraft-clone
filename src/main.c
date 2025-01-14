#include <camera.h>
#include <stdio.h>

#include "window.h"
#include "shader.h"
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

    double mouse_x, mouse_y;
    glfwGetCursorPos(window->glfw_window, &mouse_x, &mouse_y);
    fly_camera_change_angle(camera, mouse_x, mouse_y);
}

int main() {
    Window window = window_create(1280, 720, "Minecraft Clone");
    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");
    Fly_Camera camera = fly_camera_create(window_get_width(&window), window_get_height(&window));

    Chunk test_chunk = chunk_create();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!window_should_close(&window)) {
        processInput(&window, &camera);
        fly_camera_update(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&test_shader);
        test_chunk.model = matrix4f_identity();
        //matrix4f_rotate(&test_chunk.model, (float) glfwGetTime() * 100.0f, (Vector3f) {0.0f, 1.0f, 0.0f});
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, test_chunk.model.values);
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "view"), 1, GL_FALSE, camera.view.values);
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, camera.projection.values);

        chunk_draw(&test_chunk);

        window_update(&window);
    }

    shader_delete(&test_shader);

    window_destory(&window);

    return 0;
}