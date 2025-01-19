#include "texture.h"

#include <stdio.h>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static unsigned char* image_load(const char* image_file_path, int* width, int* height, int* channels);
static void image_unload(unsigned char* image);

Texture texture_create(const char* image_file_path) {
    Texture texture = {0};

    unsigned char* image_data = image_load(image_file_path, &texture.width, &texture.height, &texture.channels);
    if (!image_data) {
        printf("ERROR: Failed to load texture image!\n");
    }

    glGenTextures(1, &texture.id);
    texture_bind(&texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    texture_unbind();

    image_unload(image_data);

    return texture;
}

void texture_bind(Texture* texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(Texture* texture) {
    glDeleteTextures(1, &texture->id);
}

static unsigned char* image_load(const char* image_file_path, int* width, int* height, int* channels) {
    unsigned char* image_data = stbi_load(image_file_path, width, height, channels, 4);
    if (!image_data) {
        return NULL;
    }

    return image_data;
}

static void image_unload(unsigned char* image) {
    stbi_image_free(image);
}