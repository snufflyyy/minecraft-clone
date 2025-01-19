#include "world.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "camera.h"

World world_create(int render_distance) {
    World world = {0};

    world.render_distance = render_distance;

    int chunk_count = world.render_distance * world.render_distance * world.render_distance;
    world.chunks = malloc(chunk_count * sizeof(Chunk));
    if (!world.chunks) {
        printf("ERROR: Failed to allocate memory for world!\n");
        exit(-8);
    }

    for (int x = 0; x < world.render_distance; x++) {
        for (int y = 0; y < world.render_distance; y++) {
            for (int z = 0; z < world.render_distance; z++) {
                world.chunks[x * (world.render_distance * world.render_distance) + y * world.render_distance + z] = chunk_create();
            }
        }
    }

    for (int x = 0; x < world.render_distance; x++) {
        for (int y = 0; y < world.render_distance; y++) {
            for (int z = 0; z < world.render_distance; z++) {
                int current_index        = x * (world.render_distance * world.render_distance) + y * world.render_distance + z;
                int above_chunk_index    = x * (world.render_distance * world.render_distance) + (y + 1) * world.render_distance + z;
                int below_chunk_index    = x * (world.render_distance * world.render_distance) + (y - 1) * world.render_distance + z;
                int left_chunk_index     = (x - 1) * (world.render_distance * world.render_distance) + y * world.render_distance + z;
                int right_chunk_index    = (x + 1) * (world.render_distance * world.render_distance) + y * world.render_distance + z;
                int infront_chunk_index  = x * (world.render_distance * world.render_distance) + y * world.render_distance + (z + 1);
                int behind_chunk_index   = x * (world.render_distance * world.render_distance) + y * world.render_distance + (z - 1);

                Chunk* chunk_neighbours[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

                if (y != world.render_distance - 1) {
                    chunk_neighbours[0] = &world.chunks[above_chunk_index];
                }
                if (y != 0) {
                    chunk_neighbours[1] = &world.chunks[below_chunk_index];
                }

                if (x != 0) {
                    chunk_neighbours[2] = &world.chunks[left_chunk_index];
                }
                if (x != world.render_distance - 1) {
                    chunk_neighbours[3] = &world.chunks[right_chunk_index];
                }

                if (z != world.render_distance - 1) {
                    chunk_neighbours[4] = &world.chunks[infront_chunk_index];
                }
                if (z != 0) {
                    chunk_neighbours[5] = &world.chunks[behind_chunk_index];
                }

                chunk_update(&world.chunks[current_index], chunk_neighbours);
                chunk_generate_mesh(&world.chunks[current_index]);
            }
        }
    }

    world.model = matrix4f_identity();
    world.shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");

    world.model_uniform_location = glGetUniformLocation(world.shader.id, "model");
    world.view_uniform_location = glGetUniformLocation(world.shader.id, "view");
    world.projection_uniform_location = glGetUniformLocation(world.shader.id, "projection");

    world.texture_atlas = texture_create("../assets/textures/texture_atlas.png");

    return world;
}

void world_update(World* world) {
    
}

void world_draw(World* world, Fly_Camera* camera) {
    float half_render_distance = (world->render_distance / 2.0f) * CHUNK_SIZE;
    
    for (int x = 0; x < world->render_distance; x++) {
        for (int y = 0; y < world->render_distance; y++) {
            for (int z = 0; z < world->render_distance; z++) {
                world->model = matrix4f_identity();

                matrix4f_translate(&world->model, (Vector3f) {
                    (x * CHUNK_SIZE) - half_render_distance,
                    (y * CHUNK_SIZE) - half_render_distance, 
                    (z * CHUNK_SIZE) - half_render_distance
                });

                shader_bind(&world->shader);
                glUniformMatrix4fv(world->model_uniform_location, 1, GL_FALSE, world->model.values);
                glUniformMatrix4fv(world->view_uniform_location, 1, GL_FALSE, camera->view.values);
                glUniformMatrix4fv(world->projection_uniform_location, 1, GL_FALSE, camera->projection.values);

                texture_bind(&world->texture_atlas);
                chunk_draw(&world->chunks[x * (world->render_distance * world->render_distance) + y * world->render_distance + z]);
            }
        }
    }
}

void world_destory(World* world) {
    texture_delete(&world->texture_atlas);
    shader_delete(&world->shader);

    for (int x = 0; x < world->render_distance; x++) {
        for (int y = 0; y < world->render_distance; y++) {
            for (int z = 0; z < world->render_distance; z++) {
                chunk_destroy(&world->chunks[x * (world->render_distance * world->render_distance) + y * world->render_distance + z]);
            }
        }
    }
}