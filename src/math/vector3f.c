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

float vector3f_magnitude(Vector3f* vector) {
    return sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
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
        vector->x = 0;
        vector->y = 0;
        vector->z = 0;
        return;
    }

    vector->x /= scalar;
    vector->y /= scalar;
    vector->z /= scalar;
}

void vector3f_normalize(Vector3f* vector) {
    float magnitude = vector3f_magnitude(vector);
    vector3f_divide_by_scalar(vector, magnitude);
}