#pragma once

#include "chunk.h"
#include "math/matrix4f.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

typedef struct World {
    int render_distance;

    Chunk* chunks;
    Matrix4f model;

    Shader shader;
    int model_uniform_location;
    int view_uniform_location;
    int projection_uniform_location;

    Texture texture_atlas;
} World;

World world_create(int render_distance);
void world_update(World* world);
void world_draw(World* world, Fly_Camera* camera);
void world_destory(World* world);