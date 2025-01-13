#pragma once

typedef struct Shader {
    unsigned int id;
} Shader;

Shader shader_create(const char* vertexShaderPath, const char* fragmentShaderPath);
void shader_use(Shader* shader);
void shader_delete(Shader* shader);