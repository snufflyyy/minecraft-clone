#pragma once

typedef struct Vector3f {
    float x, y, z;
} Vector3f;

Vector3f vector3f_add(Vector3f* vector1, Vector3f* vector2);
Vector3f vector3f_subtract(Vector3f* vector1, Vector3f* vector2);
float vector3f_dot_product(Vector3f* vector1, Vector3f* vector2);
float vector3f_magnitude(Vector3f* vector);
Vector3f vector3f_cross_product(Vector3f* vector1, Vector3f* vector2);
void vector3f_scale(Vector3f* vector, float scalar);
void vector3f_divide_by_scalar(Vector3f* vector, float scalar);
void vector3f_normalize(Vector3f* vector);