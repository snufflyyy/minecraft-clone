#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

static const char* file_to_string(const char* file_path);
static unsigned int create_individual_shader(const char* shader_source, GLenum shader_type);

Shader shader_create(const char* vertex_shader_path, const char* fragment_shader_path) {
    Shader shader = {0};
    
    unsigned int vertexShader = create_individual_shader(file_to_string(vertex_shader_path), GL_VERTEX_SHADER);
    unsigned int fragmentShader = create_individual_shader(file_to_string(fragment_shader_path), GL_FRAGMENT_SHADER);

    shader.id = glCreateProgram();

    glAttachShader(shader.id, vertexShader);
    glAttachShader(shader.id, fragmentShader);
    glLinkProgram(shader.id);

    int shaderResult;
    char shaderResultBuffer[512];

    glGetProgramiv(shader.id, GL_LINK_STATUS, &shaderResult);
    if (!shaderResult) {
        glGetProgramInfoLog(shader.id, 512, NULL, shaderResultBuffer);
        printf("ERROR: Failed to link shader program!\n");
        printf("%s", shaderResultBuffer);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
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

    char* string = malloc(length + 1); // + 1 for null terminator
    if (!string) {
        printf("ERROR: Failed to allocate memory for string!\n");
    }

    unsigned int bytesRead = fread(string, 1, length, file);
    string[bytesRead] = '\0'; 
    
    fclose(file);

    return string;
}

static unsigned int create_individual_shader(const char* shader_source, GLenum shader_type) {
    int shaderResult;
    char shaderResultBuffer[512];

    unsigned int shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderResult);
    if (!shaderResult) {
        glGetShaderInfoLog(shader, 512, NULL, shaderResultBuffer);

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
        printf("%s", shaderResultBuffer);
    }

    return shader;
}