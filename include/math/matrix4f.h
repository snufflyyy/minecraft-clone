#pragma once

#include <math/vector3f.h>

typedef struct Matrix4f {
    float values[16];
} Matrix4f;

Matrix4f matrix4f_identity();
Matrix4f matrix4f_multiply(Matrix4f* matrix1, Matrix4f* matrix2);
void matrix4f_scale(Matrix4f* matrix, Vector3f vector);
void matrix4f_translate(Matrix4f* matrix, Vector3f vector);
void matrix4f_rotate(Matrix4f* matrix, float angle, Vector3f vector);
Matrix4f matrix4f_perspective(float fov, float aspect_ratio, float near_plane, float far_plane);
Matrix4f matrix4f_look_at(Vector3f position, Vector3f target, Vector3f up);