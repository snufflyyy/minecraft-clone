#include <stdio.h>

#include "window.h"
#include "shader.h"
#include "chunk.h"
#include "math/matrix4f.h"

int main() {
    Window window = window_create(1280, 720, "Minecraft Clone");
    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");

    Chunk test_chunk = chunk_create();

    Matrix4f view = matrix4f_identity();
    Matrix4f projection = matrix4f_perspective(60.0f, (float) window_get_width(&window) / (float) window_get_height(&window), 0.01f, 1000.0f);

    matrix4f_translate(&view, (Vector3f) {0.0f, 0.0f, -5.0f});

    shader_use(&test_shader);
    glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "view"), 1, GL_FALSE, view.values);
    glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, projection.values);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!window_should_close(&window)) {
        if (window.just_resized) {
            shader_use(&test_shader);
            projection = matrix4f_perspective(60.0f, (float) window_get_width(&window) / (float) window_get_height(&window), 0.01f, 1000.0f);
            glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, projection.values);
            window.just_resized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&test_shader);
        test_chunk.model = matrix4f_identity();
        matrix4f_rotate(&test_chunk.model, (float) glfwGetTime() * 50.0f, (Vector3f) {0.0f, 1.0f, 0.0f});
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, test_chunk.model.values);

        chunk_draw(&test_chunk);

        window_update(&window);
    }

    shader_delete(&test_shader);

    window_destory(&window);

    return 0;
}