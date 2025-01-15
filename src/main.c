#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "window.h"
#include "shader.h"
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

#define NUMBER_OF_CHUNKS 1

int main() {
    srand(time(NULL));

    Window window = window_create(1920, 1080, "Minecraft Clone");
    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");
    Fly_Camera camera = fly_camera_create(window_get_width(&window), window_get_height(&window));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, channels;
    unsigned char* image = stbi_load("../assets/textures/dirt.jpg", &width, &height, &channels, 4);
    if (!image) {
        printf("failed!\n");
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    Chunk test_chunks[NUMBER_OF_CHUNKS];
    for (int i = 0; i < NUMBER_OF_CHUNKS; i++) {
        test_chunks[i] = chunk_create();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!window_should_close(&window)) {
        calculate_fps_and_deltatime();

        processInput(&window, &camera);
        fly_camera_update(&camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&test_shader);
        for (int i = 0; i < NUMBER_OF_CHUNKS; i++) {
            //chunk_generate_mesh(&test_chunks[i]);
            test_chunks[i].model = matrix4f_identity();
            matrix4f_translate(&test_chunks[i].model, (Vector3f) {(float) i * CHUNK_SIZE, 0.0f, 0.0f});
            glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, test_chunks[i].model.values);
            chunk_draw(&test_chunks[i]);
        }

        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "view"), 1, GL_FALSE, camera.view.values);
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, camera.projection.values);

        window_update(&window);
    }

    shader_delete(&test_shader);
    window_destory(&window);

    return 0;
}