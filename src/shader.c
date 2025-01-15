#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

static const char* file_to_string(const char* file_path);
static unsigned int create_individual_shader(const char* shader_source, GLenum shader_type);

Shader shader_create(const char* vertex_shader_path, const char* fragment_shader_path) {
    Shader shader = {0};
    
    unsigned int vertex_shader = create_individual_shader(file_to_string(vertex_shader_path), GL_VERTEX_SHADER);
    unsigned int fragment_shader = create_individual_shader(file_to_string(fragment_shader_path), GL_FRAGMENT_SHADER);

    shader.id = glCreateProgram();

    glAttachShader(shader.id, vertex_shader);
    glAttachShader(shader.id, fragment_shader);
    glLinkProgram(shader.id);

    int shader_result;
    char shader_result_buffer[512];

    glGetProgramiv(shader.id, GL_LINK_STATUS, &shader_result);
    if (!shader_result) {
        glGetProgramInfoLog(shader.id, 512, NULL, shader_result_buffer);
        printf("ERROR: Failed to link shader program!\n");
        printf("%s", shader_result_buffer);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader;
}

void shader_bind(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_delete(Shader* shader) {
    glDeleteProgram(shader->id);
}

static const char* file_to_string(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("ERROR: Failed to open vertex shader source!\n");
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* string = malloc(length + 1); // + 1 bc null terminator
    if (!string) {
        printf("ERROR: Failed to allocate memory for string!\n");
    }

    unsigned int bytes_read = fread(string, 1, length, file);
    string[bytes_read] = '\0';
    
    fclose(file);

    return string;
}

static unsigned int create_individual_shader(const char* shader_source, GLenum shader_type) {
    int shader_result;
    char shader_result_buffer[512];

    unsigned int shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_result);
    if (!shader_result) {
        glGetShaderInfoLog(shader, 512, NULL, shader_result_buffer);

        char* type;
        switch (shader_type) {
            case GL_VERTEX_SHADER:
                type = "vertex";
                break;
            case GL_FRAGMENT_SHADER:
                type = "fragment";
                break;
            default:
                type = "unknown";
                break;
        }

        printf("ERROR: Failed to compile %s shader!\n", type);
        printf("%s", shader_result_buffer);
    }

    return shader;
}