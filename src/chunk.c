#include "chunk.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <glad/glad.h>

Chunk chunk_create() {
    Chunk chunk = {0};

    // temp until world generation is done
    // just setting all the blocks to grass for now
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                chunk.blocks[x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z].type = GRASS;
            }
        }
    }

    return chunk;
}

void chunk_update(Chunk* chunk, Chunk* neighbors[6]) {
    chunk->number_of_vertices = 0;
    chunk->number_of_indices = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                // in current chunk
                int current_index      = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int top_index          = x * (CHUNK_SIZE * CHUNK_SIZE) + (y + 1) * CHUNK_SIZE + z;
                int bottom_index       = x * (CHUNK_SIZE * CHUNK_SIZE) + (y - 1) * CHUNK_SIZE + z;
                int left_index         = (x - 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int right_index        = (x + 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int forward_index      = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (z + 1);
                int backward_index     = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (z - 1);

                // neighboring chunks
                int top_neighbour      = x * (CHUNK_SIZE * CHUNK_SIZE) + 0 * CHUNK_SIZE + z;
                int bottom_neighbour   = x * (CHUNK_SIZE * CHUNK_SIZE) + (CHUNK_SIZE - 1) * CHUNK_SIZE + z;
                int left_neighbour     = 0 * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int right_neighbour    = (CHUNK_SIZE - 1) * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;
                int forward_neighbour  = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + (CHUNK_SIZE - 1);
                int backward_neighbour = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + 0;

                // if current block is air just skip it
                if (chunk->blocks[current_index].type == AIR) { continue; }

                if (y == CHUNK_SIZE - 1 && neighbors[0]) {
                    if (neighbors[0]->blocks[top_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_top_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[top_index].type == AIR) {
                        chunk->blocks[current_index].is_top_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }

                if (y == 0 && neighbors[1]) {
                    if (neighbors[1]->blocks[bottom_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_bottom_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[bottom_index].type == AIR) {
                        chunk->blocks[current_index].is_bottom_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }

                if (x == 0 && neighbors[2]) {
                    if (neighbors[2]->blocks[left_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_left_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[left_index].type == AIR) {
                        chunk->blocks[current_index].is_left_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }

                if (x == CHUNK_SIZE - 1 && neighbors[3]) {
                    if (neighbors[3]->blocks[right_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_right_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[right_index].type == AIR) {
                        chunk->blocks[current_index].is_right_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }

                if (z == 0 && neighbors[4]) {
                    if (neighbors[4]->blocks[forward_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_forward_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[forward_index].type == AIR) {
                        chunk->blocks[current_index].is_forward_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }

                if (z == CHUNK_SIZE - 1 && neighbors[5]) {
                    if (neighbors[5]->blocks[backward_neighbour].type == AIR) {
                        chunk->blocks[current_index].is_backward_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                } else {
                    if (chunk->blocks[backward_index].type == AIR) {
                        chunk->blocks[current_index].is_backward_face_visible = true;
                        chunk->number_of_vertices += 4;
                        chunk->number_of_indices += 6;
                    }
                }


                /*
                if ((neighbors[0] != NULL && neighbors[0]->blocks[top_neighbour].type == AIR) || chunk->blocks[top_index].type == AIR) {
                    chunk->blocks[current_index].is_top_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }
                if ((neighbors[1] != NULL && neighbors[1]->blocks[bottom_neighbour].type == AIR) || chunk->blocks[bottom_index].type == AIR) {
                    chunk->blocks[current_index].is_bottom_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }

                // check the left and right faces
                if ((neighbors[2] != NULL && neighbors[2]->blocks[left_neighbour].type == AIR) || chunk->blocks[left_index].type == AIR) {
                    chunk->blocks[current_index].is_left_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }
                if ((neighbors[3] != NULL && neighbors[3]->blocks[right_neighbour].type == AIR) || chunk->blocks[right_index].type == AIR) {
                    chunk->blocks[current_index].is_right_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }

                // check the forward and backward faces
                if ((neighbors[4] != NULL && neighbors[4]->blocks[forward_neighbour].type == AIR) || chunk->blocks[forward_index].type == AIR) {
                    chunk->blocks[current_index].is_forward_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }
                if ((neighbors[5] != NULL && neighbors[5]->blocks[backward_neighbour].type == AIR) || chunk->blocks[backward_index].type == AIR) {
                    chunk->blocks[current_index].is_backward_face_visible = true;
                    chunk->number_of_vertices += 4;
                    chunk->number_of_indices += 6;
                }
                */
            }
        }
    }
}

void chunk_generate_mesh(Chunk* chunk) {
    chunk->vertices = malloc(chunk->number_of_vertices * sizeof(Vertex));
    chunk->indices = malloc(chunk->number_of_indices * sizeof(unsigned int));

    chunk->number_of_vertices = 0;
    chunk->number_of_indices = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                int current_index = x * (CHUNK_SIZE * CHUNK_SIZE) + y * CHUNK_SIZE + z;

                // if current block is air just skip it
                if (chunk->blocks[current_index].type == AIR) { continue; }

                // generate top face
                if (chunk->blocks[current_index].is_top_face_visible) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate bottom face
                if (chunk->blocks[current_index].is_bottom_face_visible) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, -1.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate left face
                if (chunk->blocks[current_index].is_left_face_visible) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {-1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate right face
                if (chunk->blocks[current_index].is_right_face_visible) {
                    Vertex v0 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {1.0f, 0.0f, 0.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }

                // generate forward face
                if (chunk->blocks[current_index].is_forward_face_visible) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z - 0.5f},
                        .normal = {0.0f, 0.0f, -1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                }

                // generate backward face
                if (chunk->blocks[current_index].is_backward_face_visible) {
                    Vertex v0 = {
                        .position = {x - 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 0.0f}
                    };
                    Vertex v1 = {
                        .position = {x + 0.5f, y - 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 0.0f}
                    };
                    Vertex v2 = {
                        .position = {x + 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {0.0f, 1.0f}
                    };
                    Vertex v3 = {
                        .position = {x - 0.5f, y + 0.5f, z + 0.5f},
                        .normal = {0.0f, 0.0f, 1.0f},
                        .texture_coord = {1.0f, 1.0f}
                    };

                    chunk->vertices[chunk->number_of_vertices++] = v0;
                    chunk->vertices[chunk->number_of_vertices++] = v1;
                    chunk->vertices[chunk->number_of_vertices++] = v2;
                    chunk->vertices[chunk->number_of_vertices++] = v3;

                    unsigned int base_index = chunk->number_of_vertices - 4;
                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 1;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;

                    chunk->indices[chunk->number_of_indices++] = base_index;
                    chunk->indices[chunk->number_of_indices++] = base_index + 2;
                    chunk->indices[chunk->number_of_indices++] = base_index + 3;
                }
            }
        }
    }

    glGenVertexArrays(1, &chunk->VAO);
    glBindVertexArray(chunk->VAO);

    glGenBuffers(1, &chunk->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, chunk->number_of_vertices * sizeof(Vertex), chunk->vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &chunk->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->number_of_indices * sizeof(unsigned int), chunk->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void chunk_draw(Chunk* chunk) {
    glBindVertexArray(chunk->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glDrawElements(GL_TRIANGLES, chunk->number_of_indices, GL_UNSIGNED_INT, 0);
}

void chunk_destroy(Chunk* chunk) {
    free(chunk->indices);
    chunk->indices = NULL;
    free(chunk->vertices);
    chunk->vertices = NULL;
    
    glDeleteBuffers(1, &chunk->EBO);
    glDeleteBuffers(1, &chunk->VBO);
    glDeleteVertexArrays(1, &chunk->VAO);
}