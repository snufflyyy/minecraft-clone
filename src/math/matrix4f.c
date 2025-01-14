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
    matrix->values[0] *= vector.x;
    matrix->values[5] *= vector.y;
    matrix->values[10] *= vector.z;
}

void matrix4f_translate(Matrix4f* matrix, Vector3f vector) {
    matrix->values[12] += vector.x;
    matrix->values[13] += vector.y;
    matrix->values[14] += vector.z;
}

void matrix4f_rotate(Matrix4f* matrix, float angle, Vector3f vector) {
    angle *= ((float) M_PI / 180.0f);
    vector3f_normalize(&vector);

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

    fov *= ((float) M_PI / 180.0f);

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

Matrix4f matrix4f_look_at(Vector3f position, Vector3f center, Vector3f up) {
    Vector3f forward = vector3f_subtract(&center, &position);
    vector3f_normalize(&forward);
    vector3f_normalize(&up);

    Vector3f side = vector3f_cross_product(&forward, &up);
    up = vector3f_cross_product(&side, &forward);

    Matrix4f matrix = matrix4f_identity();

    matrix.values[0] = side.x;
    matrix.values[1] = up.x;
    matrix.values[2] = -forward.x;

    matrix.values[4] = side.y;
    matrix.values[5] = up.y;
    matrix.values[6] = -forward.y;

    matrix.values[8] = side.z;
    matrix.values[9] = up.z;
    matrix.values[10] = -forward.z;

    matrix.values[12] = -vector3f_dot_product(&side, &position);
    matrix.values[13] = -vector3f_dot_product(&up, &position);
    matrix.values[14] = -vector3f_dot_product(&forward, &position);


    return matrix;
}