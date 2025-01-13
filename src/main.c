#include <stdio.h>

#include "window.h"
#include "shader.h"
#include "math/matrix4f.h"

static float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,



     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     1.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     1.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     1.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     1.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     1.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     1.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     1.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     1.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     1.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     1.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     1.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     1.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     1.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     1.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     1.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     1.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     1.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     1.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
};

int main() {
    Window window = window_create(1280, 720, "Minecraft Clone");

    // quad
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");

    Matrix4f model = matrix4f_identity();
    Matrix4f view = matrix4f_identity();
    Matrix4f projection = matrix4f_perspective(60.0f, (float) window_get_width(&window) / (float) window_get_height(&window), 0.01f, 1000.0f);

    matrix4f_translate(&view, (Vector3f) {0.0f, 0.0f, -3.0f});

    shader_use(&test_shader);
    glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, model.values);
    glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "view"), 1, GL_FALSE, view.values);
    glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, projection.values);

    while (!window_should_close(&window)) {
        if (window.just_resized) {
            shader_use(&test_shader);
            projection = matrix4f_perspective(60.0f, (float) window_get_width(&window) / (float) window_get_height(&window), 0.01f, 1000.0f);
            glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "projection"), 1, GL_FALSE, projection.values);
            window.just_resized = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&test_shader);
        model = matrix4f_identity();
        matrix4f_rotate(&model, (float) glfwGetTime() * 50.0f, (Vector3f) {0.0f, 1.0f, 0.0f});
        glUniformMatrix4fv(glGetUniformLocation(test_shader.id, "model"), 1, GL_FALSE, model.values);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));

        window_update(&window);
    }

    shader_delete(&test_shader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    window_destory(&window);

    return 0;
}