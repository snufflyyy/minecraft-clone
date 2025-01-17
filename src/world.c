#include "world.h"

#include <stdlib.h>

#include <glad/glad.h>

#include "camera.h"

World world_create(int render_distance) {
    World world = {0};

    world.render_distance = render_distance;

    int chunk_count = world.render_distance * world.render_distance * world.render_distance;
    world.chunks = malloc(chunk_count * sizeof(Chunk));

    for (int x = 0; x < world.render_distance; x++) {
        for (int y = 0; y < world.render_distance; y++) {
            for (int z = 0; z < world.render_distance; z++) {
                world.chunks[x * (render_distance * render_distance) + y * render_distance + z] = chunk_create();
            }
        }
    }

    world.model = matrix4f_identity();
    world.shader = shader_create("../assets/shaders/test.vert", "../assets/shaders/test.frag");
    world.texture_atlas = texture_create("../assets/textures/dirt.jpg");

    return world;
}

void world_update(World* world) {

}

void world_draw(World* world, Fly_Camera* camera) {
    for (int x = 0; x < world->render_distance; x++) {
        for (int y = 0; y < world->render_distance; y++) {
            for (int z = 0; z < world->render_distance; z++) {
                world->model = matrix4f_identity();
                matrix4f_translate(&world->model, (Vector3f) {(float) x * CHUNK_SIZE, (float) y * CHUNK_SIZE, (float) z * CHUNK_SIZE});

                shader_bind(&world->shader);
                glUniformMatrix4fv(glGetUniformLocation(world->shader.id, "model"), 1, GL_FALSE, world->model.values);
                glUniformMatrix4fv(glGetUniformLocation(world->shader.id, "view"), 1, GL_FALSE, camera->view.values);
                glUniformMatrix4fv(glGetUniformLocation(world->shader.id, "projection"), 1, GL_FALSE, camera->projection.values);

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
                chunk_destory(&world->chunks[x * (world->render_distance * world->render_distance) + y * world->render_distance + z]);
            }
        }
    }
}