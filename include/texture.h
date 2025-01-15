#pragma once

typedef struct Texture {
    unsigned int id;
    int width;
    int height;
    int channels;
} Texture;

Texture texture_create(const char* image_file_path);
void texture_bind(Texture* texture);
void texture_unbind();
void texture_delete(Texture* texture);