#include <stdio.h>

#include "window.h"
#include "shader.h"
#include "math/matrix4f.h"

static float vertices[] = {
    -1.0f,  1.0f, 0.0f, // top left
     1.0f,  1.0f, 0.0f, // top right
    -1.0f, -1.0f, 0.0f, // bottom left
     1.0f, -1.0f, 0.0f  // bottom right
};
static unsigned int indices[] = {
    0, 2, 1, // left triangle
    2, 1, 3  // right triangle
};

int main() {
    Window window = window_create(1920, 1080, "Minecraft Clone");

    // quad
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader test_shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");

    Matrix4f model = matrix4f_identity();
    Matrix4f view = matrix4f_identity();
    Matrix4f projection = matrix4f_perspective(75, (float) window_get_width(&window) / (float) window_get_height(&window), 0.01f, 1000.0f);

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
        
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(&test_shader);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        window_update(&window);
    }

    shader_delete(&test_shader);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    window_destory(&window);

    return 0;
}