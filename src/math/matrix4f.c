#include "math/matrix4f.h"

#include <math.h>

// this is needed because M_PI is not standard for sum reason
#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832
#endif

Matrix4f matrix4f_identity() {
    Matrix4f matrix = {0};

    matrix.values[0] = 1;
    matrix.values[1] = 0;
    matrix.values[2] = 0;
    matrix.values[3] = 0;

    matrix.values[4] = 0;
    matrix.values[5] = 1;
    matrix.values[6] = 0;
    matrix.values[7] = 0;

    matrix.values[8] = 0;
    matrix.values[9] = 0;
    matrix.values[10] = 1;
    matrix.values[11] = 0;

    matrix.values[12] = 0;
    matrix.values[13] = 0;
    matrix.values[14] = 0;
    matrix.values[15] = 1;

    return matrix;
}

Matrix4f matrix4f_multiply(Matrix4f* matrix1, Matrix4f* matrix2) {
    Matrix4f matrix = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                matrix.values[i * 4 + j] += matrix1->values[i * 4 + k] * matrix2->values[k * 4 + j];
            }
        }
    }
    
    return matrix;
}

void matrix4f_scale(Matrix4f* matrix, Vector3f vector) {
    Matrix4f scale_matrix = matrix4f_identity();
    
    scale_matrix.values[0] = vector.x;
    scale_matrix.values[5] = vector.y;
    scale_matrix.values[10] = vector.z;

    *matrix = matrix4f_multiply(matrix, &scale_matrix);
}

void matrix4f_translate(Matrix4f* matrix, Vector3f vector) {
    Matrix4f translate_matrix = matrix4f_identity();

    translate_matrix.values[12] += vector.x;
    translate_matrix.values[13] += vector.y;
    translate_matrix.values[14] += vector.z;

    *matrix = matrix4f_multiply(matrix, &translate_matrix);
}

void matrix4f_rotate(Matrix4f* matrix, float angle, Vector3f vector) {
    angle *= (M_PI / 180.0f);

    float cos_angle = cosf(angle);
    float sin_angle = sinf(angle);
    float minus_one_cos_angle = (1.0f - cos_angle);

    Matrix4f temp = matrix4f_identity();

    temp.values[0] = cos_angle + vector.x * vector.x * minus_one_cos_angle;
    temp.values[1] = vector.y * vector.x * minus_one_cos_angle + vector.z * sin_angle;
    temp.values[2] = vector.z * vector.x * minus_one_cos_angle - vector.y * sin_angle;

    temp.values[4] = vector.x * vector.y * minus_one_cos_angle - vector.z * sin_angle;
    temp.values[5] = cos_angle + vector.y * vector.y * minus_one_cos_angle;
    temp.values[6] = vector.z * vector.y * minus_one_cos_angle + vector.x * sin_angle;

    temp.values[8] = vector.x * vector.z * minus_one_cos_angle + vector.y * sin_angle;
    temp.values[9] = vector.y * vector.z * minus_one_cos_angle - vector.x * sin_angle;
    temp.values[10] = cos_angle + vector.z * vector.z * minus_one_cos_angle;

    *matrix = matrix4f_multiply(matrix, &temp);
}

Matrix4f matrix4f_perspective(float fov, float aspect_ratio, float near_plane, float far_plane) {
    Matrix4f matrix = {0};

    fov *= (M_PI / 180.0f);

    float tan_half_fov = tanf(fov / 2);

    matrix.values[0] = 1 / (aspect_ratio * tan_half_fov);
    matrix.values[1] = 0;
    matrix.values[2] = 0;
    matrix.values[3] = 0;

    matrix.values[4] = 0;
    matrix.values[5] = 1 / tan_half_fov;
    matrix.values[6] = 0;
    matrix.values[7] = 0;

    matrix.values[8] = 0;
    matrix.values[9] = 0;
    matrix.values[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    matrix.values[11] = -1;

    matrix.values[12] = 0;
    matrix.values[13] = 0;
    matrix.values[14] = -(2 * far_plane * near_plane) / (far_plane - near_plane);
    matrix.values[15] = 0;

    return matrix;
}