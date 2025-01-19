#pragma once

#include "math/vector3f.h"
#include "math/vector2f.h"

typedef struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texture_coord;
    float texture_index;
} Vertex;