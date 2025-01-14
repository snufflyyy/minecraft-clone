#include "math/vector3f.h"

#include <math.h>

Vector3f vector3f_add(Vector3f* vector1, Vector3f* vector2) {
    Vector3f vector;

    vector.x = vector1->x + vector2->x;
    vector.y = vector1->y + vector2->y;
    vector.z = vector1->z + vector2->z;

    return vector;
}

Vector3f vector3f_subtract(Vector3f* vector1, Vector3f* vector2) {
    Vector3f vector;

    vector.x = vector1->x - vector2->x;
    vector.y = vector1->y - vector2->y;
    vector.z = vector1->z - vector2->z;

    return vector;
}

float vector3f_dot_product(Vector3f* vector1, Vector3f* vector2) {
    return vector1->x * vector2->x + vector1->y * vector2->y + vector1->z * vector2->z;
}

Vector3f vector3f_cross_product(Vector3f* vector1, Vector3f* vector2) {
    Vector3f vector;

    vector.x = (vector1->y * vector2->z) - (vector1->z * vector2->y);
    vector.y = (vector1->z * vector2->x) - (vector1->x * vector2->z);
    vector.z = (vector1->x * vector2->y) - (vector1->y * vector2->x);

    return vector;
}

void vector3f_scale(Vector3f* vector, float scalar) {
    vector->x *= scalar;
    vector->y *= scalar;
    vector->z *= scalar;
}

void vector3f_divide_by_scalar(Vector3f* vector, float scalar) {
    if (scalar == 0) {
        return;
    }

    vector->x /= scalar;
    vector->y /= scalar;
    vector->z /= scalar;
}

void vector3f_normalize(Vector3f* vector) {
    float magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);

    vector3f_divide_by_scalar(vector, magnitude);
}

void vector3f_normalize_and_scale(Vector3f* vector, Vector3f* scalar_vector) {
    float vector_magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
    float scalar_vector_magnitude = sqrtf(scalar_vector->x * scalar_vector->x + scalar_vector->y * scalar_vector->y + scalar_vector->z * scalar_vector->z);

    vector3f_divide_by_scalar(vector, vector_magnitude);
    vector3f_scale(vector, scalar_vector_magnitude);
}